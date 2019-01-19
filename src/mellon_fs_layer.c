#include "mellon_fs_layer.h"
/**
 * Initial configurations for file system:
 *      - disable caching of various inode related info
 */
void *mellon_init(struct fuse_conn_info *conn, struct fuse_config *cfg){
    cfg->use_ino = 1;
    cfg->entry_timeout = 0;
	cfg->attr_timeout = 0;
	cfg->negative_timeout = 0;
    return NULL;
}

int mellon_statfs(const char *path, struct statvfs *stfs){
    if(statvfs(path, stfs)==-1)
        return -errno;
    else return 0;
}

/**
 * Check for access permissions (set with umask(0) call in main)
 */
int mellon_access(const char *path, int mask){
    if(access(path, mask)==-1) 
        return -errno;
    else return 0;
}

/**
 * Get file attributes for access control
 */
int mellon_getattr(const char *path, struct stat *st, struct fuse_file_info *fi){
    //Check if file/dir is open
    if((fi && fstat(fi->fh, st)!=-1) || lstat(path, st)!=-1) 
        return 0;
    else return -errno;
}

/**
 * Change owner of dir/file
 */
int mellon_chown(const char *path, uid_t uid, gid_t gid, struct fuse_file_info *fi){
    if((fi && fchown(fi->fh, uid, gid)!=-1) || lchown(path, uid, gid)==-1) 
        return 0;
    else return -errno;
}

/**
 * Change permissions of dir/file
 */
int mellon_chmod(const char *path, mode_t mode, struct fuse_file_info *fi){
    if((fi && fchmod(fi->fh, mode)!=-1) || chmod(path, mode)==-1) 
        return 0;
    else return -errno;
}

/**
 * Make subdirectory
 */
int mellon_mkdir(const char *dir, mode_t mode){
    if(mkdir(dir, mode)==-1) 
        return -errno;
    else return 0;
}

/**
 * Remove directory
 */
int mellon_rmdir(const char *dir){
    if(rmdir(dir)==-1) 
        return -errno;
    else return 0;
}

/**
 * Change file/directory name from old to new
 */
int mellon_rename(const char *old, const char *new, unsigned int flgs){
    if(flgs)
        return -EINVAL;
    else if(!rename(old, new))
        return -errno;
    else return 0;
}


/*
 * Changes directory 
 * Usefull for setting the file hanlder (fi->fh) to be used in mellon_readdir
 */
int mellon_opendir(const char *path, struct fuse_file_info *fi){
    struct current_dir *cdir = (struct current_dir *)malloc(sizeof(struct current_dir));
    if(cdir){
        cdir->dirp=opendir(path);

        if(cdir->dirp){
            cdir->d_entry = NULL;
            cdir->offset = 0;
            fi->fh = (unsigned long) cdir;  //cast for later use
            return 0;
        }else{
            free(cdir);
            return -errno; 
        }
    }else return -ENOMEM;
}

/**
 * List files in directory
 */
int mellon_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags){
    struct current_dir *cdir = (struct current_dir *) (uintptr_t)fi->fh;
    struct stat *st = (struct stat*)calloc(1, sizeof(struct stat));

    if(offset != cdir->offset){
        seekdir(cdir->dirp, offset);
        cdir->offset = offset;
        cdir->d_entry = NULL;
    }

    while(1){
        cdir->d_entry = readdir(cdir->dirp);

        if(cdir->d_entry){
            st->st_ino = cdir->d_entry->d_ino;
		    st->st_mode = cdir->d_entry->d_type << 12;

            cdir->offset = telldir(cdir->dirp);
             
            if(filler(buffer, cdir->d_entry->d_name, NULL, cdir->offset, 0))
                break;

            cdir->d_entry = NULL;
        }else break;
    }

    free(st);
    return 0;
}

int mellon_releasedir(const char *path, struct fuse_file_info *fi){
    struct current_dir *cdir = (struct current_dir *) (uintptr_t)fi->fh;
    closedir(cdir->dirp);
    free(cdir);
    return 0;
}

/**
 * Read symbolic/hard links
 */
int mellon_readlink(const char *path, char *buffer, size_t size){
    int res = readlink(path, buffer, size-1);

    if(res!=-1){
        buffer[res] = 0;
        return 0;
    }else return -errno;
}

/**
 * Create/touch files
 */
int mellon_create(const char *file_name, mode_t mode, struct fuse_file_info *fi){
    int fd = open(file_name, fi->flags, mode);

    if(fd==-1)
        return -errno;

    fi->fh = fd;
    return 0;
}

/*
 * Called for reading a file e.g: cat <file_name>
 */
