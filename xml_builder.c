#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//#include "webSql.c"
//#include "xml_parser.c"

//Must recieve SQL response line as a NULL terminated string 
char* xmlContactFieldsB(char* sqlLine){

    //Field1 finder
    char* idP = sqlLine;
    char* nameP;
    char* nrP;

    //Terminator counter
    int counter  = 0;
    //String iterator
    int iterator = 0;
    while(counter < 2){

        //Field2 finder
        if(counter == 0 && *(sqlLine+iterator) == ','){
            nameP = (sqlLine+iterator+1);
            *(sqlLine+iterator) = '\0';
            counter++;
        }
        //Field3 finder. Field 3 is nullterminated.
        if(counter == 1 && *(sqlLine+iterator) == ','){
            nrP = (sqlLine+iterator+1);
            *(sqlLine+iterator) = '\0';
            counter++;
            break;
        }
        iterator++;
    }//Now we got a string consisting of several nullterminated strings

    char* idTag = "\t\t<id>";
    char* idEnd = "</id>\n";
    char* idTot = malloc(strlen(idTag)+strlen(idEnd)+strlen(idP)+1);
    strcpy(idTot, idTag);
    strcat(idTot, idP);
    strcat(idTot, idEnd);

    char* nameTag = "\t\t<name>";
    char* nameEnd = "</name>\n";
    char* nameTot = malloc(strlen(nameTag)+strlen(nameEnd)+strlen(nameP)+1);
    strcpy(nameTot, nameTag);
    strcat(nameTot, nameP);
    strcat(nameTot, nameEnd);

    char* nrTag = "\t\t<nr>";
    char* nrEnd = "</nr>\n";
    char* nrTot = malloc(strlen(nrTag)+strlen(nrEnd)+strlen(nrP)+1);
    strcpy(nrTot, nrTag);
    strcat(nrTot, nrP);
    strcat(nrTot, nrEnd);

    char* total = malloc(strlen(idTot)+strlen(nameTot)+strlen(nrTot)+1);
    strcpy(total, idTot);
    strcat(total, nameTot);
    strcat(total, nrTot);

    free(idTot);
    free(nameTot);
    free(nrTot);

    return total;
}

char* xmlContactB(int lineCount, char* sqlLines){

    char* allCont[lineCount];
    int totStrlen = 0;

    int lineReader = 0;
    int totOffset  = 0;

    //Each iteration creates a string for one contact
    while(lineReader < lineCount){
        //sqlLines += totOffset;
        int lineOffset  = 0;//Terminate with this one
        char* lineStart = (sqlLines+totOffset);

        //This while reads string, finds first \n char
        //Terminates it with \0
        while(1){
            //Nullterminating newline, then breaks loop
            if(*(lineStart+lineOffset) == '\n'){
                *(lineStart+lineOffset) = '\0';
                lineOffset++;
                break;
            }
            //If if not executed, pointer not pointing to newline
            //Increment lineOffset
            lineOffset++;
        }
        totOffset += lineOffset;

        //Defining 2 constants, and sends the newly parsed line
        //for further paring
        char* contTag       = "\t<contact>\n";
        char* contFields    = xmlContactFieldsB(lineStart);
        char* contEnd       = "\t</contact>\n";
        

        int thisStrlen      = strlen(contTag)+strlen(contFields)+strlen(contEnd);
        totStrlen += thisStrlen;
        char* contTot       =  malloc(thisStrlen+1);

        strcpy(contTot, contTag);
        strcat(contTot, contFields);
        strcat(contTot, contEnd);

        free(contFields);
        allCont[lineReader] = contTot;
        lineReader++;
    }

    //Creating one long string to return
    int looper    = 0;
    char* allContTot = malloc(totStrlen+1);
    strcpy(allContTot, allCont[looper]);
    free(allCont[looper]);
    looper++;
    while(looper < lineCount){
        strcat(allContTot, allCont[looper]);
        free(allCont[looper]);
        looper++;
    }

    return allContTot;
}


char* xmlContactsB(int lineCount, char* sqlLines){

    char* contactsTag = "<contacts>\n";
    char* contents    = xmlContactB(lineCount, sqlLines);
    char* contactsEnd = "</contacts>\n";

    //must malloc complete size of the above
    char* completeXml  = malloc(strlen(contactsTag)+strlen(contents)+strlen(contactsEnd)+1);

    strcpy(completeXml, contactsTag);
    strcat(completeXml, contents);
    strcat(completeXml, contactsEnd);

    free(contents);

    return completeXml;
}

char* xmlBuilder(int lineCount, char* sqlLines){


    char* xmlTag      = "<?xml version=\"1.0\"?>\n";
    if(lineCount == 0){
        return xmlTag;
    }
    //"<?xml-stylesheet type=\"text/xsl\" href=\"/styleXML.xsl\"?>"
    char* xslt        = "";
    char* contents    = xmlContactsB(lineCount, sqlLines);

    char* xml = malloc(strlen(xslt)+strlen(xmlTag)+strlen(contents)+1);

    strcpy(xml, xmlTag);
    strcat(xml, xslt);
    strcat(xml, contents);

    free(sqlLines);

    return xml;
}

/*
int main(){
/*
    char* dbString = "1 gleb 12345\n2 juro 34453\n3 Nissen 667345283234332\n4 Andr 76346\n290 IMadeIt 123456789\n";
    char* string   = malloc(strlen(dbString)+1); //for manipulation purposes
    strcpy(string,dbString);
    string[strlen(dbString)] = '\0';
/

    //char* sql = getIdSQL("7");

    char* sql = getAllSQL();

    char* string = sqlExecutor(sql);

    //printf("%s", string);

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
    
    write(1, xml, strlen(xml));
    printf("\n");

    char* parserOut = xmlParser(xml);

    printf("%s", parserOut);

    return 0;

}*/