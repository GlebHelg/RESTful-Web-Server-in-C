#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//#include "header.c"

#define CHUNKSIZE 8000

int bodyComplete(char* buffer, int offset, int rdTot, int rd){

    char* xmlStart = "<?xml version=\"1.0\"?>";
    int   startLen = strlen(xmlStart);

    if((rdTot - offset) < startLen && rd == CHUNKSIZE){
        //Start tag not complete
        return 0;
    }

    //Comparing start tag
    if(strncmp(&buffer[offset], xmlStart, startLen) == 0){
        //If here, it's an XML document
        char* xmlEnd   = "</contacts>";
        int   endLen   = strlen(xmlEnd);

        int looper = 0;
        while(looper < rdTot){
            char* cmpStart = &buffer[offset+looper];

            //Looking for body end tag
            if(strncmp(cmpStart, xmlEnd, endLen) == 0){
                return 1;
            }
            looper++;
        }
        //If end of buffer, but no match, not complete XML
        return 0;

    }else{
        //If here, not XML, body must be empty anyways
        return 1;
    }
}

char* reader(char prot, int fd){

    char* buffer = malloc(CHUNKSIZE);
    char* tmp;

    int rd      = 0;
    int rdTot   = 0;
    int rdCount = 0;

    int fullRequest = 1;
    dprintf(2, "About to read for a while\n");
    //Read 4eva
    while(1){
        //Denne readen leser maksimalt CHUNKSIZE, minimalt 1
        rd = read(fd, buffer+rdTot, CHUNKSIZE-(rdTot % CHUNKSIZE));
        rdTot += rd;
        rdCount++;

        if(rd < 0){
            //Error
            fprintf(stderr, "Read error in while in reader()\n");
            //NEED INTERNAL SERVER ERROR RESPONSE
            break;
        }

        //For HTTP protocol
        if(prot == 'H'){
            fullRequest = 0;

            //holds the offset from start to body
            int offset  = 0;
            int found2NL = 0;
            //Code for finding double newline. HTTP specific.
            while(offset<rdTot-1){
                if(buffer[offset] == '\n' && buffer[offset] == buffer[offset+2]){
                    //CRLF
                    found2NL = 1;
                    offset += 3;
                    //Offset now points to start of body
                    break;
                }else if(buffer[offset] == '\n' && buffer[offset] == buffer[offset+1]){
                    //LF
                    found2NL = 1;
                    offset += 2;
                    break;
                }
                offset++;
            }

            //True if double NL found, check if body complete
            if(found2NL == 1){
                //Need to check if whole body has been read, but only for XML
                fullRequest = bodyComplete(buffer, offset, rdTot, rd);
                
                if(fullRequest == 1){
                    //Since we now have the whole request
                    dprintf(2, "Ensured complete request\n");
                    break;
                }
            }
        }//End of HTTP protocol if() */

        if(rdTot % CHUNKSIZE == 0){
            //Realloc
            tmp = realloc(buffer, CHUNKSIZE*(rdCount+1));
            if(tmp == NULL){
                dprintf(2, "Realloc error!");
                return NULL;
                //exit(0);
            }
            buffer = tmp;
            
        }else if(rd > 0){
            //Continue reading
            //There will allways be place for 1 extra character in buffer.
            //if max = 1000, rd = 999, next rd=0, rdTot = 999, buffer[999] is unused, buffer[999]='\0'
            buffer[rdTot] = '\0';
            if(fullRequest == 1){
                break;
            }
        }
    }//End of While loop

    if(fullRequest == 1){
        return buffer;
    }else{
        return NULL;
    }
}

/*
int main(){

    int fd = open("/www/REQUEST.http", O_RDONLY);
    if(fd == -1){
        printf("Open error\n");
    }

    char* body = reader('H', fd);
    close(fd);

    if(body != NULL){
        printf("%s", body);
    }else{
        printf("Not a complete HTTP request...\n");
    }

}//*/