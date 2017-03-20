#include <stdio.h>
#include "mysql.h"
#include<string.h>
#include<assert.h>

int enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput,int outSize)  
{  
    assert(pOutput != NULL);  
    assert(outSize >= 6);  

    if ( unic <= 0x0000007F )  
    {  
        // * U-00000000 - U-0000007F:  0xxxxxxx  
        *pOutput     = (unic & 0x7F);  
        return 1;  
    }  
    else if ( unic >= 0x00000080 && unic <= 0x000007FF )  
    {  
        // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx  
        *(pOutput+1) = (unic & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 6) & 0x1F) | 0xC0;  
        return 2;  
    }  
    else if ( unic >= 0x00000800 && unic <= 0x0000FFFF )  
    {  
        // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx  
        *(pOutput+2) = (unic & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >>  6) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 12) & 0x0F) | 0xE0;  
        return 3;  
    }  
    else if ( unic >= 0x00010000 && unic <= 0x001FFFFF )  
    {  
        // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
        *(pOutput+3) = (unic & 0x3F) | 0x80;  
        *(pOutput+2) = ((unic >>  6) & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >> 12) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 18) & 0x07) | 0xF0;  
        return 4;  
    }  
    else if ( unic >= 0x00200000 && unic <= 0x03FFFFFF )  
    {  
        // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
        *(pOutput+4) = (unic & 0x3F) | 0x80;  
        *(pOutput+3) = ((unic >>  6) & 0x3F) | 0x80;  
        *(pOutput+2) = ((unic >> 12) & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >> 18) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 24) & 0x03) | 0xF8;  
        return 5;  
    }  
    else if ( unic >= 0x04000000 && unic <= 0x7FFFFFFF )  
    {  
        // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
        *(pOutput+5) = (unic & 0x3F) | 0x80;  
        *(pOutput+4) = ((unic >>  6) & 0x3F) | 0x80;  
        *(pOutput+3) = ((unic >> 12) & 0x3F) | 0x80;  
        *(pOutput+2) = ((unic >> 18) & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >> 24) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 30) & 0x01) | 0xFC;  
        return 6;  
    }  

    return 0;  
}  


int main(void)
{
    int ret, i;
    MYSQL *mysql;
    MYSQL *mysql_err;

    mysql = mysql_init(NULL);
    if (mysql == NULL) {
        printf("mysql_init err: %s\n", mysql_error(mysql));
        return -1;
    }
    printf("mysql_init success! \n");

    mysql_err = mysql_real_connect(mysql, "10.10.110.51", "root", "123456", "mydb_utf1", 0, NULL, 0);
    if (mysql_err == NULL) { 
        printf("mysql_real_connect err %d:%s\n", mysql_errno(mysql), mysql_error(mysql));
        return -1;
    }
    printf("connect mydb61 ok...\n");

    //char *psql = "select * from ip";
    unsigned char buf[32] = {0};
    enc_unicode_to_utf8_one(0x7f8e, buf,32);  
    char *psql = "insert into ip values(1017,'1.1.1.1','中文');";

    ret = mysql_query(mysql, psql);
    if (ret != 0) {
        ret = mysql_errno(mysql);
        printf("mysql_query err:%d\n", ret);
        return -1;	
    }

    MYSQL_RES *result;
    //获取结果集
    //MYSQL_RES *mysql_store_result(MYSQL *mysql) 
    result = mysql_store_result(mysql);
    if (result == NULL) {
        printf("mysql_store_result err %d:%s\n", mysql_errno(mysql), mysql_error(mysql));
        return -1;		
    }

    //获取列数
    //unsigned int mysql_field_count(MYSQL *mysql) 
    int num = mysql_field_count(mysql);

    //打印表头
    //MYSQL_FIELD *mysql_fetch_fields(MYSQL_RES *result) 
    MYSQL_FIELD *fields;
    fields = mysql_fetch_fields(result);

    for(i = 0; i < num; i++) {
        printf("%12s",fields[i].name);
    }
    printf("\n");

    MYSQL_ROW row;				//typedef char** MYSQL_ROW;
    //循环 fetch数据
    //MYSQL_ROW mysql_fetch_row(MYSQL_RES *result) 
    while ((row = mysql_fetch_row(result))) {
        for (i = 0; i < num; i++) {
            printf("%12s", row[i]);
        }
        printf("\n");
    }

    mysql_close(mysql);
    printf("close ok...\n");

    return 0;	
}
