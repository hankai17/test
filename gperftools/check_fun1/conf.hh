#ifndef CONF_HPP
#define CONF_HPP

#include<iostream>
#include<ext/hash_map>
#include<unordered_map>
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
class dns_conf {
public:
	void open_conf_file();
	void close_conf_file();
    int parse_white_conf();
	int parse_black_conf();
	int posAfterStar(const char *,int &,char &);

    int check_black(char *,int);
    int in_black_head(char *,int);
    int in_black_rear(char *);

    int check_white(char *,int);
    int in_white_head(char *,int);
    int in_white_rear(char *);

    int in_head(char*,int);
    int in_rear(char*);
    int check(char*,int);
public:
	std::string      c_white_filename;
	std::string      c_black_filename;
	std::ifstream    c_white_filestream;
	std::ifstream    c_black_filestream;
	int          lineNum;
	int					block_index;
	int  unkown_block_start;
	int  in_white_flag;
	int  in_black_flag;

    int  black_head_flag;	
    int  black_rear_flag;	
    int  white_head_flag;	
    int  white_rear_flag;	

	std::vector< std::vector<icr_str_t *>* > str_servers;	
    int group;
    ~dns_conf()
    {
        for(int i=0; i<(int)str_servers.size(); i++)
        {
            if(str_servers[i])
            {
                delete str_servers[i];
                str_servers[i]=nullptr;
            }
        }
    }
std::unordered_map<unsigned long,int,__gnu_cxx::hash<unsigned long>,hm_strcmp2> servers_hash;

std::unordered_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1> str_white_domain_hash;
std::unordered_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1> str_white_head_hash;
std::unordered_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1> str_white_rear_hash;

std::unordered_map<char*,int,__gnu_cxx::hash<char*>,hm_strcmp1> str_black_domain_hash;
std::unordered_map<char*,int,__gnu_cxx::hash<char*>,hm_strcmp1> str_black_head_hash;
std::unordered_map<char*,int,__gnu_cxx::hash<char*>,hm_strcmp1> str_black_rear_hash;
};

#endif
