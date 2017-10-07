#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//#include "header.c"


//Simle function using reader 
char* bodyLoader(char* path){

    //dprintf(2,"bodyLoader(): %s\n", path);
    int fd = open(path, O_RDONLY);
    if(fd == -1){
        return "No Such File";
    }

    char* body = reader('R', fd);
    close(fd);

    return body;
}

void mimeResponseFiller(char* noun, resp_t* resp){
    //Make iterator point to string that holds extension 
    char* iterator = noun;
    int counter = 0;
    while(*(iterator+counter) != '\0'){
        counter++;
    }
    while((iterator+counter) != noun){
        if(*(iterator+counter) == '.') break;
        counter--;
    }//Iterator now points to extension start or noun
    
    //Automatic mimetype fill in
    //(*resp).content_ext  = mimeMatcher('e', iterator);
    dprintf(2,"mimeResponseFiller(): %s\n", (iterator+counter));
    (*resp).content_type = mimeMatcher('t', (iterator+counter));
    
    dprintf(2,"mimeResponseFiller(): %s\n",(*resp).content_type);
    return;
}

void responsePrinter(resp_t resp){

    dprintf(1,"%s %d %s\n%s %s\n\n%s",\
        resp.http, resp.statcode, resp.statcode_verbose,\
        CTI, resp.content_type,\
        resp.body);

}

void pngSender(resp_t resp, char* path){

    //Get Picture Size
    FILE *picture;
    picture = fopen(path, "r");
    int size;
    fseek(picture, 0, SEEK_END);
    size = ftell(picture);
    fseek(picture, 0, SEEK_SET);

    dprintf(1,"%s %d %s\n%s %s\n\n",\
        resp.http, resp.statcode, resp.statcode_verbose,\
        CTI, resp.content_type);
    //Send Picture as Byte Array
    char send_buffer[size];
    while(!feof(picture)) {
        fread(send_buffer, 1, sizeof(send_buffer), picture);
        write(1, send_buffer, sizeof(send_buffer));
    }

    exit(0);
}

void get(char flag, req_t* req){
    resp_t resp;

    dprintf(2, "naGnH(): %s\n", (*req).noun);
    if(-1 == access((*req).noun, R_OK)){

        //If file NOT accessible or NOT found
        resp.http             = HTTP                    ;
        resp.statcode         = Not_Found               ;
        resp.statcode_verbose = statcodesV(Not_Found)   ;
        mimeResponseFiller(NOT_FOUND_FILE, &resp);
        
        if(flag == 'R'){
            resp.body         = bodyLoader(NOT_FOUND_FILE);

        }else if(flag == 'H'){
            resp.body         = ""                      ;
        }
    }else{

        //If file accessible YAY
        resp.http             = HTTP                    ;
        resp.statcode         = Ok                      ;
        resp.statcode_verbose = statcodesV(Ok)          ;
        //passing noun and response struct address
        mimeResponseFiller((*req).noun, &resp);

        if(flag == 'R'){
            //GET
            if(resp.content_type == mimes.png.type){
                pngSender(resp, (*req).noun);
            }
            resp.body             = bodyLoader((*req).noun) ;
        }else if(flag == 'H'){
            //HEAD
            resp.body             = ""                      ;
        }

    }
    //Next one responsible for file printing
    responsePrinter(resp);
    exit(1);
}

int nonApi(req_t* req){

    dprintf(2, "nonAPI(): %s\n", (*req).noun);

    if(strcmp((*req).verb, verbs.get) == 0){
        //GET
        get('R', req);
        return 0;
    }else if(strcmp((*req).verb, verbs.head) == 0){
        //GET(HEAD)
        get('H', req);
        return 0;
    }else{
        //Method_Not_Allowed
        dprintf(1, "%d %s\n",Method_Not_Allowed, statcodesV(Method_Not_Allowed));
        exit(1);
    }
}