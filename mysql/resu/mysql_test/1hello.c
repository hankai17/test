#include <stdio.h>
#include "mysql.h"

int main(void)
{
	int ret, i;
	MYSQL *mysql;
	MYSQL *mysql_err;
	
	//初始化
	//MYSQL *mysql_init(MYSQL *mysql) 
	mysql = mysql_init(NULL);
	if (mysql == NULL) {
		//const char *mysql_error(MYSQL *mysql) 
		printf("mysql_init err: %s\n", mysql_error(mysql));
		return -1;
	}
	printf("mysql_init success! \n");
	
	//练接数据库
	//MYSQL *mysql_real_connect(MYSQL *mysql, const char *host, const char *user, const char *passwd, const char *db, unsigned int port, const char *unix_socket, unsigned long client_flag) 
	//mysql_err = mysql_real_connect(mysql, "127.0.0.1", "root", "123456", "mydb61", 0, NULL, 0);
	mysql_err = mysql_real_connect(mysql, "10.10.110.51", "root", "123456", "mydb61", 0, NULL, 0);
	if (mysql_err == NULL) { 
		printf("mysql_real_connect err %d:%s\n", mysql_errno(mysql), mysql_error(mysql));
		return -1;
	}
	printf("connect mydb61 ok...\n");
	
	char *psql = "select * from emp";
	//char *psql = "insert into dept values(90, '90dname', '90loc')";
	
	//执行sql语句
	//int mysql_query(MYSQL *mysql, const char *query) 
	ret = mysql_query(mysql, psql);
	if (ret != 0) {
		ret = mysql_errno(mysql);
		printf("mysql_real_connect err:%d\n", ret);
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
	
	//关闭
	//void mysql_close(MYSQL *mysql) 
	mysql_close(mysql);
	printf("close ok...\n");
	
	return 0;	
}
