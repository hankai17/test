#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql.h"

int main(void)
{
	int ret, num, i;
	MYSQL *mysql = NULL;
	char sqlbuf[1024];
	
	//��ʼ��
	mysql = mysql_init(NULL);
	if (mysql == NULL) {
		ret = mysql_errno(mysql);	
		printf("mysql_init error: %d\n", ret);
		return ret;
	}
	printf("mysql_init ok...\n");
	
	//�������ݿ�
	mysql = mysql_real_connect(mysql, "localhost", "root", "123456", "mydb2", 0, NULL, 0);
	if (mysql == NULL) {
		ret = mysql_errno(mysql);	
		printf("mysql_real_connect error: %d\n", ret);
		return ret;
	}
	printf("connect ok...\n");
	
	ret = mysql_query(mysql, "set names utf8");
	if (ret != 0) {
		printf("mysql_real_connect error: %d\n", ret);
		return ret;
	}	
	
	while (1) {
		memset(sqlbuf, 0, sizeof(sqlbuf));
		printf("YourSQL> ");
		fgets(sqlbuf, sizeof(sqlbuf), stdin);
		
		if (strncmp(sqlbuf, "exit", 4) == 0 || strncmp(sqlbuf, "quit", 4) == 0) {
			break;
		}
		
		if (strncmp(sqlbuf, "select", 6) == 0 || strncmp(sqlbuf, "SELECT", 6) == 0) {
			//ִ��SQL���
			//char *psql = "select * from emp";
			ret = mysql_query(mysql, sqlbuf);
			if (ret != 0) {
				printf("mysql_real_connect error: %d\n", ret);
				return ret;
			}
			
			//��ȡ����
			num = mysql_field_count(mysql);			
			
			//��ȡ�����
			MYSQL_RES *result = NULL;
			result = mysql_store_result(mysql);
			if (result == NULL) {
				printf("mysql_store_result error: %s\n", mysql_error(mysql));
				return -1;
			}
			printf("store_result ok...\n");
			
			//��ӡ��ͷ
			MYSQL_FIELD *fields = NULL;
			fields = mysql_fetch_fields(result);
			for(i = 0; i < num; i++) {
				printf("%10s\t", fields[i].name);
			}
			printf("\n");
		
			//��������
			MYSQL_ROW row;									//typedef char ** MYSQL_ROW;
			while ((row = mysql_fetch_row(result))) {
				for (i = 0; i < num; i++) {
					printf("%10s\t", row[i]);
				}
				printf("\n");	
			}
			
			//�ͷŽ����
			mysql_free_result(result);			
			
		} else {
			//�ǲ�ѯ��䡣
			ret = mysql_query(mysql, sqlbuf);
			if (ret != 0) {
				printf("mysql_real_connect error: %d\n", ret);
				return ret;
			}			
		}	
	}
	
	//�Ͽ�����
	mysql_close(mysql); 
	printf("close ok...\n");
	
	return 0;
}