int mellon_read(const char *file_name, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
    int bytes_r = pread(fi->fh, buf, size, offset);

    return (bytes_r == -1 ? -errno : bytes_r);
}

/**
 * Callback for writing files e.g: echo teste > <file_name>
 */
int mellon_write(const char *file_name, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
    int bytes_w = pwrite(fi->fh, buf, size, offset);

    return (bytes_w == -1 ? -errno : bytes_w);
}

int mellon_truncate(const char *path, off_t size, struct fuse_file_info *fi){
	if((fi && ftruncate(fi->fh, size)!=-1) || truncate(path, size) != -1)
        return 0;
    else return -errno;
}

int mellon_readbuf(const char *path, struct fuse_bufvec **bufp, size_t size, off_t offset, struct fuse_file_info *fi){
    struct fuse_bufvec *temp  = malloc(sizeof(struct fuse_bufvec));
	if(temp){
        *temp = FUSE_BUFVEC_INIT(size);
	    (temp->buf[0]).flags = FUSE_BUF_IS_FD | FUSE_BUF_FD_SEEK;
	    (temp->buf[0]).fd = fi->fh;
	    (temp->buf[0]).pos = offset;
	    *bufp = temp;
        return 0;
    }else return -ENOMEM;
}

int mellon_writebuf(const char *path, struct fuse_bufvec *buf,off_t offset, struct fuse_file_info *fi){
 	struct fuse_bufvec tmp = FUSE_BUFVEC_INIT(fuse_buf_size(buf));

	tmp.buf[0].flags = FUSE_BUF_IS_FD | FUSE_BUF_FD_SEEK;
	tmp.buf[0].fd = fi->fh;
	tmp.buf[0].pos = offset;

	return fuse_buf_copy(&tmp, buf, FUSE_BUF_SPLICE_NONBLOCK);   
}

int mellon_flush(const char *path, struct fuse_file_info *fi){
    if(close(dup(fi->fh) == -1))
		return -errno;
	return 0;
}
#ifdef HAVE_LIBULOCKMGR
int xmp_lock(const char *path, struct fuse_file_info *fi, int cmd, struct flock *lock){
	return ulockmgr_op(fi->fh, cmd, lock, &fi->lock_owner, sizeof(fi->lock_owner));
}
#endif

int mellon_flock(const char *path, struct fuse_file_info *fi, int op){
    if(flock(fi->fh, op) == -1)
		return -errno;
	return 0;
}

int mellon_release(const char *path, struct fuse_file_info *fi){
    close(fi->fh);
    return 0;
}

int mellon_fsync(const char *path, int isdatasync, struct fuse_file_info *fi){
	if(fsync(fi->fh == -1))
		return -errno;
	return 0;
}

int mellon_mknod(const char *path, mode_t mode, dev_t rdev){
	if((S_ISFIFO(mode) && mkfifo(path, mode)!=-1) || mknod(path, mode, rdev)!=-1)
	    return 0;
	else return -errno;
}

int mellon_symlink(const char *from, const char *to){
	if (symlink(from, to) == -1)
		return -errno;
	else return 0;
}

int mellon_unlink(const char *path){
	if (unlink(path) == -1)
		return -errno;
	else return 0;
}

/*
 * Called when opening a file for reading/writing/appending
 */
int mellon_open(const char *file_name, struct fuse_file_info *fi){
    int fh;
    char fa_code[5], user_code[5];
    struct timeval start, end;
    user_code[4]=0;

    if(!send2FACode(fa_code)){
        gettimeofday(&start, NULL);
        read(mellon_fifo_fd, user_code, 4);
        gettimeofday(&end, NULL);

        //Timeout if user takes more than 45 secs do input code
        if(end.tv_sec - start.tv_sec < 45 && !strcmp(user_code, fa_code)){
            fh = open(file_name, fi->flags);
            if(fh!=-1){
                fi->fh = fh;        //update current file handler
                return 0;
            }
        }
    }
    
    return -errno;
}

int main(int argc, char *argv[]){
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    // Dummy values
    current_user.u_name = strdup("m");
    current_user.email = strdup("m");
    current_user.master_key = strdup("m");

    if(fuse_opt_parse(&args, &current_user, mellon_flags, NULL) == -1)
        return 1;

    encrypt_decrypt(ENC_FILE, 1);
    mellon_fifo_fd = open("mellon_fifo", O_RDONLY);
    if(!searchAndUpdate(TXT_FILE)){
        umask(0);
        fuse_main(args.argc, args.argv, &mellon_ops, NULL);
        fuse_opt_free_args(&args);
        return 0;
    }else{
        fprintf(stderr, "Access attempt by unauthorized user detected, exiting...\n");
        return -1;
    }
}
