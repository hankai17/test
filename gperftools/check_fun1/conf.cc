#include "conf.hh"
#include <gperftools/profiler.h>

int dns_conf::posAfterStar(const char *str, int &point_num, char &wildcard_flash) {
    if (!str) {
        return 0;
    }
    const char *_p = str;
    char c = *_p;
    int  pos = 0;
    point_num = 0;
    while (*_p) {
        if (c == '*') {
            pos = _p + 1 - str;
            if (point_num == 0)
            {
                wildcard_flash = '1';
            }
            else
            {
                wildcard_flash = '2';
            }
        }
        else if (c == '.') {
            point_num++;
        }
        _p += 1;
        c = *_p;
    }
    return pos;
}


void dns_conf::open_conf_file() {
    c_white_filename = "/usr/local/etc/domain-white-list";
    c_black_filename = "/usr/local/etc/domain-black-list";
    try
    {
        c_white_filestream.open(c_white_filename);
        c_black_filestream.open(c_black_filename);
        if (!(c_black_filestream.is_open() && c_white_filestream.is_open()) ) {
            //std::cerr << "open " << cfilename << " failed \n";
            std::cout << "open err" << std::endl;
            std::exit(1);
        }
        std::cout << "open success" << std::endl;
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
}

void dns_conf::close_conf_file() {
    if (c_white_filestream.is_open()) {
        c_white_filestream.close();
    }
    if (c_black_filestream.is_open()) {
        c_black_filestream.close();
    }
}

int dns_conf::parse_black_conf() {
    char ch;
    int pn;
    char flag='0';
    int line_num=0;
    std::ostringstream _strline;
    while (c_black_filestream.get(ch)) {
        //std::cout<<"ch:"<<ch <<"  ascii:"<<(int)ch<<std::endl;    
        if(ch == '\n')
        {
            line_num++;
            posAfterStar(_strline.str().c_str(), pn, flag);
            std::cout<<"domain:"<<_strline.str();
            std::cout<<"   flag:"<<flag<<std::endl;

            if(flag == '0') 
            {
                //domain_black_hash.insert(__gnu_cxx::hash_map<std::string, int>::value_type(_strline.str(), line_num));

                char* tmp = new char [_strline.str().length()+1]; 
                strcpy(tmp,_strline.str().c_str());
                //std::cout<<"black:"<<tmp<<std::endl;
                str_black_domain_hash.insert(std::unordered_map<char*,int>::value_type(tmp,line_num));

            }
            else if(flag=='1')
            {
                //black_head_hash.insert(__gnu_cxx::hash_map<std::string, int>::value_type(_strline.str().erase(0,1), line_num));
                char* tmp = new char [_strline.str().length()]; 
                strcpy(tmp,_strline.str().erase(0,1).c_str());
                //std::cout<<"black:"<<tmp<<std::endl;
                str_black_head_hash.insert(std::unordered_map<char*,int>::value_type(tmp,line_num));
                black_head_flag = 1;
            }
            else if(flag=='2')
            {
                //black_rear_hash.insert(__gnu_cxx::hash_map<std::string, int>::value_type(_strline.str().substr(0, _strline.str().find("*")), line_num));
                char* tmp = new char [_strline.str().length()]; 
                strcpy(tmp,_strline.str().substr(0, _strline.str().find("*")).c_str());
                //std::cout<<"black:"<<tmp<<std::endl;
                str_black_rear_hash.insert(std::unordered_map<char*,int>::value_type(tmp,line_num));
                black_rear_flag = 1;
            }
            flag='0'; 
            _strline.str("");

            continue;
        }
        _strline << ch;
    }
    return 0;
}

int dns_conf::parse_white_conf()
{
    char ch;
    int block_num = 0;
    int ref_num = 0;
    std::ostringstream _strline;
    int new_flag = 0;
    int group_flag = 1;//flag=0时 在一个组
    //std::vector<std::string>* p=NULL;//存ip
    std::vector<icr_str_t*>* str_p=NULL;
    int group1=0;//第几组

    int servers_flag=0;//servers开始
    int domains_flag=0;//domains开始

    while (c_white_filestream.get(ch)) {
        if(ch == '{')
            block_num++;
        if(ch == '}')
            block_num--;
        if(block_num == 0)
        {   //当一个组结束 置标志位,同时清空_strline
            //不能清空
            group_flag=1;
            //_strline.str("");
        }

        if(block_num == 1 && group_flag == 1) 
        {
            //每一个组的开始,只进来一次
            group1++;
            //std::cout<<"组名:"<<_strline.str()<<"  第"<<group<<"组"<<std::endl;
            //std::cout<<"  第"<<group<<"组"<<std::endl;
            std::cout<<"  G"<<group1<<"group1"<<std::endl;
            new_flag=0; 
            group_flag=0;
            _strline.str("");
        }
        if(ch == '=')//一个小组开始 , 假设小组里不再嵌套
        {
            if((int)_strline.str().find("servers",0) != -1)
            {
                servers_flag = 1;
            }else if((int)_strline.str().find("domains",0) != -1)
            {
                domains_flag = 1;
            }
            else
            {
                std::cout<<"not exist the key"<<std::endl;
                return 1;
            }
        }
        if(servers_flag == 1)//解析servers小组
        {
            if (new_flag == 0)//只进来一次
            {
                //std::vector<std::string>* new_p = new std::vector < std::string >;
                //p = new_p;
                //servers.push_back(p);
                //append
                std::vector<icr_str_t*>* str_new_p = new std::vector<icr_str_t*>;
                str_p = str_new_p;	
                str_servers.push_back(str_p);

                servers_hash.insert(std::unordered_map<unsigned long,int>::value_type((unsigned long)str_new_p,group1));

                if(servers_hash.find((unsigned long)str_servers[0]) != servers_hash.end()) {
                    std::cout<<"success insert !"<<std::endl;
                }

                //std::cout<<"insert key="<<static_cast<const void*>(str_new_p)<<std::endl;
                std::cout<<"group1="<<group1<<std::endl;	
                new_flag = 1;            
                _strline.str("");
            }
            if (ch == '"')
            {
                ref_num++;
                if(ref_num == 1)
                    _strline.str("");//第一个" 则立即清空_strline
                continue;
            }
            if (ref_num == 2)
            {
                //std::cout<<"ip: "<<_strline.str()<<std::endl;
                //(*p).push_back(_strline.str());

                //append
                icr_str_t *tmp (new icr_str_t(_strline.str()));
                (*str_p).push_back(tmp);

                _strline.str("");
                ref_num = 0;
            }
            if(ch == '}')    //一个小组的结束标志.但是如果有嵌套 那就得修改了
            {
                servers_flag = 0;
                _strline.str("");
            }
        }
        if( domains_flag == 1)//解析域名小组
        {
            //这之前ref_num必须为0
            if (ch == '"')
            {
                ref_num++;
                if(ref_num == 1)
                    _strline.str("");//第一个" 则立即清空_strline
                continue;
            }
            if (ref_num == 2)
            {
                int pn; char flag = '0';
                //_strline << '.';
                std::cout<<"domain: "<<_strline.str()<<std::endl;
                posAfterStar(_strline.str().c_str(), pn, flag);
                if (flag == '0')
                {
                    //普通
                    //domain_white_hash.insert(__gnu_cxx::hash_map<std::string, std::vector<std::string>*>::value_type(_strline.str(), p));
                    char* tmp = new char [_strline.str().length()+1];  //!!!!can free!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    strcpy(tmp,_strline.str().c_str());
                    //append  								
                    str_white_domain_hash.insert(std::unordered_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1>::value_type(tmp,str_p));					
                    //delete []tmp;
                }
                else if (flag == '1')
                {
                    //前缀  //需要去掉前缀的* 
                    //white_head_hash.insert(__gnu_cxx::hash_map<std::string, std::vector<std::string>*>::value_type(_strline.str().erase(0,1), p));

                    //append					
                    char* tmp = new char [_strline.str().length()];  
                    strcpy(tmp,_strline.str().erase(0,1).c_str());
                    str_white_head_hash.insert(std::unordered_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1>::value_type(tmp,str_p));					
                    //delete []tmp;
                    white_head_flag = 1;
                }
                else if (flag == '2')
                {	
                    //后缀  //需要去掉后缀的*				
                    //white_rear_hash.insert(__gnu_cxx::hash_map<std::string, std::vector<std::string>*>::value_type(_strline.str().substr(0, _strline.str().find("*")), p));

                    //append
                    char* tmp = new char [_strline.str().length()];  
                    strcpy(tmp,_strline.str().substr(0, _strline.str().find("*")).c_str());
                    str_white_rear_hash.insert(std::unordered_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1>::value_type(tmp,str_p));					
                    //delete []tmp;
                    white_rear_flag = 1;
                }
                //domain.push_back(_strline.str());
                _strline.str("");
                ref_num = 0;
            }
            if(ch == '}')
            {
                domains_flag = 0;
                _strline.str("");
            }
        }
        _strline << ch;
    }
    if(block_num != 0)
    {
        std::cout<<"block not match"<<std::endl;
        return 1;
    }
    return 0;
}

int dns_conf::in_black_head(char * str_domain,int len)
{
    char* tmp_addr;
    tmp_addr = str_domain+len-1;

    while(tmp_addr != str_domain)
    {
        tmp_addr -= 1;//	error tmp_addr --;	
        if( *tmp_addr == '.')
        {
            std::unordered_map<char*, int,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator iter;
            if( ( iter = str_black_head_hash.find(tmp_addr)) != str_black_head_hash.end() ) {
                return 1;				
            }	
        }
    }	
    return 0;
}
int dns_conf::in_black_rear(char * str_domain)
{
    char *tmp_addr = str_domain;
    char tmp_char;
    while(*tmp_addr != '\0')
    {
        if(*tmp_addr == '.')
        {
            tmp_addr += 1;
            tmp_char = *tmp_addr;  //std::cout<<"tmp_char:"<<tmp_char<<std::endl;
            *tmp_addr = '\0';
            std::unordered_map<char*, int,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator iter;
            if( ( iter = str_black_rear_hash.find(str_domain)) != str_black_rear_hash.end() ) {
                *tmp_addr = tmp_char;		
                return 1;	
            }
            *tmp_addr = tmp_char;		
        }
        tmp_addr += 1;
    }
    return 0;	
}
int dns_conf::check_black(char * str_domain,int len) 
{
    std::unordered_map<char*, int,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator iter;
    if( ( iter = str_black_domain_hash.find(str_domain)) != str_black_domain_hash.end() ) {
        std::cout<<"in normal black list,normal process"<<std::endl;
        in_black_flag = 1;
        return 1;
    }
    else if ( in_black_head(str_domain,len) == 1) {
        std::cout<<"in black head list,normal process"<<std::endl;
        in_black_flag = 1;
        return 1;
    }
    else if ( in_black_rear(str_domain) == 1)	{
        std::cout<<"in black rear list,normal process"<<std::endl;
        in_black_flag = 1;
        return 1;
    }
    else {
        std::cout<<"str_domain="<<str_domain;
        std::cout<<"  not in black list,check white list"<<std::endl;
        in_black_flag = 0;
    } 
    return 0;		
}



int dns_conf::in_white_head(char * str_domain,int len)
{
    char* tmp_addr;
    tmp_addr = str_domain + len -1;
    while(tmp_addr != str_domain)
    {
        tmp_addr -= 1;//	error tmp_addr --;	
        if( *tmp_addr == '.')
        {
            std::unordered_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator iter;
            if( ( iter = str_white_head_hash.find(tmp_addr)) != str_white_head_hash.end() ) {
                std::unordered_map<unsigned long, int,__gnu_cxx::hash<unsigned long>,hm_strcmp2>::iterator itr;
                if( (itr = servers_hash.find((unsigned long)((*iter).second))) != servers_hash.end()) {
                    group = itr->second;
                }
                return 1;				
            }	
        }
    }	
    return 0;	
}
int dns_conf::in_white_rear(char *str_domain)
{
    char *tmp_addr = str_domain;
    char tmp_char;
    while(*tmp_addr != '\0')
    {
        if(*tmp_addr == '.')
        {
            tmp_addr += 1;
            tmp_char = *tmp_addr;  //std::cout<<"tmp_char:"<<tmp_char<<std::endl;
            *tmp_addr = '\0';
            std::unordered_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator iter;
            if( ( iter = str_white_rear_hash.find(str_domain)) != str_white_rear_hash.end() ) {
                std::unordered_map<unsigned long, int,__gnu_cxx::hash<unsigned long>,hm_strcmp2>::iterator itr;
                if( (itr = servers_hash.find((unsigned long)((*iter).second))) != servers_hash.end()) {
                    group = itr->second;
                }
                *tmp_addr = tmp_char;		
                return 1;	
            }
            *tmp_addr = tmp_char;		
        }
        tmp_addr += 1;
    }
    return 0;	
}

int dns_conf::check_white(char *str_domain,int len)
{
    std::unordered_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator iter;
    if( ( iter = str_white_domain_hash.find(str_domain)) != str_white_domain_hash.end() ) {
        std::cout<<"in normal white list,my process"<<std::endl;
        std::unordered_map<unsigned long, int,__gnu_cxx::hash<unsigned long>,hm_strcmp2>::iterator itr;
        if( (itr = servers_hash.find((unsigned long)((*iter).second))) != servers_hash.end()) {
            group = itr->second;
        }
        in_white_flag = 1;
        return 1;
    }
    else if ( in_white_head(str_domain,len) == 1) {
        std::cout<<"in white head list,my process"<<std::endl;
        in_white_flag = 1;
        return 1;
    }
    else if ( in_white_rear(str_domain) == 1)	{
        std::cout<<"in white rear list,my process"<<std::endl;
        in_white_flag = 1;
        return 1;
    }
    else {
        std::cout<<"str_domain="<<str_domain;
        std::cout<<"  not in white list,normal process"<<std::endl;
        in_white_flag = 0;
    } 
    return 0;		
}	

int dns_conf::check(char* str_domain,int len)
{
    ProfilerStart("checkfun");
    in_white_flag = 0;
    in_black_flag = 0;
    std::unordered_map<char*, int,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator b_iter;
    std::unordered_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator w_iter;
    if( ( b_iter = str_black_domain_hash.find(str_domain)) != str_black_domain_hash.end() ) {
        std::cout<<"in normal black list,normal process"<<std::endl;
        in_black_flag = 1;
        return 1;
    }
    else if( ( w_iter = str_white_domain_hash.find(str_domain)) != str_white_domain_hash.end() ) {
        std::cout<<"in normal white list,my process"<<std::endl;
        std::unordered_map<unsigned long, int,__gnu_cxx::hash<unsigned long>,hm_strcmp2>::iterator itr;
        if( (itr = servers_hash.find((unsigned long)((*w_iter).second))) != servers_hash.end()) {
            group = itr->second;
        }
        in_white_flag = 1;
        return 2;
    }
    else 
    {
        if(black_head_flag == 1 || white_head_flag == 1)
        {
            int ret = in_head(str_domain,len);    
            if( 1 == ret )
            {
                in_black_flag = 1;
                return 3;
            }
        }
        if( black_rear_flag == 1 || white_rear_flag == 1)
        {
            {
                int ret1 = in_rear(str_domain);
                if( 1==ret1 )
                {
                    in_black_flag = 1;
                    return 5;
                }
                else if( 0 == ret1 )
                {
                    if( 1 == in_white_flag)
                    {
                        in_white_flag = 1;
                        return 2;
                    }
                    else
                    {
                        std::cout<<"just normal"<<std::endl;
                        in_white_flag = 0;
                        in_black_flag = 0;
                        return 0;
                    }
                }
            }
        }
    }
    ProfilerStop();
    return 0;		
}

int dns_conf::in_rear(char *str_domain)
{
    char *tmp_addr = str_domain;
    char tmp_char;
    while(*tmp_addr != '\0')
    {
        if(*tmp_addr == '.')
        {
            tmp_addr += 1;
            tmp_char = *tmp_addr;  //std::cout<<"tmp_char:"<<tmp_char<<std::endl;
            *tmp_addr = '\0';

            std::unordered_map<char*, int,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator b_iter;
            std::unordered_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator w_iter;
            if(black_rear_flag == 1)
            {
                if( ( b_iter = str_black_rear_hash.find(str_domain)) != str_black_rear_hash.end() ) {
                    *tmp_addr = tmp_char;		
                    std::cout<<"in black rear"<<std::endl;
                    return 1;	
                }
            }
            if(white_rear_flag == 1)
            {
                if( ( w_iter = str_white_rear_hash.find(str_domain)) != str_white_rear_hash.end() ) {
                    std::unordered_map<unsigned long, int,__gnu_cxx::hash<unsigned long>,hm_strcmp2>::iterator itr;
                    if( (itr = servers_hash.find((unsigned long)((*w_iter).second))) != servers_hash.end()) {
                        group = itr->second;
                    }
                    in_white_flag = 1;
                    *tmp_addr = tmp_char;		
                    std::cout<<"in white rear"<<std::endl;
                }
            }
            *tmp_addr = tmp_char;		
        }
        tmp_addr += 1;
    }
    return 0;	
}

//optimize
// return 1:in black head list. return 2:in white head list. return 0:not in head
int dns_conf::in_head(char * str_domain,int len)
{
    char* tmp_addr;
    tmp_addr = str_domain + len -1;
    while(tmp_addr != str_domain)
    {
        tmp_addr -= 1;//	error tmp_addr --;	
        if( *tmp_addr == '.')
        {
            std::unordered_map<char*, int,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator h_iter;
            std::unordered_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator w_iter;
            if( black_head_flag == 1)
            {
                if( ( h_iter = str_black_head_hash.find(tmp_addr)) != str_black_head_hash.end() ) {
                    std::cout<<"in black head"<<std::endl;
                    return 1;				
                }	
            }
            if( white_head_flag == 1)
            {
                if( ( w_iter = str_white_head_hash.find(tmp_addr)) != str_white_head_hash.end() ) {
                    std::unordered_map<unsigned long, int,__gnu_cxx::hash<unsigned long>,hm_strcmp2>::iterator itr;
                    if( (itr = servers_hash.find((unsigned long)((*w_iter).second))) != servers_hash.end()) {
                        group = itr->second;
                    }
                    std::cout<<"in white head"<<std::endl;
                    in_white_flag = 1;
                }	
            }
        }
    }	
    return 0;	
}


