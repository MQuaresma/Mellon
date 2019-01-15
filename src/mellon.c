/**
 * 
 *
 */
#define FUSE_USE_VERSION 31

#include<fuse/fuse.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#define MOD 10000

/**
 * Generate random number between 0000 and 9999
 * returns -1 if error occurs
 */
int gen2FACode(){
	return 1;
	/*bytes fa_code_byte[sizeof(int)];
	int fa_code;

	if(RAND_bytes(fa_code_byte, sizeof(fa_code)) != 1){
		fprintf(stderr, "Couldn't generated 2FA code, exiting...\n");
		return -1;
	}*/
}

/**
 * Initial configurations for file system:
 *      - disable caching of various inode related info
 */
static void *mellon_init(struct fuse_conn_info *conn, struct fuse_config *cfg){
    cfg->use_ino = 1;
    cfg->nullpath_ok = 1;
    cfg->entry_timeout = 0;
	cfg->attr_timeout = 0;
	cfg->negative_timeout = 0;
    return NULL;
}


/**
 * Check for access permissions (set with umask(0) call in main)
 */
static int mellon_access(const char *path, int mask){
    if(access(path, mask)==-1) 
        return -errno;
    else return 0;
}

/**
 * Get file attributes for access control
 */
static int mellon_getattr(const char *path, struct stat *st, struct fuse_file_info *fi){
    if(fi) //check if file is open
        fstat(fi->fh, st);
    else 
        lstat(path, st);
    return 0;
}

/**
 * Make subdirectory
 */
static int mellon_mkdir(const char *dir, mode_t mode){
    if(mkdir(dir, mode)==-1) 
        return -errno;
    else return 0;
}

/**
 * Remove directory
 */
static int mellon_rmdir(const char *dir){
    if(rmdir(dir)==-1) 
        return -errno;
    else return 0;
}

/**
 * List files in directory
 * FIX
 */
static int mellon_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags){
    if(!strcmp(path, "/")){
        filler(buffer, ".", NULL, 0, 0);
        filler(buffer, "..", NULL, 0, 0);
        filler(buffer, "dummy_file", NULL, 0, 0);
        filler(buffer, "dummy_file_1", NULL, 0, 0);
    }
    return 0;
}

/*
 * Called when opening a file for reading/writing/appending
 */
static int mellon_open(const char *file_name, struct fuse_file_info *fi){
    char fa_code[5];

    puts("Enter access code: ");
    fgets(fa_code, sizeof(fa_code), stdin);

    if(!strcmp(fa_code, "1\n"))
        return 0;
    return -errno;
}

/*
 * Called for reading a file e.g: cat <file_name>
 * FIX: remove stubs/dummy files
 */
static int mellon_read(const char *file_name, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
    char dummy_content[] = "hello, this is just some test content";
    int len = strlen(dummy_content);
    int c_read = (len-offset < size ? len-offset : size); //read max amount of chars possible

    if(!strcmp(file_name, "/dummy_file") || !strcmp(file_name, "/dummy_file_1")){
        memcpy(buf, dummy_content+offset, c_read);
        return c_read; 
    }else
        return -1;
    
}


static struct fuse_operations mellon_ops = {
    .init = mellon_init,                                      //called when mounting the filesystem
    .getattr = mellon_getattr,                  
    .access = mellon_access,
    .readdir = mellon_readdir,                      //called when listing a directory
    .open = mellon_open,                            //called when opening a file
    .read = mellon_read
    /*.write = mellon_write,
    .create = mellon_create,
    .rename = mellon_rename,
    .chmod = mellon_cmod,
    .chown = mellon_chown,*/

};

int main(int argc, char *argv[]){
    umask(0); //remove all restrictions
    fuse_main(argc, argv, &mellon_ops, NULL);
}
