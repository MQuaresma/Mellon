#include "mellon_common_layer.h"
#include<curl/curl.h>

#define POST_BODY "{\"personalizations\": [{\"to\": [{\"email\": \"%s\"}]}],\"from\": {\"email\": \"%s\"},\"subject\": \"MellonFS Auth Code\",\"content\": [{\"type\": \"text/plain\", \"value\": \"%s\"}]}"
#define AES_IV "0987654321"
#define DEC "openssl enc -aes-256-cbc -in %s -out %s -K %s -iv %s -d"
#define ENC "openssl enc -aes-256-cbc -in %s -out %s -K %s -iv %s && rm %s"
