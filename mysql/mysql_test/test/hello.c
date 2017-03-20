#include <stdio.h>
#include "mysql.h"

int main(void)
{
	int ret, num, i;
	MYSQL *mysql = NULL;
	
	//��ʼ��
	//MYSQL *mysql_init(MYSQL *mysql)
	mysql = mysql_init(NULL);
	if (mysql == NULL) {
		//unsigned int mysql_errno(MYSQL *mysql)
		ret = mysql_errno(mysql);	
		printf("mysql_init error: %d\n", ret);
		return ret;
	}
	printf("mysql_init ok...\n");
	
	//�������ݿ�
	//MYSQL *mysql_real_connect(MYSQL *mysql, const char *host, const char *user, const char *passwd, const char *db, unsigned int port, const char *unix_socket, unsigned long client_flag) 
	mysql = mysql_real_connect(mysql, "localhost", "root", "123456", "mydb61", 0, NULL, 0);
	if (mysql == NULL) {
		ret = mysql_errno(mysql);	
		printf("mysql_real_connect error: %d\n", ret);
		return ret;
	}
	printf("connect ok...\n");	
	
	//ִ��SQL���
	//int mysql_query(MYSQL *mysql, const char *query) 
	char *psql = "select * from emp";
	ret = mysql_query(mysql, psql);
	if (ret != 0) {
		printf("mysql_real_connect error: %d\n", ret);
		return ret;
	}
	
	//��ȡ����
	//unsigned int mysql_field_count(MYSQL *mysql)
	num = mysql_field_count(mysql);
	
	
	//��ȡ�����
	//MYSQL_RES *mysql_store_result(MYSQL *mysql) 
	MYSQL_RES *result = NULL;
	result = mysql_store_result(mysql);
	if (result == NULL) {
		printf("mysql_store_result error: %s\n", mysql_error(mysql));
		return -1;
	}
	printf("store_result ok...\n");
	
	//��ӡ��ͷ
	//MYSQL_FIELD *mysql_fetch_fields(MYSQL_RES *result) 
	MYSQL_FIELD *fields = NULL;
	fields = mysql_fetch_fields(result);
	for(i = 0; i < num; i++) {
		printf("%10s\t", fields[i].name);
	}
	printf("\n");

	
	//��������
	//MYSQL_ROW mysql_fetch_row(MYSQL_RES *result)	
	MYSQL_ROW row;									//typedef char ** MYSQL_ROW;
	while ((row = mysql_fetch_row(result))) {
		for (i = 0; i < num; i++) {
			printf("%10s\t", row[i]);
		}
		printf("\n");	
	}
	
	//�ͷŽ����
	//void mysql_free_result(MYSQL_RES *result) 
	mysql_free_result(result);
	
	//�Ͽ�����
	//void mysql_close(MYSQL *mysql) 
	mysql_close(mysql); 
	printf("close ok...\n");
	
	return 0;
}