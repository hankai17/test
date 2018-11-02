#pragma once
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<fstream>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/time.h>


unsigned int get_uint(char* buffer, int offset);
char* int_2_str(int ip, char *buf);  
//打开文件并初始化   
int init_file(std::ifstream& f, char** buf/*out*/, int* file_size/*out*/);
//查找
int search_ip(char* buffer, int file_size, std::string& ip, std::string& result/*out*/);
//关闭文件
int close_file(std::ifstream& file, char* buffer);

