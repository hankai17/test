/*************************************************************************
	> File Name: 6hash_stru.cpp
	> Author: guaz
	> Mail: hankai17@126.com 
	> Created Time: 2017-02-14 12:18:24
 ************************************************************************/

#include<iostream>
#include<ext/hash_map>
#include<vector>
#include<sstream>   
#include<fstream>
#include<string>
#include<string.h>

using namespace std;

struct s{
    int a;
    int id;
};

struct hm_strcmp1
{
    bool operator()(const char*p1,const char* p2) const
    {
        //return strcasecmp(p1,p2) == 0; // this is error
        return p1 == p2;
    }
}; 

__gnu_cxx::hash_map<char*,int,__gnu_cxx::hash<char*>,hm_strcmp1> struct_hash;

int main()
{
    s* s1 = new s; 
    s* s2 = new s; 
    s* s3 = new s; 

    vector<std::string> * p_vec = new vector<std::string>;
   cout<<"sizeof p_vec="<<sizeof(p_vec)<<std::endl;

    int *p = nullptr;
    char *q = nullptr;
    long long l;

   cout<<"sizeof p="<<sizeof(p)<<std::endl;
   cout<<"sizeof q="<<sizeof(q)<<std::endl;
    cout<<"sizeof ll"<<sizeof(l)<<std::endl;

    struct_hash.insert(__gnu_cxx::hash_map<char*,int>::value_type((char*)s1,100));
    struct_hash.insert(__gnu_cxx::hash_map<char*,int>::value_type((char*)s2,200));
    struct_hash.insert(__gnu_cxx::hash_map<char*,int>::value_type((char*)s3,300));

    __gnu_cxx::hash_map<char*,int,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator iter;
    if((iter = struct_hash.find((char*)s3)) != struct_hash.end()) {
        std::cout<<"test "<<iter->second<<std::endl;
    }

    return 0;
}
