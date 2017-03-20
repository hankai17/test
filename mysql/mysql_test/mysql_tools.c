#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "mysql.h"

int main(void)
{
	int ret, i, num;
	MYSQL *mysql;
	MYSQL *mysql_err;
	
	MYSQL_RES *result;
	MYSQL_FIELD *fields;
	MYSQL_ROW row;				//typedef char** MYSQL_ROW;
	
	char sqlbuf[1024];
	
	//初始化
	mysql = mysql_init(NULL);
	if (mysql == NULL) { 
		printf("mysql_init err: %s\n", mysql_error(mysql));
		return -1;
	}
	printf("mysql_init success! \n");
	
	//练接数据库 
	//mysql_err = mysql_real_connect(mysql, "127.0.0.1", "root", "123456", "mydb2", 0, NULL, 0);
	mysql_err = mysql_real_connect(mysql, "10.10.110.51", "root", "123456", "mydb2", 0, NULL, 0);
	if (mysql_err == NULL) { 
		printf("mysql_real_connect err %d:%s\n", mysql_errno(mysql), mysql_error(mysql));
		return -1;
	}
	printf("connect mydb61 ok...\n");
	
	mysql_query(mysql, "set names utf8");
	
	while (1) {
		bzero(sqlbuf,sizeof(sqlbuf));
		printf("YourSQL> ");
		fgets(sqlbuf, sizeof(sqlbuf), stdin);  //insert delete update select
		
		if (strncmp(sqlbuf, "exit", 4) == 0 || strncmp(sqlbuf, "quit", 4) == 0) {
			break;	
		}
		
		//执行sql语句
		ret = mysql_query(mysql, sqlbuf);
		if (ret != 0) {
			ret = mysql_errno(mysql);
			printf("mysql_real_connect err:%d\n", ret);
			return -1;	
		}
		if (strncmp(sqlbuf, "select", 6) == 0 || strncmp(sqlbuf, "SELECT", 6) == 0) {
			
			//获取结果集
			result = mysql_store_result(mysql);
			if (result == NULL) {
				printf("mysql_store_result err %d:%s\n", mysql_errno(mysql), mysql_error(mysql));
				return -1;		
			}
			
			//获取列数
			num = mysql_field_count(mysql);
			
			//打印表头
			fields = mysql_fetch_fields(result);
			for(i = 0; i < num; i++) {
			   printf("%12s",fields[i].name);
			}
			printf("\n");
			
			//循环 fetch数据
			while ((row = mysql_fetch_row(result))) {
				for (i = 0; i < num; i++) {
					printf("%12s", row[i]);
				}
				printf("\n");
			}			
					
		} else {
			printf("Query OK, %d row affected\n", mysql_affected_rows(mysql));	
			continue;
		}	
	}
	
	mysql_close(mysql);
	printf("close ok...\n");
	
	return 0;	
}
