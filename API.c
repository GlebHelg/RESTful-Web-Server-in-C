#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "xml_builder.c"
#include "xml_parser.c"
#include "webSql.c"
//#include "header.c"

/*
void responsePrinter(resp_t resp){

    dprintf(1,"%s %d %s\n%s %s\n\n%s",\
        resp.http, resp.statcode, resp.statcode_verbose,\
        CTI, resp.content_type,\
        resp.body);

    return;
}*/

//For multiline sql output, in general only used by GET
char* sqlToXML(char* sql){

    char* string = sqlExecutor(sql);

    //line counter
    int lines = 0;
    int looper = 0;
    while(string[looper] != '\0'){
        if(string[looper] == '\n'){
            lines++;
        }
        looper++;
    }
    char* xml = xmlBuilder(lines, string);
    return xml;
}

void aput(req_t* req){    
    //Parsed request
    //Body is XML
    char* dbValues = xmlParser((*req).body);

    if(dbValues == NULL){
        return;
    }

    //SQL format looper
    int offset = 0;
    while(dbValues[offset] != '\0'){
        char* line[3];
        //0 = id, 1 = name, 2 = nr

        line[0] = dbValues+offset;
        int field = 1;
        int looper = 0;
        while(dbValues[offset + looper] != '\n'){
            if(dbValues[offset + looper] == ','){
                dbValues[offset + looper] = '\0';
                line[field] = dbValues+offset+looper +1;
                field++;
            }
            looper++;
        }//We have now reached the first '\n'
        dbValues[offset+looper] = '\0';
        printf("%s,%s,%s\n", line[0],line[1],line[2]);
        char* sql = putIdSQL(line[0], line[1], line[2]);
        sqlExecutor(sql);
        looper++;
        offset += looper;
    }
    return;
}

void apost(req_t* req){
    //Parsed request
    //Body is XML
    char* dbValues = xmlParser((*req).body);
    if(dbValues == NULL){
        return;
    }

    //SQL format looper
    int offset = 0;
    while(dbValues[offset] != '\0'){
        char* line[3];
        //0 = id, 1 = name, 2 = nr

        line[0] = dbValues+offset;
        int field = 1;
        int looper = 0;
        while(dbValues[offset + looper] != '\n'){
            if(dbValues[offset + looper] == ','){
                dbValues[offset + looper] = '\0';
                line[field] = dbValues+offset+looper +1;
                field++;
            }
            looper++;
        }//We have now reached the first '\n'
        dbValues[offset+looper] = '\0';
        char* sql = postIdSQL(line[0], line[1], line[2]);
        sqlExecutor(sql);
        looper++;
        offset += looper;
    }
    return;
}

//Expecting verb DELETE 
void adelete(req_t* req){    
    //No response from this one so far
    char* id = (*req).noun + 5;// /api/ = 5 chars
    char* sql;
    if(strcmp(id, "*") == 0){
        sql = deleteAllSQL(id);
    }else{
        sql = deleteIdSQL(id);
    }
    char* x  = sqlExecutor(sql);
    return;
}

void aget(req_t* req){
    resp_t resp;

    dprintf(2, "aget(): %s\n", (*req).noun);

    resp.http             = HTTP                    ;
    resp.statcode         = Ok                      ;
    resp.statcode_verbose = statcodesV(Ok)          ;
    resp.content_type     = mimes.xml.type          ;

    //Body must be XML
    //Need to extract 
    if(strcmp((*req).noun, "/api") == 0){
        resp.content_type = "text/plain";
        resp.body         = "Specify ID\n";
    }else if(strcmp((*req).noun, "/api/") == 0){
        char* sql  = getAllSQL();
        resp.body  = sqlToXML(sql);
    }else if(strncmp((*req).noun, "/api/",5) == 0){
        //must parse id
        char* id  = (*req).noun+5;// /api/ = 5 chars
        dprintf(2, "aget else: %s\n",(*req).noun+5);
        char* sql = getIdSQL(id);
        resp.body = sqlToXML(sql);
    }
    
    //Next one responsible for file printing 
    responsePrinter(resp);

    return;
}

//Control-function for the API
int Api(req_t* req){

        dprintf(2, "Api(): %s\n", (*req).noun);

        if(strcmp((*req).verb, verbs.get) == 0){
            //GET
            aget(req);
            return 0;
        }else if(strcmp((*req).verb, verbs.put) == 0){
            //PUT
            aput(req);
            return 0;
        }else if(strcmp((*req).verb, verbs.post) == 0){
            //POST
            apost(req);
            return 0;
        }else if(strcmp((*req).verb, verbs.delete) == 0){
            //DELETE
            adelete(req);
            return 0;
        }else{
            //Method_Not_Allowed
            dprintf(1, "%d %s\n",Method_Not_Allowed, statcodesV(Method_Not_Allowed));
            return 1;
        }
}

/*
int main(){

    setgid(1000);
    setuid(1000);

    mimes = mimeDefiner();
    verbs = verbDefiner();

req_t req;
req.verb = "GET";
req.noun = "/api/";
req.mime = "";
char* xml = "<contacts><contact><id>22</id><name>MrApe</name><nr>1234</nr></contact></contacts>";
req.body = malloc(strlen(xml));
strcpy(req.body, xml);
///
req.body = "";
    int r = Api(&req);

    printf("return value from Api(): %d\n", r);

    return 0;
}*/