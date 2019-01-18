#include "auth_module.h"
/**
 * Generate random number between 0000 and 9999
 * and send it to the current user
 * returns -1 if error occurs
 */
int send2FACode(char *buf){
    char *email_body = (char*)malloc(sizeof(char)*(strlen(POST_BODY)+strlen(current_user.email)+strlen(FROM)+4));
    CURL *curl;   
    CURLcode res = CURLE_OK;
    struct curl_slist *header_params = NULL;

    curl = curl_easy_init();

    if(curl){
        getentropy(buf, sizeof(char)*4);
        for(int i = 0; i < 4; i ++)
            *(buf+i) = (*(buf+i) % 10) + 48;      //convert random bytes in ascii numbers
    
        sprintf(email_body, POST_BODY, current_user.email, FROM, buf);

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.sendgrid.com/v3/mail/send");
        header_params = curl_slist_append(header_params, "Authorization: Bearer");
        header_params = curl_slist_append(header_params, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_params);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, email_body);
        
        res = curl_easy_perform(curl);
        if(res!=CURLE_OK){
            fprintf(stderr, "Couldn't send email address to: %s : (%s)\n", current_user.email, curl_easy_strerror(res));
            return -1;
        }
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    free(email_body);
    return 0;
}


