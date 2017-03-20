#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mysql.h"

#define STRING_SIZE 50
 
#define DROP_SAMPLE_TABLE "DROP TABLE IF EXISTS test_table"
#define CREATE_SAMPLE_TABLE "CREATE TABLE test_table(col1 INT,\
                                                 col2 VARCHAR(40),\
                                                 col3 SMALLINT,\
                                                 col4 TIMESTAMP)"
#define INSERT_SAMPLE "INSERT INTO test_table(col1,col2,col3) VALUES(?,?,?)"


int main(void)
{
	int ret, i;
	MYSQL *mysql;
	MYSQL *mysql_err;
	
	//初始化
	mysql = mysql_init(NULL);
	if (mysql == NULL) {
		//const char *mysql_error(MYSQL *mysql) 
		printf("mysql_init err: %s\n", mysql_error(mysql));
		return -1;
	}
	printf("mysql_init success! \n");
	
	//练接数据库
	mysql_err = mysql_real_connect(mysql, "127.0.0.1", "root", "123456", "mydb61", 0, NULL, 0);
	if (mysql_err == NULL) { 
		printf("mysql_real_connect err %d:%s\n", mysql_errno(mysql), mysql_error(mysql));
		return -1;
	}
	printf("connect mydb61 ok...\n");
	
//----------------------------------------------------------------------------	
	MYSQL_STMT    *stmt;
	MYSQL_BIND    bind[3];
	my_ulonglong  affected_rows;
	int           param_count;
	short         small_data;
	int           int_data;
	char          str_data[STRING_SIZE];
	unsigned long str_length;
	my_bool       is_null;
	 
	if (mysql_query(mysql, DROP_SAMPLE_TABLE))
	{
	  fprintf(stderr, " DROP TABLE failed\n");
	  fprintf(stderr, " %s\n", mysql_error(mysql));
	  exit(0);
	}
	 
	if (mysql_query(mysql, CREATE_SAMPLE_TABLE))
	{
	  fprintf(stderr, " CREATE TABLE failed\n");
	  fprintf(stderr, " %s\n", mysql_error(mysql));
	  exit(0);
	}
	 
	/* Prepare an INSERT query with 3 parameters */
	/* (the TIMESTAMP column is not named; the server */
	/*  sets it to the current date and time) */
	stmt = mysql_stmt_init(mysql);
	if (!stmt)
	{
	  fprintf(stderr, " mysql_stmt_init(), out of memory\n");
	  exit(0);
	}
	if (mysql_stmt_prepare(stmt, INSERT_SAMPLE, strlen(INSERT_SAMPLE)))
	{
	  fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
	  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	  exit(0);
	}
	fprintf(stdout, " prepare, INSERT successful\n");
	 
	/* Get the parameter count from the statement */
	param_count= mysql_stmt_param_count(stmt);
	fprintf(stdout, " total parameters in INSERT: %d\n", param_count);
	 
	if (param_count != 3) /* validate parameter count */
	{
	  fprintf(stderr, " invalid parameter count returned by MySQL\n");
	  exit(0);
	}
	 
	/* Bind the data for all 3 parameters */
	 
	memset(bind, 0, sizeof(bind));
	 
	/* INTEGER PARAM */
	/* This is a number type, so there is no need to specify buffer_length */
	bind[0].buffer_type= MYSQL_TYPE_LONG;
	bind[0].buffer= (char *)&int_data;
	bind[0].is_null= 0;
	bind[0].length= 0;
	 
	/* STRING PARAM */
	bind[1].buffer_type= MYSQL_TYPE_STRING;
	bind[1].buffer= (char *)str_data;
	bind[1].buffer_length= STRING_SIZE;
	bind[1].is_null= 0;
	bind[1].length= &str_length;
	 
	/* SMALLINT PARAM */
	bind[2].buffer_type= MYSQL_TYPE_SHORT;
	bind[2].buffer= (char *)&small_data;
	bind[2].is_null= &is_null;
	bind[2].length= 0;
	 
	/* Bind the buffers */
	if (mysql_stmt_bind_param(stmt, bind))
	{
	  fprintf(stderr, " mysql_stmt_bind_param() failed\n");
	  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	  exit(0);
	}
	 
	/* Specify the data values for the first row */
	int_data= 177;             /* integer */
	strncpy(str_data, "hahahaha", STRING_SIZE); /* string  */
	small_data=100;
	str_length= strlen(str_data);
	 
	/* INSERT SMALLINT data as NULL */
	is_null= 0;
	 
	/* Execute the INSERT statement - 1*/
	if (mysql_stmt_execute(stmt))
	{
	  fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
	  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	  exit(0);
	}
	 
	/* Get the total number of affected rows */
	affected_rows= mysql_stmt_affected_rows(stmt);
	fprintf(stdout, " total affected rows(insert 1): %lu\n",
	                (unsigned long) affected_rows);
	 
	if (affected_rows != 1) /* validate affected rows */
	{
	  fprintf(stderr, " invalid affected rows by MySQL\n");
	  exit(0);
	}
	 
	/* Specify data values for second row, then re-execute the statement */
	int_data= 27;
	strncpy(str_data, "mysql stmt", STRING_SIZE);
	str_length= strlen(str_data);
	small_data= 250;         /* smallint */
	is_null= 1;               /* reset */
	 
	/* Execute the INSERT statement - 2*/
	if (mysql_stmt_execute(stmt))
	{
	  fprintf(stderr, " mysql_stmt_execute, 2 failed\n");
	  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	  exit(0);
	}
	 
	/* Get the total rows affected */
	affected_rows= mysql_stmt_affected_rows(stmt);
	fprintf(stdout, " total affected rows(insert 2): %lu\n",
	                (unsigned long) affected_rows);
	 
	if (affected_rows != 1) /* validate affected rows */
	{
	  fprintf(stderr, " invalid affected rows by MySQL\n");
	  exit(0);
	}
	
	int_data= 6666;
	strncpy(str_data, "AAA", STRING_SIZE);
	str_length= strlen(str_data);
	small_data= 299;         /* smallint */
	is_null= 0;               /* reset */
	 
	/* Execute the INSERT statement - 2*/
	if (mysql_stmt_execute(stmt))
	{
	  fprintf(stderr, " mysql_stmt_execute, 2 failed\n");
	  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	  exit(0);
	}
	 
	/* Get the total rows affected */
	affected_rows= mysql_stmt_affected_rows(stmt);
	fprintf(stdout, " total affected rows(insert 2): %lu\n",
	                (unsigned long) affected_rows);
	 
	if (affected_rows != 1) /* validate affected rows */
	{
	  fprintf(stderr, " invalid affected rows by MySQL\n");
	  exit(0);
	}
	 
	/* Close the statement */
	if (mysql_stmt_close(stmt))
	{
	  fprintf(stderr, " failed while closing the statement\n");
	  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	  exit(0);
	}

	//关闭
	mysql_close(mysql);
	printf("close ok...\n");
	
	return 0;	
}