#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


char* xmlParser(char* xmlString){

    //Visual representation of XML structure and all important varibles 

    char* idTag   = "<id>"  ; char* idEnd   = "</id>";
    char* nameTag = "<name>"; char* nameEnd = "</name>";
    char* nrTag   = "<nr>"  ; char* nrEnd   = "</nr>";


    int looper = 0;
    int counter = 0;
    //SQL line counter
    while(*(xmlString+looper) != '\0'){
        if(strncmp(idTag, (xmlString+looper), 4) == 0){
            counter++;
        }
        looper++;
    }
    looper = 0;

    if(counter == 0){
        return NULL;
    }

    //Now we know how much to declare 
    char*   ids[counter]; char*   idsE[counter];
    char* names[counter]; char* namesE[counter];
    char*   nrs[counter]; char*   nrsE[counter];

    //Finding start and end of ids
    counter = 0;
    while(*(xmlString+looper) != '\0'){
        if(strncmp(idTag, (xmlString+looper), 4) == 0){
            ids[counter] = (xmlString+looper+4);
            int looperS = 0;
            //End finder
            while(strncmp(idEnd, (xmlString+looper+looperS), 5)){ looperS++; }
            idsE[counter] = (xmlString+looper+looperS);
            counter++;
        }
        looper++;
    }
    looper = 0;

    //Finding start and end of names
    counter = 0;
    while(*(xmlString+looper) != '\0'){
        if(strncmp(nameTag, (xmlString+looper), 6) == 0){
            names[counter] = (xmlString+looper+6);
            int looperS = 0;
            //End finder
            while(strncmp(nameEnd, (xmlString+looper+looperS), 6)){ looperS++; }
            namesE[counter] = (xmlString+looper+looperS);
            counter++;
        }
        looper++;
    }
    looper = 0;


    //Finding start and end of nrs
    counter = 0;
    while(*(xmlString+looper) != '\0'){
        if(strncmp(nrTag, (xmlString+looper), 4) == 0){
            nrs[counter] = (xmlString+looper+4);
            int looperS = 0;
            //End finder
            while(strncmp(nrEnd, (xmlString+looper+looperS), 4)){ looperS++; }
            nrsE[counter] = (xmlString+looper+looperS);
            counter++;
        }
        looper++;
    }

    char* parsedLines[counter];
    char* NL = "\n";
    char* comma = ",";
    looper = 0;
    while(looper < counter){
           *idsE[looper] = '\0';
         *namesE[looper] = '\0';
           *nrsE[looper] = '\0';

        parsedLines[looper] = malloc(strlen(ids[looper])+strlen(names[looper])+strlen(nrs[looper])+2+2);

        strcpy(parsedLines[looper], ids[looper]  );
        strcat(parsedLines[looper], comma);
        strcat(parsedLines[looper], names[looper]);
        strcat(parsedLines[looper], comma);
        strcat(parsedLines[looper], nrs[looper]  );
        strcat(parsedLines[looper], NL);

        looper++;
    }
    looper = 0;

    int totLength = 0;
    while(looper < counter){
        totLength += strlen(parsedLines[looper]);
        looper++;
    }
    looper = 0;

    char* totReturn = malloc(totLength);
    strcpy(totReturn, parsedLines[0]);
    free(parsedLines[0]);
    looper++;
    while(looper < counter){
        strcat(totReturn, parsedLines[looper]);
        free(parsedLines[looper]);
        looper++;
    }

    //Remember to free in the calling function
    return totReturn;

}

/*
int main(){

char* XML = "\
<contacts>\
\
<contact><id>1</id><name>gleb</name><nr>12345</nr></contact>\
\
<contact><id>2</id><name>juro</name><nr>34453</nr></contact>\
\
<contact><id>3</id><name>Andr</name><nr>76346</nr></contact>\
\
<contact><id>290</id><name>IMadeIt</name><nr>123456789</nr></contact>\
\
</contacts>";

    char* string   = malloc(strlen(XML)+1); //for manipulation purposes
    strcpy(string,XML);
    string[strlen(XML)] = '\0';
    

    write(1, string, strlen(string));
    puts("\n");
    
    char* dbValues = xmlParser(string);
    printf("%s\n", dbValues);

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
                line[field] = dbValues+offset + looper + 1;
                field++;
            }
            looper++;
        }//We have now reached the first '\n'
        dbValues[offset+looper] = '\0';
        printf("%s:%s:%s\n",line[0], line[1], line[2]);
        looper++;
        offset += looper;
    }

    return 0;

}//*/