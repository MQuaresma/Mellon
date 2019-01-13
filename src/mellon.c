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

int main(){

}
