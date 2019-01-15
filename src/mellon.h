#define FUSE_USE_VERSION 31
#include<fuse.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<dirent.h>

static void *mellon_init(struct fuse_conn_info *, struct fuse_config *);
static int mellon_statfs(const char*, struct statvfs *);
static int mellon_access(const char *, int);
static int mellon_getattr(const char *, struct stat *, struct fuse_file_info *);
static int mellon_chown(const char *, uid_t, gid_t, struct fuse_file_info *);
static int mellon_chmod(const char *, mode_t, struct fuse_file_info *);
static int mellon_mkdir(const char *, mode_t);
static int mellon_rmdir(const char *);
static int mellon_readdir(const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info *, enum fuse_readdir_flags);
static int mellon_opendir(const char *, struct fuse_file_info *);
static int mellon_rename(const char*, const char*, unsigned int);
static int mellon_create(const char *, mode_t , struct fuse_file_info *);
static int mellon_open(const char *, struct fuse_file_info *);
static int mellon_read(const char *, char *, size_t, off_t, struct fuse_file_info *);
static int mellon_write(const char *, const char *, size_t, off_t, struct fuse_file_info *);


int gen2FACode();

static struct fuse_operations mellon_ops = {
    .init = mellon_init,
    .statfs = mellon_statfs,
    .access = mellon_access,
    .getattr = mellon_getattr,                  
    .chown = mellon_chown,
    .chmod = mellon_chmod,
    .mkdir = mellon_mkdir,
    .rmdir = mellon_rmdir,
    .opendir = mellon_opendir, 
    .readdir = mellon_readdir,
    .rename = mellon_rename,
    .create = mellon_create,
    .open = mellon_open,
    .read = mellon_read,
    .write = mellon_write
};

struct trusted_user{
    char u_name[100];
    char email[100];
};

struct current_dir{
    DIR* dirp;
    struct dirent *d_entry;
    int offset;
};
