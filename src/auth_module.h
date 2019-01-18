#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<curl/curl.h>
#include <sys/time.h>

#define POST_BODY "{\"personalizations\": [{\"to\": [{\"email\": \"%s\"}]}],\"from\": {\"email\": \"%s\"},\"subject\": \"Auth Code\",\"content\": [{\"type\": \"text/plain\", \"value\": \"%s\"}]}"
#define FROM "example@gmail.com"

extern struct trusted_user current_user;
