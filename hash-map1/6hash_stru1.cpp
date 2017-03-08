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
    bool operator()(const unsigned long &p1,const unsigned  long &p2) const
    {
        return p1 == p2;
    }
}; 

__gnu_cxx::hash_map<unsigned long ,int,__gnu_cxx::hash< unsigned long >,hm_strcmp1> struct_hash;

int main()
{
    s* s1 = new s; 
    s* s2 = new s; 
    s* s3 = new s; 
    cout<<"sizeof(long)="<<sizeof(long)<<endl;
    cout<<"sizeof(long long)="<<sizeof(long long)<<endl;
    struct_hash.insert(__gnu_cxx::hash_map<unsigned  long,int>::value_type((unsigned  long)s1,100));
    struct_hash.insert(__gnu_cxx::hash_map<unsigned  long,int>::value_type((unsigned  long)s2,200));
    struct_hash.insert(__gnu_cxx::hash_map<unsigned  long,int>::value_type((unsigned  long)s3,300));
    
    __gnu_cxx::hash_map<unsigned long,int,__gnu_cxx::hash<unsigned long>,hm_strcmp1>::iterator iter;
    if((iter = struct_hash.find((unsigned long)s3)) != struct_hash.end()) {
        std::cout<<"test "<<iter->second<<std::endl;
    }
    return 0;
}
