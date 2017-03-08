/*************************************************************************
	> File Name: char_hash1.cpp
	> Author: Bill
	> Mail: hankai17@126.com 
	> Created Time: 2017-01-24 15:41:43
 ************************************************************************/

#include<ext/hash_map>
#include<string>
#include<iostream>

#include<string.h>

struct m_strcmp
{
    bool operator()(const char*p1,const char* p2) const
    {
        return strcasecmp(p1,p2) == 0;
    }
}; //stupid

__gnu_cxx::hash_map<char *,int,__gnu_cxx::hash<char *>,m_strcmp> test;

int main()
{
    char p[10] = "mike";
    test.insert(std::pair<char*,int>(p,1));
    //test.insert(std::pair<char*,int>("dog",2)); //this case: "dog" is a string object
    if(test.find(p) != test.end() )
    {
        std::cout<<"found it" <<std::endl;
    }
    else
    {
        std::cout<<"not found" <<std::endl;
    }

    return 0;
}
