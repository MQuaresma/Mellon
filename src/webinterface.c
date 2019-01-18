#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char *argv[]){
    printf("Content-type: text/html\n\n");
    printf("<html><title>MellonFS-Web Interface</title><body>\n");
    printf("<form action=\"/cgi-bin/run.sh\" method=\"get\">");
    printf("Authentication Code:");
    printf("<input type=\"text\" name=\"fa_code\">");
    printf("<input type=\"submit\">");
    printf("</form>\n");
    printf("</body></html>");
    return 0;
}
