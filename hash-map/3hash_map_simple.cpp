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

class str_hash
{
    public:
    size_t operator()(const std::string &str) const
    {
        unsigned long __h=0;
        for(size_t i=0;i<str.size();i++)
        __h=5*__h+str[i];
        return size_t(__h);
    }
};

class hm_strcmp
{
    public:
    bool operator()(const std::string& s1,const std::string& s2) const
    {
        return s1==s2;
    }
};
//最好必须加上仿函数,防止所插入的key重复,第三个参数必须与第一个参数类型相同
__gnu_cxx::hash_map<std::string,int,__gnu_cxx::hash<std::string>,hm_strcmp> domain_hash;
__gnu_cxx::hash_map<std::string,std::vector<std::string>*,str_hash,hm_strcmp> test_hash;

typedef __gnu_cxx::hash_map<std::string,std::vector<std::string>*,str_hash,hm_strcmp>::iterator it;


int main()
{
    std::string s1("hello");
    std::vector<std::string>*p = new std::vector<std::string>;
    (*p).push_back("mikedog");
    test_hash.insert(std::pair<std::string,std::vector<std::string>*>(s1,p));
    test_hash.insert(std::pair<std::string,std::vector<std::string>*>("1",p));
    test_hash.insert(std::pair<std::string,std::vector<std::string>*>(s1,p));
    std::cout<<test_hash.size()<<std::endl;
    it it1;    

    if((it1=test_hash.find("hello"))!=test_hash.end())
     {   
        std::cout<<"find it"<<std::endl;
        //std::cout<<it1->first<<" "<<*(it1->second)[0]<<std::endl;
        std::cout<<it1->first<<" "<<(*(it1->second))[0]<<std::endl;//must in this way
     }
      
    else 
        std::cout<<"not find it"<<std::endl;


    return 0;
}

/*
__gnu_cxx::hash_map<std::string,int,str_hash,hm_strcmp> test_hash;
int main()
{
    char s1[]="hello";//插入的必须是栈上的内存
    std::string s2("hello");
    test_hash.insert(std::pair<std::string,int>(s2,2));
    test_hash.insert(std::pair<std::string,int>("1",1));
    std::cout<<test_hash.size()<<std::endl;

    return 0;
}
*/






























