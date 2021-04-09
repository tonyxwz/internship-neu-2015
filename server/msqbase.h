#ifndef M_SQBASE
#define M_SQBASE
#include "sqlite3.h"
#include <stdio.h>
#include <string.h>
#include "mystruct.h"
int
initsqbase(const char* filname, sqlite3** db);
int
sqbasechart(sqlite3** db);
int
sqbasesearch(sqlite3** db,
             int* nrow,
             int* ncolumn,
             char*** result,
             const char* tablen,
             const char* nwhere);
int
sqbaseinsert(sqlite3** db, const char* tablen, illness person);
int
loginin(sqlite3*** db,
        const char* tablen,
        const char* username,
        const char* password);
int
getnursekeshi(sqlite3** db, char* keshi, const char* username);
int
selectdoctor(sqlite3** db, const char* id, const char* doctorname);
int
finishdoctor(sqlite3** db, const char* id);
int
sqbasesearchbydoctor(sqlite3** db,
                     int* nrow,
                     int* ncolumn,
                     char*** result,
                     const char* tablen,
                     const char* nwhere);
int
getnumber(sqlite3** db, const char* keshi, int* row);
int
getdnumber(sqlite3** db, const char* keshi, int* row);
#endif
