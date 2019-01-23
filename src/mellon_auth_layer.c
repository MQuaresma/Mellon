#include "mellon_auth_layer.h"
/**
 * Generate random number between 0000 and 9999
 * and send it to the current user
 * returns -1 if error occurs
 */
int send2FACode(char *buf){
    char *email_body = (char*)malloc(sizeof(char)*(strlen(POST_BODY)+strlen(current_user.email)*2+4));
    CURL *curl;   
    CURLcode res = CURLE_OK;
    struct curl_slist *header_params = NULL;

    curl = curl_easy_init();

    if(curl){
        getentropy(buf, sizeof(char)*4);
        for(int i = 0; i < 4; i ++)
            *(buf+i) = (*(buf+i) % 10) + 48;      //convert random bytes in ascii numbers
    
        sprintf(email_body, POST_BODY, current_user.email, current_user.email, buf);

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.sendgrid.com/v3/mail/send");
        header_params = curl_slist_append(header_params, "Authorization: Bearer");
        header_params = curl_slist_append(header_params, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_params);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, email_body);
        
        res = curl_easy_perform(curl);
        if(res!=CURLE_OK){
            free(email_body);
            fprintf(stderr, "Couldn't send email address to: %s : (%s)\n", current_user.email, curl_easy_strerror(res));
            return -1;
        }
        curl_easy_cleanup(curl);
    }

    free(email_body);
    curl_global_cleanup();
    return 0;
}


int encrypt_decrypt(char *source, int enc_dec){
    char payload[200]; 
    if(enc_dec){ //decrypt
        if(access(source, F_OK)==-1){ //No ACL found
            sprintf(payload, "touch %s", TXT_FILE);
        }else{
            sprintf(payload, DEC, ENC_FILE, TXT_FILE, AES_KEY, AES_IV);
        }
    }else{      //encrypt
        sprintf(payload, ENC, TXT_FILE, ENC_FILE, AES_KEY, AES_IV, source);
    }

    system(payload);
    return 1;
}

int searchAndUpdate(char *source){
    FILE *acl_file = fopen(source, "r");
    char *acl_entry=NULL;
    int rd, ret_val=-1;
    size_t in_len=0;

    if(acl_file){
        while((rd=getline(&acl_entry, &in_len, acl_file))!=-1){
            if(!strcmp(strtok(acl_entry, ":"), current_user.u_name))
                break;
            free(acl_entry);
        }

        fclose(acl_file);
        if(rd==-1){
            if(!strcmp(current_user.master_key, AES_KEY) && (acl_file = fopen(source, "a"))){
                fprintf(acl_file, "%s:%s\n", current_user.u_name, current_user.email);
                fflush(acl_file);
                fclose(acl_file);
                ret_val=0;
            }
        }else{
            acl_entry[rd-1]=0;                              //remove new line character
            current_user.email = strdup(strtok(NULL, ":"));
            free(acl_entry);
            ret_val=0;
        }
    }
    encrypt_decrypt(TXT_FILE, 0);
    return ret_val;
}
