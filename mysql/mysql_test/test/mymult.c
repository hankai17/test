#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql.h"

void process_result_set(MYSQL *mysql, MYSQL_RES *result)
{
	int num, i;
	
	//获取列数
	num = mysql_field_count(mysql);			
	
	//打印表头
	MYSQL_FIELD *fields = NULL;
	fields = mysql_fetch_fields(result);
	for(i = 0; i < num; i++) {
		printf("%10s\t", fields[i].name);
	}
	printf("\n");

	//解析数据
	MYSQL_ROW row;									//typedef char ** MYSQL_ROW;
	while ((row = mysql_fetch_row(result))) {
		for (i = 0; i < num; i++) {
			printf("%10s\t", row[i]);
		}
		printf("\n");	
	}			
	return ;	
}

int main(void)
{
	int ret, status;
	MYSQL *mysql = NULL;
	MYSQL_RES *result = NULL;
	
	//初始化
	//MYSQL *mysql_init(MYSQL *mysql)
	mysql = mysql_init(NULL);
	if (mysql == NULL) {
		//unsigned int mysql_errno(MYSQL *mysql)
		ret = mysql_errno(mysql);	
		printf("mysql_init error: %d\n", ret);
		return ret;
	}
	printf("mysql_init ok...\n");
	
	//连接数据库
	//MYSQL *mysql_real_connect(MYSQL *mysql, const char *host, const char *user, const char *passwd, const char *db, unsigned int port, const char *unix_socket, unsigned long client_flag) 
	mysql = mysql_real_connect(mysql, "localhost", "root", "123456", "mydb61", 0, NULL, CLIENT_MULTI_STATEMENTS);
	if (mysql == NULL) {
		ret = mysql_errno(mysql);	
		printf("mysql_real_connect error: %d\n", ret);
		return ret;
	}
	printf("connect ok...\n");
	
/////////////////////////////////////////////

/* execute multiple statements */
status = mysql_query(mysql,"DROP TABLE IF EXISTS test_table;\
							CREATE TABLE test_table(id INT);\
							INSERT INTO test_table VALUES(10);\
							UPDATE test_table SET id=20 WHERE id=10;\
							SELECT * FROM test_table");
							//DROP TABLE test_table");
				//1111 2222  3333 4444 5555
				//           ▲ 			
							
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
		} else {
		if (mysql_field_count(mysql) == 0)
			{
				printf("%lld rows affected\n",
				mysql_affected_rows(mysql));
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

	mysql_close(mysql); 
	printf("close ok...\n");
	
	return 0;
}