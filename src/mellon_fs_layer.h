#define FUSE_USE_VERSION 31
#include "mellon_common_layer.h"
#include<fuse.h>
#include<stddef.h>
#include<errno.h>
#include<fcntl.h>
#include<dirent.h>
#include<sys/time.h>

void *mellon_init(struct fuse_conn_info *, struct fuse_config *);
int mellon_statfs(const char*, struct statvfs *);
int mellon_access(const char *, int);
int mellon_getattr(const char *, struct stat *, struct fuse_file_info *);
int mellon_chown(const char *, uid_t, gid_t, struct fuse_file_info *);
int mellon_chmod(const char *, mode_t, struct fuse_file_info *);
int mellon_mkdir(const char *, mode_t);
int mellon_rmdir(const char *);
int mellon_readdir(const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info *, enum fuse_readdir_flags);
int mellon_readlink(const char *, char *, size_t);
int mellon_opendir(const char *, struct fuse_file_info *);
int mellon_releasedir(const char *, struct fuse_file_info *);
int mellon_rename(const char*, const char*, unsigned int);
int mellon_create(const char *, mode_t , struct fuse_file_info *);
int mellon_open(const char *, struct fuse_file_info *);
int mellon_read(const char *, char *, size_t, off_t, struct fuse_file_info *);
int mellon_write(const char *, const char *, size_t, off_t, struct fuse_file_info *);
int mellon_truncate(const char *, off_t, struct fuse_file_info *);
int mellon_readbuf(const char *, struct fuse_bufvec **, size_t, off_t, struct fuse_file_info *);
int mellon_writebuf(const char *, struct fuse_bufvec *,off_t, struct fuse_file_info *);
int mellon_flush(const char *, struct fuse_file_info *);
#ifdef HAVE_LIBULOCKMGR
int mellon_lock(const char *, struct fuse_file_info *, int, struct flock *);
#endif
int mellon_flock(const char *, struct fuse_file_info *, int);
int mellon_release(const char *, struct fuse_file_info *);
int mellon_fsync(const char *, int, struct fuse_file_info *);
int mellon_mknod(const char *, mode_t, dev_t);
int mellon_symlink(const char *, const char *);
int mellon_unlink(const char *);

struct current_dir{
    DIR* dirp;
    struct dirent *d_entry;
    int offset;
};

int mellon_fifo_fd;

static const struct fuse_operations mellon_ops = {
    .init       = mellon_init,
    .statfs     = mellon_statfs,
    .access     = mellon_access,
    .getattr    = mellon_getattr,                  
    .chown      = mellon_chown,
    .chmod      = mellon_chmod,
    .mkdir      = mellon_mkdir,
    .rmdir      = mellon_rmdir,
    .opendir    = mellon_opendir, 
    .readdir    = mellon_readdir,
    .readlink   = mellon_readlink,
    .releasedir = mellon_releasedir,
    .rename     = mellon_rename,
    .create     = mellon_create,
    .open       = mellon_open,
    .read       = mellon_read,
    .write      = mellon_write,
    .truncate   = mellon_truncate,
    .read_buf   = mellon_readbuf,
    .write_buf  = mellon_writebuf,
    .flush      = mellon_flush,
    #ifdef HAVE_LIBULOCKMGR
    .lock		= mellon_lock,
    #endif
    .flock		= mellon_flock,
    .release	= mellon_release,
	.fsync		= mellon_fsync,
    .mknod      = mellon_mknod,
    .symlink    = mellon_symlink,
    .unlink     = mellon_unlink
};

static const struct fuse_opt mellon_flags[] = {
    {"--user=%s", offsetof(struct trusted_user, u_name), 1},
    {"--email=%s", offsetof(struct trusted_user, email), 1},
    {"--master_key=%s", offsetof(struct trusted_user, master_key), 1},
    FUSE_OPT_END
};
