typedef struct illness
{ // illness struct
  char name[30];
  char sex[10];
  char age[10];
  char telephone[50];
  char department[50];
  char doctorname[20];
  char ybid[20];
  char status[2];
} illness;
typedef struct doctor
{ // doctor struct
  char name[20];
  int sex;
  char age[3];
  char telephone[20];
  char department[20];
  int status;
} doctor;
typedef struct nurse
{ // nurse struct
  char name[20];
  int sex;
  char age[3];
  char telephone[50];
  char department[50];
} nurse;
typedef struct passarg
{
  int i;
  sqlite3** db;
} passarg;
