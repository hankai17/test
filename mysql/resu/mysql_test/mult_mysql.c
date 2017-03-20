#include <stdio.h>
#include <stdlib.h>
#include "mysql.h"

void process_result_set(MYSQL *mysql, MYSQL_RES *result)
{	
	int i, num;
	//获取列数
	num = mysql_field_count(mysql);
	
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
	while ((row = mysql_fetch_row(result))) {
		for (i = 0; i < num; i++) {
			printf("%12s", row[i]);
		}
		printf("\n");
	}
	return;	
}

int main(void)
{
	int ret, i, status;
	MYSQL *mysql;
	MYSQL *mysql_err  = NULL;
	MYSQL_RES *result = NULL;
	
	//初始化 
	mysql = mysql_init(NULL);
	if (mysql == NULL) {
		//const char *mysql_error(MYSQL *mysql) 
		printf("mysql_init err: %s\n", mysql_error(mysql));
		return -1;
	}
	printf("mysql_init success! \n");
	
	//练接数据库 
	//mysql_err = mysql_real_connect(mysql, "127.0.0.1", "root", "123456", "mydb61", 0, NULL, CLIENT_MULTI_STATEMENTS);
	mysql_err = mysql_real_connect(mysql, "10.10.110.51", "root", "123456", "mydb61", 0, NULL, CLIENT_MULTI_STATEMENTS);
	if (mysql_err == NULL) { 
		printf("mysql_real_connect err %d:%s\n", mysql_errno(mysql), mysql_error(mysql));
		return -1;
	}
	printf("connect mydb61 ok...\n");
	
	/////////////////////////////////////////////////////////////////
	/* execute multiple statements */
	status = mysql_query(mysql, "DROP TABLE IF EXISTS test_table;\
								CREATE TABLE test_table(id INT);\
								INSERT INTO test_table VALUES(10);\
								UPDATE test_table SET id=20 WHERE id=10;\
								SELECT * FROM test_table");
								//DROP TABLE test_table"
	if (status)
	{
		printf("Could not execute statement(s)");
		mysql_close(mysql);
		exit(0);
	}
	/* process each statement result */
	do {
		/* did current statement return data? */
		result = mysql_store_result(mysql);
		if (result)
		{
			/* yes; process rows and free the result set */
			process_result_set(mysql, result);
			mysql_free_result(result);
		} else /* no result set or error */
		{
			if (mysql_field_count(mysql) == 0)
			{
				printf("%lld rows affected\n", mysql_affected_rows(mysql));
			}
			else /* some error occurred */
			{
				printf("Could not retrieve result set\n");
				break;
			}
		}
		
		/* more results? -1 = no, >0 = error, 0 = yes (keep looping) */
		if ((status = mysql_next_result(mysql)) > 0)
			printf("Could not execute statement\n");
			
	} while (status == 0);
	
	//关闭
	mysql_close(mysql);
	printf("close ok...\n");
	
	return 0;	
}
