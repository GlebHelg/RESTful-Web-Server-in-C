#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


req_t parser(char* bufClient){

    req_t req = reqDefiner();

    //String iterator 
    char* scanner        = bufClient;
    int lineCounter      = 1;
    int counter          = 0;
    int asciNullCount    = 0;
    
    //Sets verb to point to start of client request. 
    req.verb = bufClient;

    //char* mimeEnd; int mimeFlag = 0;
    while(1){
        //Verb finder
        if(asciNullCount == 0 && *(scanner+counter) == ' '){
            *(scanner+ counter) = '\0';
            asciNullCount++;
            counter++;
            req.noun = (scanner+counter);
            continue;
        }
        //Noun terminator
        if(asciNullCount == 1 && *(scanner+counter) == ' '){
            *(scanner+counter) = '\0';
            asciNullCount++;
            counter++;
            continue;
        }
        //MIME finder
        if(*(scanner+counter) == 'C' && strncmp((scanner+counter), CTI, 13) == 0){
            req.mime = (scanner+counter+14);
            //MIME end finder. Terminated after while.
            while(*(scanner+counter) != '\n'){
                if(*(scanner+counter) == '\r')
                {
                    *(scanner+counter) = '\0';
                }
                counter++;
            }
            //mimeFlag = 1;
            //mimeEnd = (scanner+counter);
            continue;
        }
        //Body finder
        if(*(scanner+counter) == '\n' && *(scanner+counter+1) == '\n'){
            counter+=2;
            req.body = (scanner+counter);
            //No need to null terminate, allready nullterminated in buffer.
            //Whole request scanned
            break;
        }else if(*(scanner+counter) == '\r' && *(scanner+counter+1) == '\n' && *(scanner+counter+2) == '\r' && *(scanner+counter+3) == '\n'){
            counter+=4;
            req.body = (scanner+counter);
            //No need to null terminate, allready nullterminated in buffer.
            //Whole request scanned
            break;
        }
        counter++;
    }
    /*/Nullterminates mime type if mime exist, reset mimeFlag
    if(mimeFlag == 1){
        *mimeEnd = '\0';
        mimeFlag = 0;
    }*/

    if(strcmp(req.noun, "/") == 0){
        req.noun = DEFAULT_FILE;
    }

    dprintf(2,"parser(): %s\n",req.verb);
    dprintf(2,"parser() noun: %s\n",req.noun);
    dprintf(2,"parser(): %s\n",req.mime);
    dprintf(2,"parser(): %s\n",req.body);

    //Returning by value
    return req;
}


/*
int main(){

char* request = \
"
GET / HTTP/1.1\n\
Host: localhost\n\
Connection: keep-alive\n\
Cache-Control: max-age=0\n\
Upgrade-Insecure-Requests: 1\n\
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,**;q=0.8\n\
Accept-Encoding: gzip, deflate, sdch, br\n\
Accept-Language: en-US,en;q=0.8,nb;q=0.6\n\\n\
";
    req_t req = parser();

    return 0;
}//*/