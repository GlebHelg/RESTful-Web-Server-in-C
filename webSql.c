#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>
//#include "sqlite3.c"

int callback(void *data, int argc, char **argv, char **column){
   int i;
   char *str = (char *) malloc(512);
   strcpy(str, "");

   for(i=0; i<argc; i++){
     if(i == argc-1){
      sprintf(&str[strlen(str)], "%s\n", argv[i]);
      break;
     }
      sprintf(&str[strlen(str)], "%s,", argv[i]);
   } 

   strcat(data, str); 
   return 0;
}

//selects all
char* getAllSQL(){
    char* all = "SELECT * FROM post;";

    return all;
}

char* getIdSQL(char* id){
    char* empty = "SELECT * FROM post WHERE id = ";
    char* complete = malloc(strlen(empty)+strlen(id)+1);
    strcpy(complete, empty);
    strcat(complete, id);

    //Remember to free in caller function
    return complete;
}


//Put does an update
char* putIdSQL(char* id, char* name, char* nr){
    char* empty = \
    "UPDATE post SET name='', nr='' WHERE id=";
    char* complete = malloc(strlen(empty)+strlen(id)+strlen(name)+strlen(nr));
    sprintf(complete, "UPDATE post SET name='%s', nr='%s' WHERE id=%s", name, nr, id);
    //Remember to free in caller function
    return complete;
}

//Post inserts a totally new VALUE
char* postIdSQL(char* id, char* name, char* nr){
    char* empty = "INSERT INTO post (id, name, nr) VALUES (,'',)";

    char* complete = malloc(strlen(empty) +strlen(id) +strlen(name) +strlen(nr) +1);

    sprintf(complete, "INSERT INTO post (id, name, nr) VALUES (%s,'%s',%s)", id, name, nr);

    //Remember to free in caller function
    return complete;
}

//Delete id
char* deleteIdSQL(char* id){
    char* delete = "DELETE FROM post WHERE id = ";
    char* complete = malloc(28 +sizeof(id) +1);

    strcpy(complete, delete);
    strcat(complete, id);

    //Remember to free in caller function
    return complete;
}

//Delete all
char* deleteAllSQL(char* id){

    //Remember to free in caller function
    return "DELETE * FROM post";
}

void dropTable(sqlite3* db, char* data){
   char *zErrMsg = 0;
   int rc;
   char *sql;
    sql = "DROP TABLE post;";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, data, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else
        fprintf(stdout, "Operation successfull\n");

    printf("%s\n\n", data);
}

void truncateTable(sqlite3* db, char* data){
   char *zErrMsg = 0;
   int rc;
   char *sql;
    sql = "DELETE FROM post;";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, data, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else
        fprintf(stdout, "Operation successfull\n");

    printf("%s\n\n", data);
}

void vacuumDB(sqlite3* db, char* data){
    char *zErrMsg = 0;
    int rc;
    char *sql;
    sql = "VACUUM;";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, data, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else
        fprintf(stdout, "Operation successfull\n");

    printf("%s\n\n", data);
}

void fillInitial(sqlite3* db, char* data){
   char *zErrMsg = 0;
   int rc;
   char *sql;

   /* sql statements */
   sql = "INSERT INTO post ( id, name, nr )"         \
         "VALUES ( '1', 'OleDoffen1', '92345678' );"  \
         "INSERT INTO post ( id, name, nr )"         \
         "VALUES ( '2', 'OleDoffen2', '92345678' );"  \
         "INSERT INTO post ( id, name, nr )"         \
         "VALUES ( '3', 'OleDoffen3', '92345678' );"  \
         "INSERT INTO post ( id, name, nr )"         \
         "VALUES ( '4', 'OleDoffen4', '92345678' );"  \
         "INSERT INTO post ( id, name, nr )"         \
         "VALUES ( '5', 'OleDoffen5', '92345678' );";
   
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, data, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else
        fprintf(stdout, "Operation successfull\n");


    printf("%s\n\n", data);
}

void createInitial(sqlite3* db, char* data){
    char *zErrMsg = 0;
    int rc;
    char *sql;
    sql = "CREATE TABLE post (id SMALLINT NOT NULL,name VARCHAR(30),nr SMALLINT NOT NULL,PRIMARY KEY(id ASC));";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, data, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else
        fprintf(stdout, "Operation successfull\n");

    printf("%s\n\n", data);
}

void errorLogCallback(void *pArg, int iErrCode, const char *zMsg){
  fprintf(stderr, "(%d) %s\n", iErrCode, zMsg);
}

char* sqlExecutor(char* sql){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char* data = (char *) calloc(1, 2048);

    char* pData = NULL;
    sqlite3_config(SQLITE_CONFIG_LOG, errorLogCallback, pData);

    // Open database
    rc = sqlite3_open("/my.db", &db);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "Can't open database: %s\nExtended: %d\n", sqlite3_errmsg(db), sqlite3_extended_errcode(db));
        return(0);
    }

    //Execute SQL statement 
    rc = sqlite3_exec(db, sql, callback, data, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        exit(0);
    }

    sqlite3_close(db);

    return data;

}

/*
int main(int argc, char* argv[])
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;
    char* data = (char *) calloc(1, 2048);
   
    // Open database
   rc = sqlite3_open("/www/root/my.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   }else
      fprintf(stderr, "Opened database successfully\n");

    printf("Truncate:\n");
    truncateTable(db, data);
    free(data);

    printf("Vacuum:\n");
    data = (char *) calloc(1, 2048);
    vacuumDB(db, data);
    free(data);
//*
    printf("DropTable:\n");
    data = (char *) calloc(1, 2048);
    dropTable(db, data);
    free(data);

    printf("CreateInitial:\n");
    data = (char *) calloc(1, 2048);
    createInitial(db, data);
    free(data);
//*
    printf("Fill initial:\n");
    data = (char *) calloc(1, 2048);
    fillInitial(db, data);
    free(data);
//
   //SQL tester
   int q = 0;
   while(q < 9){
    data = calloc(1, 2048);
    printf("Iteration: %d\n", q);
        switch(q){
                case 0: sql = getAllSQL();
                break;

                case 1:
                     printf("Get id=3 SQL\n");
                     sql = getIdSQL("3");
                break; 

                case 2: sql = getAllSQL();
                break; 

                case 3:
                    printf("Update id SQL (id, name, nr)\n"); 
                    sql = putIdSQL("5", "Kasper", "34273468");
                break;

                case 4: sql = getAllSQL();
                break; 

                case 5: 
                    printf("INSERT id SQL (id, name, nr)\n");
                    sql = postIdSQL("7", "Gleb", "42424242");
                break; 

                case 6: sql = getAllSQL();
                break;

                case 7:
                    printf("DELETE id SQL (2)\n"); 
                    sql = deleteIdSQL("2");
                break;

                case 8: sql = getAllSQL();
                break;
        }

        //Execute SQL statement 
        rc = sqlite3_exec(db, sql, callback, data, &zErrMsg);
        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }else
            fprintf(stdout, "Operation successfull\n");

        printf("%s\n\n", data);
        fflush(stdout);
        free(data);
        q++;
   }

   sqlite3_close(db);

   return 0;
}*/
