#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define UA "Mozilla/5.0 (X11; Linux x86_64; rv:52.0) Gecko/20100101 Firefox/52.0"

#define URL "http://translate.googleapis.com/translate_a/single?client=gtx&sl=en&tl=ru&dt=t&q=\""

char * printData(char *p);
size_t write_data(char *data, size_t size, size_t nmemb);

int main(int argc, char *argv[])
{
    CURL *curl = curl_easy_init();    
    
    if (argc != 2) {
        printf("Usage: %s \"text\"\n", argv[0]);
        return 1;
    }    

    if (!curl) {
        puts("curl error");    
        return 1;
    }

    CURLcode res;
    char *rdata, *urldata;
    int len;
    
    len = strlen(argv[1]);
    rdata = curl_easy_escape(curl, argv[1], len);
    len += strlen(rdata) + 512;
    urldata = (char *)calloc(len, sizeof(char));
    strcpy(urldata, URL);
    strcat(urldata, rdata);
    strcat(urldata, "\"");
    curl_easy_setopt(curl, CURLOPT_URL, urldata);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, UA);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
        printf("Response error");
    curl_easy_cleanup(curl);
    free(urldata);

    return 0;
}


size_t write_data(char *data, size_t size, size_t nmemb)
{
    char *p = data + 4;
    
    if (*p == '\\' && *(p + 1) == '\"')
        p += 2;

    p = printData(p);
    
    while (*p) {
        if (*p == ']' && *(p + 1) == ',' && *(p + 2) == '['
            && *(p + 4) != 'e' && *(p + 5) != 'n') {
            p += 4;
            p = printData(p);
        }
        ++p;
    }
    
    putchar('\n');
    
    return  size * nmemb;
}

char * printData(char *p)
{
    while (*p) {
        if (*p == '\\' && *(p + 1) == '\"' && *(p + 2) == '\"'
            && *(p + 3) == ',')
            break;
        else if (*p == '\"' && *(p + 1) == ',' && *(p + 2) == '\"')
           break;
       putchar(*p++);
    }

    return p;
}
