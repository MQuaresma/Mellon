/**
 * 
 *
 */
#define FUSE_USE_VERSION 31

#include<fuse.h>
#include<stdio.h>
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

static int mellon_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi){

}


static struct fuse_operations mellon_ops = {
    .init = 0,                                  //called when mounting the filesystem
    .getattr = mellon_getattr,                  
    .open = mellon_open                         //called when opening a file
};

int main(int argc, char *argv[]){

    fuse_main(argc, argv, &mellon_ops, NULL);

}
