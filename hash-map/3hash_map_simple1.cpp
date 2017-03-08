/*************************************************************************
	> File Name: simple.cpp
	> Author: Bill
	> Mail: hankai17@126.com 
	> Created Time: 2017-01-05 10:40:36
 ************************************************************************/

#include<iostream>
#include<string>
#include<string.h>
#include<ext/hash_map>//can use gnu hash_map

struct hmstrcmp
{
    bool operator()(const char* p1,const char*p2) const
    {
        return strcmp(p1,p2) == 0;    
    }
}
/*
struct chash
{
    
}
*/
//最好必须加上仿函数,防止所插入的key重复,第三个参数必须与第一个参数类型相同
__gnu_cxx::hash_map< char*,int,__gnu_cxx::hash<const char*>,hmstrcmp > charhash;

int main()
{
    char name[20] = "mike";
    //char_hash.insert(std::pair<char*,void*>(name,1));
    return 0;
}





























