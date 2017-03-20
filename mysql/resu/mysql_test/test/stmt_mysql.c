#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <mysql.h>

#define STRING_SIZE 50

//删除一个已经存在的表
#define DROP_SAMPLE_TABLE "DROP TABLE IF EXISTS test_table"		
//创建一个有4列的表test_table		
#define CREATE_SAMPLE_TABLE "CREATE TABLE test_table(col1 INT,\
                                                 col2 VARCHAR(40),\
                                                 col3 SMALLINT,\
                                                 col4 TIMESTAMP)"   //时间戳
//带有(???)的SQL语句                                           
#define INSERT_SAMPLE "INSERT INTO test_table(col1,col2,col3) VALUES(?,?,?)"

int main(int arg, char *args[])
{
	int 			ret = 0, i=0;
	MYSQL 			*mysql;

	MYSQL_RES 		*result;
	MYSQL_ROW		row;
	MYSQL_FIELD 	*fields;
	unsigned int 	num_fields;

	mysql = mysql_init(NULL);
	if (mysql == NULL) {
		ret =  mysql_errno(mysql) ;
		printf("connect error, %s\n", mysql_error(mysql));
		goto END;
	}

	//连接到mysql server
	mysql = mysql_real_connect(mysql, "localhost", "root", "123456", "mydb2", 0, NULL,  0);
	if (mysql == NULL) {
		ret =  mysql_errno(mysql) ;
		printf("connect error, %s\n", mysql_error(mysql));
		goto END;
	}
	
	//////////////////////////////////////////////////////
	MYSQL_STMT    *stmt;
	MYSQL_BIND    bind[3];							//结构体数组
	my_ulonglong  affected_rows;
	int           param_count;
	short         small_data;
	int           int_data;
	char          str_data[STRING_SIZE];
	unsigned long str_length;
	my_bool       is_null;
	 
	if (mysql_query(mysql, DROP_SAMPLE_TABLE)) {		//借助宏完成删除test_table	
	  fprintf(stderr, " DROP TABLE failed\n");
	  fprintf(stderr, " %s\n", mysql_error(mysql));
	  exit(0);
	}
	 
	if (mysql_query(mysql, CREATE_SAMPLE_TABLE)) {	//借助宏完成创建test_table
	  fprintf(stderr, " CREATE TABLE failed\n");
	  fprintf(stderr, " %s\n", mysql_error(mysql));
	  exit(0);
	}
	 
	stmt = mysql_stmt_init(mysql); 					//————初始化预处理环境句柄
	if (!stmt) {
	  fprintf(stderr, " mysql_stmt_init(), out of memory\n");
	  exit(0);
	}
	
	//————向预处理环境句柄添加带(???)的sql语句
	if (mysql_stmt_prepare(stmt, INSERT_SAMPLE, strlen(INSERT_SAMPLE))) {

	  fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
	  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	  exit(0);
	}
	fprintf(stdout, " prepare, INSERT successful\n");

	param_count = mysql_stmt_param_count(stmt);		//辅助性函数，求有多少个‘?’
	fprintf(stdout, " total parameters in INSERT: %d\n", param_count);
	 
	if (param_count != 3) { 						//不等于3出错，辅助性判断
	  fprintf(stderr, " invalid parameter count returned by MySQL\n");
	  exit(0);
	}
	 
	memset(bind, 0, sizeof(bind));
	 
	bind[0].buffer_type= MYSQL_TYPE_LONG;
	bind[0].buffer= (char *)&int_data;			// 【注意 1】
	bind[0].is_null= 0;  
	bind[0].length= 0;	 //非字符串变量
	 
	bind[1].buffer_type= MYSQL_TYPE_STRING;
	bind[1].buffer= (char *)str_data;			// 【注意 2】
	bind[1].buffer_length= STRING_SIZE;
	bind[1].is_null= 0;
	bind[1].length= &str_length;				// 【注意 3】
	 
	bind[2].buffer_type= MYSQL_TYPE_SHORT;
	bind[2].buffer= (char *)&small_data;		// 【注意 4】
	bind[2].is_null= &is_null;			 		// 【注意 5】
	bind[2].length= 0;
	 
	if (mysql_stmt_bind_param(stmt, bind)) { 		//————把赋值以后的数组 添加到预处理环境句柄
	  fprintf(stderr, " mysql_stmt_bind_param() failed\n");
	  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	  exit(0);
	}
	 
	int_data= 10;            							        //对应 【注意 1】
	strncpy(str_data, "MySQL", STRING_SIZE); 					//对应 【注意 2】
	str_length= strlen(str_data);								//对应 【注意 3】 
	is_null= 1;		//1表示列值为NULL， 0表示非NULL		      	//对应 【注意 5】
	 
	if (mysql_stmt_execute(stmt)) { 		//————执行已经完全填好值的SQL语句，表有了第一行数据
	  fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
	  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	  exit(0);
	}
	 
	affected_rows= mysql_stmt_affected_rows(stmt);
	fprintf(stdout, " total affected rows(insert 1): %lu\n",
	                (unsigned long) affected_rows);
	 
	if (affected_rows != 1) { 
	  fprintf(stderr, " invalid affected rows by MySQL\n");
	  exit(0);
	}
	 
	int_data= 1000;																																					//对应 【注意 1】
	strncpy(str_data, "The most popular Open Source database", STRING_SIZE);//对应 【注意 2】
	str_length= strlen(str_data);											//对应 【注意 3】
	small_data= 1000;         												//对应 【注意 4】
	is_null= 0;               												//对应 【注意 5】
	 
	if (mysql_stmt_execute(stmt)) {  		//————再执行重新填值的SQL语句。 表有了第二行数据
	  fprintf(stderr, " mysql_stmt_execute, 2 failed\n");
	  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	  exit(0);
	}
	 
	affected_rows= mysql_stmt_affected_rows(stmt);
	fprintf(stdout, " total affected rows(insert 2):%lu\n", (unsigned long) affected_rows);
	 
	if (affected_rows != 1) {
	  fprintf(stderr, " invalid affected rows by MySQL\n");
	  exit(0);
	}
	 
	if (mysql_stmt_close(stmt))	{
	  fprintf(stderr, " failed while closing the statement\n");
	  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	  exit(0);
	}
		
END:		
	if (mysql != NULL)
		mysql_close(mysql);				//断开与SQL server的连接
	
	return 0;
}
