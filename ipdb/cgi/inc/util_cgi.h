#ifndef _UTIL_CGI_H_
#define _UTIL_CGI_H_

#ifdef __cplusplus
extern "C" {
#endif
//makefile无所谓 结果都一样
//但是这里一旦注释掉 就无法链接 
//这是因为这里注释了 就只用c++编译器编译了 一旦用c++编译器编译
//所有的函数传参要求异常严格 必须按照c++风格来 
//所以传参的时候总是出错
//
//所以得写上这三句话 用c编译器编译是其一 按照c风格函数链接调用是其二
//linux下的c编译器 编译后的目标文件所有符号都是修饰后的符号不会加上任何修饰 链接时直接按照名字链接 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "make_log.h"

/**
 * @brief  解析url query 类似 abc=123&bbb=456 字符串
 *          传入一个key 得到相应的value
 * @returns 0 成功 -1 失败
 */
int query_parse_key_value(const char *query, const char *key, char *value, int *value_len);

#ifdef __cplusplus
}
#endif
#endif
