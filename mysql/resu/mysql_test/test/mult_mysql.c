#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

void process_result_set(MYSQL *mysql, MYSQL_RES *result)
{
	int num, i;
	//获取列数
	//unsigned int mysql_field_count(MYSQL *mysql) 
	num = mysql_field_count(mysql);
	
	//获取表头
	//MYSQL_FIELD *mysql_fetch_fields(MYSQL_RES *result) 
	MYSQL_FIELD *fields;	
	fields = mysql_fetch_fields(result);
	for (i = 0; i < num; i++) {
	   printf("%8s\t", fields[i].name);
	}
	printf("\n");

	//读取结果集
	//MYSQL_ROW mysql_fetch_row(MYSQL_RES *result) 
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result))) {
		for (i = 0; i < num; i++) {
			printf("%8s\t", row[i]);
		}
		printf("\n");
	}	
}

int main(void)
{
	int ret, num = 0, i = 0, status = 0;
	MYSQL *mysql = NULL;
	MYSQL_RES *result = NULL;
	
	//初始化
	mysql = mysql_init(NULL);
	if (mysql == NULL) {
		ret = mysql_errno(mysql);
		printf("mysql_init error: %d", ret);
		return ret;	
	}
	printf("init ok...\n");
	
	//建立连接
	mysql = mysql_real_connect(mysql, "localhost", "root", "123456", "mydb61", 0, NULL, CLIENT_MULTI_STATEMENTS);
	if (mysql == NULL) {
		ret = mysql_errno(mysql);
		printf("mysql_real_connect error: %d", ret);
		return ret;		
	}
	printf("connect ok...\n");
///////////////////////////////////////////////////////////////

	/* execute multiple statements */
	status = mysql_query(mysql,"DROP TABLE IF EXISTS test_table;\
								CREATE TABLE test_table(id INT);\
								INSERT INTO test_table VALUES(10);\
								UPDATE test_table SET id=20 WHERE id=10;\
								SELECT * FROM test_table");
								//DROP TABLE test_table");
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
		} else {/* no result set or error */
		
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
 
	//关闭连接
	//void mysql_close(MYSQL *mysql) 
	mysql_close(mysql);
	printf("close ok...\n");
	
	return 0;	
}