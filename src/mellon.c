/**
 * 
 *
 */
#define FUSE_USE_VERSION 31

#include<fuse.h>
#include<stdio.h>
#include<stdlib.h>
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

static int mellon_open(const char *file_name, struct fuse_file_info *fi){

}

/**
 * List file attributes
 */
static int mellon_getattr(const char *path, struct stat *st, struct fuse_file_info *fi){
    st->st_uid = getuid();
    st->st_gid = getgid();
    st->st_atime = time(NULL);
    st->st_mtime = time(NULL);

    if(!strcmp(path, "/")){ //root directory relative to mount point
        st->st_mode =  S_IFDIR | 0755;
        st->st_nlink = 2;  //. and ..
    }else{
        st->st_mode = S_IFREG | 0644; 
        st->st_size = 1024;
        st->st_nlink = 1;
    }
    return 0;
}

static int read_mellon(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags){
    printf("Mellon: listing files\n");
    if(!strcmp(path, "/")){
        filler(buffer, ".", NULL, 0, 0);                    //FIX: offset
        filler(buffer, "dummy_file_2", NULL, 0, 0);
        filler(buffer, "dummy_file", NULL, 0, 0);
    }
    return 0;
}


static struct fuse_operations mellon_ops = {
    .init = 0,                                  //called when mounting the filesystem
    .getattr = mellon_getattr,                  
    .readdir = read_mellon,                     //called when listing a directory
    .open = mellon_open                         //called when opening a file
};

int main(int argc, char *argv[]){
    struct fuse_args args;

    if(fuse_opt_parse(NULL, NULL, NULL, NULL)==-1)
        exit(1);

    fuse_main(argc, argv, &mellon_ops, NULL);

}
