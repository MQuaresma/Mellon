#include<sys/file.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define AES_KEY "123456789"
#define ENC_FILE "mellon_acl.enc"
#define TXT_FILE "mellon_acl.txt"

struct trusted_user{
    char *u_name;
    char *email;
    char *master_key;
};

struct trusted_user current_user;

int send2FACode(char *);
int searchAndUpdate(char *);
int encrypt_decrypt(char *, int);