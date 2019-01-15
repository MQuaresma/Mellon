#define FUSE_USE_VERSION 31
#include<fuse.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>

static void *mellon_init(struct fuse_conn_info *, struct fuse_config *);
static int mellon_access(const char *, int);
static int mellon_getattr(const char *, struct stat *, struct fuse_file_info *);
static int mellon_chown(const char *, uid_t, gid_t, struct fuse_file_info *);
static int mellon_chmod(const char *, mode_t, struct fuse_file_info *);
static int mellon_mkdir(const char *, mode_t);
static int mellon_rmdir(const char *);
static int mellon_readdir(const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info *, enum fuse_readdir_flags);
static int mellon_open(const char *, struct fuse_file_info *);
static int mellon_read(const char *, char *, size_t, off_t, struct fuse_file_info *);

int gen2FACode();

static struct fuse_operations mellon_ops = {
    .init = mellon_init,                                      //called when mounting the filesystem
    .access = mellon_access,
    .getattr = mellon_getattr,                  
    .chown = mellon_chown,
    .chmod = mellon_chmod,
    .readdir = mellon_readdir,                      //called when listing a directory
    .mkdir = mellon_mkdir,
    .rmdir = mellon_rmdir,
    .create = mellon_create,
    .open = mellon_open,                            //called when opening a file
    .read = mellon_read
    /*.write = mellon_write,
    .rename = mellon_rename,
    */

};
