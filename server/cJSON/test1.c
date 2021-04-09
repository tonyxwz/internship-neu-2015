#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
char* doit()
{
	cJSON *root,*fmt,*img,*thm,*fld;char *out;int i; 
        root=cJSON_CreateObject();
        cJSON_AddItemToObject(root, "name", cJSON_CreateString("Jack (\"Bee\") Nimble"));
        cJSON_AddItemToObject(root, "format", fmt=cJSON_CreateObject());
        cJSON_AddStringToObject(fmt,"type",             "rect");
        cJSON_AddNumberToObject(fmt,"width",            1920);
        cJSON_AddNumberToObject(fmt,"height",           1080);
        cJSON_AddFalseToObject (fmt,"interlace");
        cJSON_AddNumberToObject(fmt,"frame rate",       24);
        out=cJSON_Print(root);  cJSON_Delete(root);     printf("%s\n",out);     free(out); 
	return out;
}

int main (int argc, const char * argv[])
{
    // 创建JSON Object
   doit();
    cJSON *root = cJSON_CreateObject();
    // 加入节点（键值对），节点名称为value，节点值为123.4
    cJSON_AddNumberToObject(root,"value",123.4);
    cJSON_AddStringToObject(root,"name","若干说明");
    // 打印JSON数据包
    char *out = cJSON_Print(root);
    printf("%s\n",out);
    // 释放内存
    cJSON_Delete(root);
    free(out);
    return 0;
}
