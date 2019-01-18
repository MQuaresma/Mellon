#define FUSE_USE_VERSION 31
#ifdef __APPLE__
#include<fuse/fuse_darwin.h>
#include<sys/stat.h>
#include<fuse/fuse.h>
#include<fuse/fuse_common_compat.h>
#include<sys/random.h>
#endif
#include<fuse.h>
#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<dirent.h>
#include<curl/curl.h>


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
int mellon_rename(const char*, const char*, unsigned int);
int mellon_create(const char *, mode_t , struct fuse_file_info *);
int mellon_open(const char *, struct fuse_file_info *);
int mellon_read(const char *, char *, size_t, off_t, struct fuse_file_info *);
int mellon_write(const char *, const char *, size_t, off_t, struct fuse_file_info *);

#define POST_BODY "{\"personalizations\": [{\"to\": [{\"email\": \"%s\"}]}],\"from\": {\"email\": \"%s\"},\"subject\": \"Auth Code\",\"content\": [{\"type\": \"text/plain\", \"value\": \"%s\"}]}"
#define FROM "example@gmail.com"
int send2FACode(char *);

struct current_dir{
    DIR* dirp;
    struct dirent *d_entry;
    int offset;
};

struct trusted_user{
    char *u_name;
    char *email;
};

static const struct fuse_operations mellon_ops = {
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
    .readlink = mellon_readlink,
    .rename = mellon_rename,
    .create = mellon_create,
    .open = mellon_open,
    .read = mellon_read,
    .write = mellon_write
};


static const struct fuse_opt mellon_flags[] = {
    {"--user=%s", offsetof(struct trusted_user, u_name), 1},
    {"--email=%s", offsetof(struct trusted_user, email), 1},
    FUSE_OPT_END
};

static struct trusted_user current_user;

/*struct trusted_user acl[10] = {
    {strdup("miguel"), strdup("miguelmirq@gmail.com")}
};*/
