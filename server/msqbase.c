#include "msqbase.h"
char* zErrMsg = 0;
int
initsqbase(const char* filname, sqlite3** db)
{ // init database
  int rc;
  rc = sqlite3_open(filname, db);
  if (rc) {
    sqlite3_close(*db);
    return 1;
  }
  return 0;
}

int
sqbasechart(sqlite3** db)
{

  char* sql = " CREATE TABLE illness(\
     ID INTEGER PRIMARY KEY AUTOINCREMENT,\
     name VARCHAR(12),\
     sex VARCHAR(5),\
     age VARCHAR(5),\
     ybid VARCHAR(20) UNIQUE,\
     doctorname  VARCHAR(20),\
     telephone VARCHAR(20),\
     department VARCHAR(20),\
     status VARCHAR(5),\
     createdtime TimeStamp NOT NULL DEFAULT (datetime('now','localtime'))\
     );";
  sqlite3_exec(*db, sql, 0, 0, &zErrMsg);
  char* sql1 = " CREATE TABLE doctor(\
     ID INTEGER PRIMARY KEY AUTOINCREMENT,\
     name VARCHAR(12),\
     usern VARCHAR(12) UNIQUE,\
     sex VARCHAR(5),\
     age VARCHAR(5),\
     telephone VARCHAR(20),\
     department VARCHAR(20),\
     password VARCHAR(20),\
     status VARCHAR(5)\
     );";
  sqlite3_exec(*db, sql1, 0, 0, &zErrMsg);
  char* sql2 = " CREATE TABLE nurse(\
     ID INTEGER PRIMARY KEY AUTOINCREMENT,\
     name VARCHAR(12),\
     usern VARCHAR(12) UNIQUE,\
     sex VARCHAR(5),\
     age VARCHAR(5),\
     telephone VARCHAR(20),\
     department VARCHAR(20),\
     password VARCHAR(20),\
     status VARCHAR(5)\
     );";

  sqlite3_exec(*db, sql2, 0, 0, &zErrMsg);
  return 0;
}

int
loginin(sqlite3*** db,
        const char* tablen,
        const char* username,
        const char* password)
{ // login
  char sql[150] = { 0 };
  char** result;
  sprintf(sql,
          "SELECT * FROM %s WHERE usern=\"%s\" and password=\"%s\";",
          tablen,
          username,
          password);
  printf("%s", zErrMsg);
  int nrow, ncolumn;
  sqlite3_get_table(**db, sql, &result, &nrow, &ncolumn, &zErrMsg);
  if (nrow >= 1) {
    return 1;
  }
  return 0;
}

int
sqbasesearch(sqlite3** db,
             int* nrow,
             int* ncolumn,
             char*** result,
             const char* tablen,
             const char* nwhere)
{ // search by keshi

  char sql[100] = { 0 };
  zErrMsg = NULL;
  sprintf(sql,
          "SELECT * FROM %s where department=\"%s\" And status=\"0\";",
          tablen,
          nwhere);
  sqlite3_get_table(*db, sql, result, nrow, ncolumn, &zErrMsg);
  if (zErrMsg != NULL)
    return 1;
  return 0;
}

int
sqbasesearchbydoctor(sqlite3** db,
                     int* nrow,
                     int* ncolumn,
                     char*** result,
                     const char* tablen,
                     const char* nwhere)
{
  char sql[100] = { 0 };
  zErrMsg = NULL;
  sprintf(sql,
          "SELECT * FROM %s where doctorname=\"%s\" And status=\"1\";",
          tablen,
          nwhere);
  printf("%s\n", sql);
  sqlite3_get_table(*db, sql, result, nrow, ncolumn, &zErrMsg);
  if (zErrMsg != NULL)
    return 1;
  return 0;
}

int
getnursekeshi(sqlite3** db, char* keshi, const char* username)
{ // get nurse keshi
  int nrow, ncolumn;
  zErrMsg = NULL;
  char sql[100] = { 0 };
  char** result;
  sprintf(sql, "SELECT * FROM nurse WHERE usern=\"%s\";", username);
  sqlite3_get_table(*db, sql, &result, &nrow, &ncolumn, &zErrMsg);
  if (zErrMsg != NULL)
    return 1;
  printf("%s", zErrMsg);
  strcat(keshi, result[15]);
  return 0;
}

int
sqbaseinsert(sqlite3** db, const char* tablen, illness person)
{ // insert data in to tablen (used to rec data from register)
  char sql[200] = { 0 };
  zErrMsg = NULL;
  printf("%s", zErrMsg);
  sprintf(
    sql,
    "INSERT INTO %s "
    "VALUES(NULL,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%s);",
    tablen,
    person.name,
    person.sex,
    person.age,
    person.ybid,
    person.doctorname,
    person.telephone,
    person.department,
    person.status,
    "datetime('now','localtime')");
  sqlite3_exec(*db, sql, 0, 0, &zErrMsg);
  if (zErrMsg != NULL)
    return 1;
  return 0;
}

int
selectdoctor(sqlite3** db, const char* id, const char* doctorname)
{ // set doctor to the sqlite
  char sql[200] = { 0 };
  sprintf(sql,
          "Update illness Set doctorname=\"%s\",status=\"1\" Where ID=%s",
          doctorname,
          id);
  sqlite3_exec(*db, sql, 0, 0, &zErrMsg);
  return 0;
}

int
finishdoctor(sqlite3** db, const char* id)
{
  char sql[200] = { 0 };
  sprintf(sql, "Update illness Set status=\"2\" Where ID=%s", id);
  printf("%s\n", sql);
  sqlite3_exec(*db, sql, 0, 0, &zErrMsg);
  return 0;
}

int
getnumber(sqlite3** db, const char* keshi, int* row)
{
  char sql[100] = { 0 };
  char** result;
  int ncolumn;
  sprintf(sql, "select * from illness where department=\"%s\";", keshi);
  // printf("%s\n",sql);
  // printf("%s",zErrMsg);
  sqlite3_get_table(*db, sql, &result, row, &ncolumn, &zErrMsg);
  // printf("%s",zErrMsg);
  return 0;
}

int
getdnumber(sqlite3** db, const char* keshi, int* row)
{
  char sql[100] = { 0 };
  char** result;
  int ncolumn;
  sprintf(sql,
          "select * from illness where department=\"%s\" and status=\"2\";",
          keshi);
  printf("%s\n", sql);
  printf("%s", zErrMsg);
  sqlite3_get_table(*db, sql, &result, row, &ncolumn, &zErrMsg);
  printf("%s", zErrMsg);
  return 0;
}
