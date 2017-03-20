#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <mysql.h>

#define STRING_SIZE 50

//ɾ��һ���Ѿ����ڵı�
#define DROP_SAMPLE_TABLE "DROP TABLE IF EXISTS test_table"		
//����һ����4�еı�test_table		
#define CREATE_SAMPLE_TABLE "CREATE TABLE test_table(col1 INT,\
                                                 col2 VARCHAR(40),\
                                                 col3 SMALLINT,\
                                                 col4 TIMESTAMP)"   //ʱ���
//����(???)��SQL���                                           
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

	//���ӵ�mysql server
	mysql = mysql_real_connect(mysql, "localhost", "root", "123456", "mydb2", 0, NULL,  0);
	if (mysql == NULL) {
		ret =  mysql_errno(mysql) ;
		printf("connect error, %s\n", mysql_error(mysql));
		goto END;
	}
	
	//////////////////////////////////////////////////////
	MYSQL_STMT    *stmt;
	MYSQL_BIND    bind[3];							//�ṹ������
	my_ulonglong  affected_rows;
	int           param_count;
	short         small_data;
	int           int_data;
	char          str_data[STRING_SIZE];
	unsigned long str_length;
	my_bool       is_null;
	 
	if (mysql_query(mysql, DROP_SAMPLE_TABLE)) {		//���������ɾ��test_table	
	  fprintf(stderr, " DROP TABLE failed\n");
	  fprintf(stderr, " %s\n", mysql_error(mysql));
	  exit(0);
	}
	 
	if (mysql_query(mysql, CREATE_SAMPLE_TABLE)) {	//��������ɴ���test_table
	  fprintf(stderr, " CREATE TABLE failed\n");
	  fprintf(stderr, " %s\n", mysql_error(mysql));
	  exit(0);
	}
	 
	stmt = mysql_stmt_init(mysql); 					//����������ʼ��Ԥ���������
	if (!stmt) {
	  fprintf(stderr, " mysql_stmt_init(), out of memory\n");
	  exit(0);
	}
	
	//����������Ԥ�����������Ӵ�(???)��sql���
	if (mysql_stmt_prepare(stmt, INSERT_SAMPLE, strlen(INSERT_SAMPLE))) {

	  fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
	  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	  exit(0);
	}
	fprintf(stdout, " prepare, INSERT successful\n");

	param_count = mysql_stmt_param_count(stmt);		//�����Ժ��������ж��ٸ���?��
	fprintf(stdout, " total parameters in INSERT: %d\n", param_count);
	 
	if (param_count != 3) { 						//������3�����������ж�
	  fprintf(stderr, " invalid parameter count returned by MySQL\n");
	  exit(0);
	}
	 
	memset(bind, 0, sizeof(bind));
	 
	bind[0].buffer_type= MYSQL_TYPE_LONG;
	bind[0].buffer= (char *)&int_data;			// ��ע�� 1��
	bind[0].is_null= 0;  
	bind[0].length= 0;	 //���ַ�������
	 
	bind[1].buffer_type= MYSQL_TYPE_STRING;
	bind[1].buffer= (char *)str_data;			// ��ע�� 2��
	bind[1].buffer_length= STRING_SIZE;
	bind[1].is_null= 0;
	bind[1].length= &str_length;				// ��ע�� 3��
	 
	bind[2].buffer_type= MYSQL_TYPE_SHORT;
	bind[2].buffer= (char *)&small_data;		// ��ע�� 4��
	bind[2].is_null= &is_null;			 		// ��ע�� 5��
	bind[2].length= 0;
	 
	if (mysql_stmt_bind_param(stmt, bind)) { 		//���������Ѹ�ֵ�Ժ������ ��ӵ�Ԥ���������
	  fprintf(stderr, " mysql_stmt_bind_param() failed\n");
	  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	  exit(0);
	}
	 
	int_data= 10;            							        //��Ӧ ��ע�� 1��
	strncpy(str_data, "MySQL", STRING_SIZE); 					//��Ӧ ��ע�� 2��
	str_length= strlen(str_data);								//��Ӧ ��ע�� 3�� 
	is_null= 1;		//1��ʾ��ֵΪNULL�� 0��ʾ��NULL		      	//��Ӧ ��ע�� 5��
	 
	if (mysql_stmt_execute(stmt)) { 		//��������ִ���Ѿ���ȫ���ֵ��SQL��䣬�����˵�һ������
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
	 
	int_data= 1000;																																					//��Ӧ ��ע�� 1��
	strncpy(str_data, "The most popular Open Source database", STRING_SIZE);//��Ӧ ��ע�� 2��
	str_length= strlen(str_data);											//��Ӧ ��ע�� 3��
	small_data= 1000;         												//��Ӧ ��ע�� 4��
	is_null= 0;               												//��Ӧ ��ע�� 5��
	 
	if (mysql_stmt_execute(stmt)) {  		//����������ִ��������ֵ��SQL��䡣 �����˵ڶ�������
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
		mysql_close(mysql);				//�Ͽ���SQL server������
	
	return 0;
}
