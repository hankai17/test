#ifndef CONF_HPP
#define CONF_HPP

#include<iostream>
#include<ext/hash_map>
#include<vector>
#include<sstream>   
#include<fstream>
#include<string>
#include<string.h>

typedef struct _icr_str_t {
    size_t           len;
    char           *data;
    _icr_str_t()
    {
        len     = 0;
        data    = nullptr;
    }
    _icr_str_t(size_t _len, char *_data)
    {
        len     = _len;
        data    = _data;
    }
    _icr_str_t& operator=(const _icr_str_t *p) {
        len  = p->len;
        data = p->data;
        return *this;
    }
    _icr_str_t(std::string _data) {
        len = _data.length();
        data = new char[_data.length()+1];
        bzero(data, _data.length()+1);
        memcpy(data, _data.c_str(), _data.length());
    }
}icr_str_t;

class hm_strcmp
{
    public:
        bool operator()(const std::string& s1,const std::string& s2) const
        {
            return s1==s2;
        }
};
struct str_hash
{
    size_t operator()(const std::string &str) const
    {
        unsigned long __h=0;
        for(size_t i=0;i<str.size();i++)
            __h=5*__h+str[i];
        return size_t(__h);
    }

};

struct hm_strcmp1
{
    bool operator()(const char*p1,const char* p2) const
    {
        return strcasecmp(p1,p2) == 0;
    }
}; 

struct hm_strcmp2
{
    bool operator()(const unsigned long &p1,const unsigned long &p2) const
    {
        return p1 == p2;
    }
}; 

class ip_obj {
    public:
    std::vector<icr_str_t*> ipv4_server;
    std::vector<icr_str_t*> ipv6_server;
    int group_id;

    void print () {
        for (const auto& it : ipv4_server) {
            std::cout<<"ipv4_server "<<it->data<<std::endl;
        }   
        for (const auto& it : ipv6_server) {
            std::cout<<"ipv6_server "<<it->data<<std::endl;
        }   
    }   
};       

class dns_conf {
    public:
        dns_conf(std::string);
        void open_conf_file();
        void close_conf_file();
        int parse_white_conf();
        int parse_black_conf();
        int posAfterStar(const char *,int &,char &);
        int ipv6_button_check(std::string );

        int in_head(char*,int,int*);
        int in_rear(char*,int*);
        int check(char*,int,int*);
    public:
        std::string      c_white_filename;
        std::string      c_black_filename;
        std::ifstream    c_white_filestream;
        std::ifstream    c_black_filestream;
        std::string file_path;
        int          lineNum;
        int					block_index;
        int  unkown_block_start;

        int  head_flag;	//名单中是否有前缀
        int  rear_flag;	
        int  max_pointes;

        std::vector< ip_obj* > str_servers; //vec中的每个vec*保存的都是每个组的ip	//vec* 作为hash_map的val
        __gnu_cxx::hash_map<char*, ip_obj*,__gnu_cxx::hash<char*>,hm_strcmp1> str_domain_hash;
        __gnu_cxx::hash_map<char*, ip_obj*,__gnu_cxx::hash<char*>,hm_strcmp1> str_head_hash;
        __gnu_cxx::hash_map<char*, ip_obj*,__gnu_cxx::hash<char*>,hm_strcmp1> str_rear_hash;

        ~dns_conf() {
            /*
            for(int i=0; i<(int)str_servers.size(); i++)
            {
                if(str_servers[i])
                {
                    delete str_servers[i];
                    str_servers[i]=nullptr;
                }
            }
            */
        }
};

#endif

