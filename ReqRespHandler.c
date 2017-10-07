#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "header.c"
#include "reader.c"
#include "parser.c"
#include "nonAPI.c"
#include "API.c"

int reqRespHandler(){
    /*/De neste to linjene kun nødvendig under testing
    char* path    = "/www/REQUEST"; 
    int fd        = open(path, O_RDONLY);/*/
    //Neste linje for socket
    int fd = 0;//*/

    //De to neste burde helst vært preprosessert(well, oh, the precompiler) 
    mimes = mimeDefiner();
    verbs = verbDefiner();

    //reader(prot, fd)
    //prot can be  : H=HTTP or R=Regular
    //fd is an int, representing a position in the fd-table. Used by read() in reader();
    char* buffer  = reader('H', fd);
    dprintf(2,"Reader done:\n%s", buffer);

    //For security reasons we close standard input after request is fully read
    //close(fd);

    if(buffer == NULL){
        //Dont know when that happens, gotta figure something
    }

    //Parser reads the buffer and builds a req_t struct
    req_t req  = parser(buffer);
    dprintf(2, "respHand(): %s\n", req.noun);
    dprintf(2, "Parser done\n");

    //Frigjør bufferet. Kun mulig pga malloc. Også grunnen til at man kan returnere
    //en peker fra en funksjon, og fortsatt kunne aksessere verdien.
    //free(buffer);

    //nonAPI() tar imot en peker til en req_t struct 
    //int q         = nonApi(&req);
    //dprintf(2, "nonAPI done\n");

//Den kommenterte delen er en req delegator
    if(strncmp(req.noun, API, 4) == 0){
        if(Api(&req) == -1){
            //Internal server error
            exit(1);
        }
        //If everything good, return normally
        dprintf(2, "API done\n");
        return 0;
    }else{
        if(nonApi(&req) == -1){
            //Internal server error
            dprintf(2, "nonAPI done\n");
            exit(1);
        }
        //If everything good, return normally
        return 0;
    }

    //dprintf assures of complete reqResp cycle(Testing purposes)
    //dprintf(2, "\nq: %d\n", q);
}