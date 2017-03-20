#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql.h"
#include "mysql_time.h"

#define DROP_SAMPLE_TABLE "DROP TABLE IF EXISTS test_table2"
#define CREATE_SAMPLE_TABLE "CREATE TABLE test_table2(date_field date,\
                                                 time_field TIME,\
                                                 timestamp_field timestamp)"
int main(void)
{
	int 		ret = 0;
	MYSQL 		*mysql;
	
	mysql = mysql_init(NULL);
	mysql = mysql_real_connect(mysql, "localhost", "root", "123456", "mydb61", 0, NULL, CLIENT_MULTI_STATEMENTS );
	if (mysql == NULL) {
		ret = mysql_errno(mysql);
		printf("%s", mysql_error(mysql));
		printf("func mysql_real_connect() err :%d\n", ret);
		return ret;
	} 
	printf(" mysql_real_connect ok......\n");
	
	MYSQL_TIME  ts;
	MYSQL_BIND  bind[3];
	MYSQL_STMT  *stmt;
	
	if (mysql_query(mysql, DROP_SAMPLE_TABLE)) {	
	  fprintf(stderr, " DROP TABLE failed\n");
	  fprintf(stderr, " %s\n", mysql_error(mysql));
	  exit(0);
	}
	 
	if (mysql_query(mysql, CREATE_SAMPLE_TABLE)) {	
	  fprintf(stderr, " CREATE TABLE failed\n");
	  fprintf(stderr, " %s\n", mysql_error(mysql));
	  exit(0);
	}
	printf("-------create test_table2!!!---------\n");
	
	char query[1024] = "INSERT INTO test_table2(date_field, time_field, timestamp_field) VALUES(?,?,?)";
	
	stmt = mysql_stmt_init(mysql);
	if (!stmt)
	{
		fprintf(stderr, " mysql_stmt_init(), out of memory\n");
		exit(0);
	}
	if (mysql_stmt_prepare(stmt, query, strlen(query))) //向环境句柄中添加sql语言  带有占位符
	{
		fprintf(stderr, "mysql_stmt_prepare(), INSERT failed \n");
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
		exit(0);
	}
	
	/* set up input buffers for all 3 parameters */
	
	bind[0].buffer_type= MYSQL_TYPE_DATE;  				//设置绑定变量属性
	bind[0].buffer= (char *)&ts;
	bind[0].is_null= 0;
	bind[0].length= 0;
	
	bind[2]= bind[1]= bind[0];
	//...
	
	mysql_stmt_bind_param(stmt, bind);					//绑定变量
	
	/* supply the data to be sent in the ts structure */
	ts.year = 2012;	
	ts.month = 12;
	ts.day = 30;
	
	ts.hour = 0;
	ts.minute = 0;
	ts.second = 0;
	
	mysql_stmt_execute(stmt);							//执行
	
	// Close the statement //
	if (mysql_stmt_close(stmt))
	{
	  fprintf(stderr, " failed while closing the statement\n");
	  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	  exit(0);
	}
  
	mysql_close(mysql);
	
	return 0;
}

