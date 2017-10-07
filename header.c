#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HTTP "HTTP/1.1"
#define CTI "Content-Type:"

/////NOUNs (default)///////////////////////
#define DEFAULT_FILE "/index.html"
#define API "/api"
#define NOT_FOUND_FILE "/notFound.html"

//////VERB///////////////////////////////////
        typedef struct{

            char* get;
            char* head;
            char* put;
            char* post;
            char* delete;
            
            char* v[5];
        }verbs_t;
//Function
verbs_t verbDefiner(){
verbs_t verbs;
verbs.get    = "GET";
verbs.head   = "HEAD";
verbs.put    = "PUT";
verbs.post   = "POST";
verbs.delete = "DELETE";
verbs.v[0]   = verbs.get;
verbs.v[1]   = verbs.head;
verbs.v[2]   = verbs.put;
verbs.v[3]   = verbs.post;
verbs.v[4]   = verbs.delete;
return verbs;
}
//Globally available
//Remember to initialize. And it is not constant.
verbs_t verbs;
///////////////////////////////////////////// 

//////////MIME/////////////////////////////////////////////////////////////
        typedef struct mimestruct{
            char* type;
            char* ext;
        }mime_t;

        typedef union mu{
            struct{
                mime_t html;
                mime_t plain;
                mime_t png;
                mime_t xml;
                mime_t xxml;
                mime_t css;
                mime_t xmldtd;
                mime_t js;
            };
            mime_t array[8];
        }mimeu_t;
//Function
mimeu_t mimeDefiner(){
mimeu_t mime;
//NonAPI
mime.html.type   = "text/html";              mime.html.ext   = ".html";
mime.plain.type  = "text/plain";             mime.plain.ext  = ".txt";
mime.png.type    = "image/png";              mime.png.ext    = ".png";
mime.xml.type    = "application/xml";        mime.xml.ext    = ".xml";
mime.xxml.type   = "text/xsl";               mime.xxml.ext   = ".xsl";
mime.css.type    = "text/CSS";               mime.css.ext    = ".css";
//API
mime.xmldtd.type = "application/xml-dtd";    mime.xmldtd.ext = ".dtd";
mime.js.type     = "application/javascript"; mime.js.ext     = ".js";
return mime;
}
//Globally available variable
//Remember to initialize. And dont mess it up, it is not constant!
mimeu_t mimes;

char* mimeMatcher(char flag, char* string){
    //Flag e returns ext, flag t returns type
    //dprintf(2, "mimeMatcher(): %s\n", string); 
    if(flag == 't'){
        for(int i = 0; i<8; i++)
            if(strcmp(string, mimes.array[i].ext) == 0){
                return mimes.array[i].type;
            }
            //If no correct matches
            return mimes.plain.type; //Default
            
    }else if(flag == 'e'){
        //Returns ext
        for(int i = 0; i<8; i++)
            if(strcmp(string, mimes.array[i].type) == 0){
                return mimes.array[i].ext;
            }
            //if no correct matches
            return mimes.plain.ext; //Default
    }
    //Flag is dictated by programmer and must be correct.
    //If flag is incorrect, just exit this process.
    exit(1);

}
/////////////////////////////////////////////////////////////////////////////

///////////STATCODES////////////////////////////////////////
        typedef enum{//Enum values available across program
            Ok                    = 200,
            Created               = 201,
            No_Content            = 204,
            Bad_Request           = 400,
            Not_Found             = 404,
            Method_Not_Allowed    = 405,
            Internal_Server_Error = 500,
            Not_Implemented       = 501
        }statcoden_t;
//Function
char* statcodesV(statcoden_t statcode){
switch(statcode){
case Ok                   : return "OK"                   ;
case Created              : return "Created"              ;
case No_Content           : return "No Content"           ;
case Bad_Request          : return "Bad Request"          ;
case Not_Found            : return "Not Found"            ;
case Method_Not_Allowed   : return "Method Not Allowed"   ;
case Internal_Server_Error: return "Internal Server Error";
case Not_Implemented      : return "Not Implemented"      ;
}}
//////////////////////////////////////////////////////////

//Request/////////////////////////////////////////////////////
typedef struct{
    char* verb;
    char* noun;
    char* mime;
    char* body;
}req_t;

req_t reqDefiner(){
    req_t req;
    req.verb = "";
    req.noun = "";
    req.mime = "";
    req.body = "";

    return req;
}
///////////////////////////////////////////////////////////////

//Response/////////////////////////////////////////////////////
typedef struct{
    //Line 1
    char* http; statcoden_t statcode; char* statcode_verbose;
    //Line 2
    char* content_type; char* content_ext;
    //Line 3,4... Remember to add preceding newlines.
    char* body;
}resp_t;

///////////////////////////////////////////////////////////////

/*
int main(){

    mimes = mimeDefiner();

    mimes.html.type = "";

}
/*
    mimes = mimeDefiner();
    char* mimet = "text/html";
    char* path = ".html";
    for(int y = 0; y < 8; y++){
        if(mimet == mime.array[y].type && path == mime.array[y].ext)
            printf("%s, %s\n", mime.array[y].type, mime.array[y].ext);
    }

    verbs = verbDefiner();
    printf("%s %s\n", verbs.get, verbs.head);
    for(int k = 0; k < 5; k++){
        printf("%s\n", verbs.v[k]);
    }

    //Declared inside function
    statcode_t* statcodes = statcodeDefiner();
    for(int i = 0; i < 8; i++){
        if(statcodes[i].numeric == Not_Found)
            printf("%d: %s\n", statcodes[i].numeric, statcodes[i].verbose);
    }

}//*/


