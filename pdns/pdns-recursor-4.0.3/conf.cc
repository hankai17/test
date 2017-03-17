#include "conf.hh"
#include "logger.hh"

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

    //c_white_filename = "${SYSCONFDIR}/domain-white-list";
    //c_black_filename = "${SYSCONFDIR}/domain-black-list";
    try
    {
        c_white_filestream.open(c_white_filename);
        c_black_filestream.open(c_black_filename);
        if (!(c_black_filestream.is_open() && c_white_filestream.is_open()) ) {
            L<<Logger::Error<< "open err" << endl;
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
    std::cout<<"  Black"<<std::endl;
    while (c_black_filestream.get(ch)) {
        if(ch == '\n')
        {
            line_num++;
            posAfterStar(_strline.str().c_str(), pn, flag);
            std::cout<<"domain: "<<_strline.str();
            std::cout<<"   flag:"<<flag<<std::endl;

            if(flag == '0') 
            {
                char* tmp = new char [_strline.str().length()+1]; 
                strcpy(tmp,_strline.str().c_str());
                if(str_black_domain_hash.find(tmp) == str_black_domain_hash.end())
                {
                    str_black_domain_hash.insert(__gnu_cxx::hash_map<char*,int>::value_type(tmp,line_num));
                }
                else
                {
                    L<<Logger::Error<<tmp<< " insert err" << endl;
                    std::cout<<tmp<<" insert err !"<<std::endl;
                    return 1;
                }
            }
            else if(flag=='1')
            {
                char* tmp = new char [_strline.str().length()]; 
                strcpy(tmp,_strline.str().erase(0,1).c_str());
                if(str_black_head_hash.find(tmp) == str_black_head_hash.end())
                {
                    str_black_head_hash.insert(__gnu_cxx::hash_map<char*,int>::value_type(tmp,line_num));
                }
                else
                {
                    L<<Logger::Error<<tmp<< " insert err" << endl;
                    std::cout<<tmp<<" insert err !"<<std::endl;
                    return 1;
                }
                black_head_flag = 1;
            }
            else if(flag=='2')
            {
                char* tmp = new char [_strline.str().length()]; 
                strcpy(tmp,_strline.str().substr(0, _strline.str().find("*")).c_str());
                if(str_black_rear_hash.find(tmp) == str_black_rear_hash.end())
                {
                    str_black_rear_hash.insert(__gnu_cxx::hash_map<char*,int>::value_type(tmp,line_num));
                }
                else
                {
                    L<<Logger::Error<<tmp<< " insert err" << endl;
                    std::cout<<tmp<<" insert err !"<<std::endl;
                    return 1;
                }
                black_rear_flag = 1;
            }
            flag='0'; 
            _strline.str("");

            continue;
        }
        _strline << ch;
    }
    std::cout<<std::endl;
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
        }

        if(block_num == 1 && group_flag == 1) 
        {
            //每一个组的开始,只进来一次
            group1++;
            std::cout<<"  Group"<<group1<<""<<std::endl;
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
                L<<Logger::Error<< "not exist the key" << endl;
                std::cout<<"not exist the key"<<std::endl;
                return 1;
            }
        }
        if(servers_flag == 1)//解析servers小组
        {
            if (new_flag == 0)//只进来一次
            {
                std::vector<icr_str_t*>* str_new_p = new std::vector<icr_str_t*>;
                str_p = str_new_p;	
                str_servers.push_back(str_p);

                servers_hash.insert(__gnu_cxx::hash_map<unsigned long,int>::value_type((unsigned long)str_new_p,group1));

                if(servers_hash.find((unsigned long)str_servers[0]) != servers_hash.end()) {
                    std::cout<<"success insert !"<<std::endl;
                }
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
                std::cout<<"domain: "<<_strline.str()<<std::endl;
                posAfterStar(_strline.str().c_str(), pn, flag);
                if (flag == '0')
                {
                    char* tmp = new char [_strline.str().length()+1]; 
                    strcpy(tmp,_strline.str().c_str());
                    if(str_white_domain_hash.find(tmp) == str_white_domain_hash.end())
                    {
                        str_white_domain_hash.insert(__gnu_cxx::hash_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1>::value_type(tmp,str_p));					
                    }
                    else
                    {
                        L<<Logger::Error<< tmp<<" insert err !" << endl;
                        std::cout<<tmp<<" insert err !"<<std::endl;
                        return 1;
                    }
                }
                else if (flag == '1')
                {
                    char* tmp = new char [_strline.str().length()];  
                    strcpy(tmp,_strline.str().erase(0,1).c_str());
                    if(str_white_head_hash.find(tmp) == str_white_head_hash.end())
                    {
                        str_white_head_hash.insert(__gnu_cxx::hash_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1>::value_type(tmp,str_p));					
                    }
                    else
                    {
                        L<<Logger::Error<< tmp<<" insert err !" << endl;
                        std::cout<<tmp<<" insert err !"<<std::endl;
                        return 1;
                    }
                    white_head_flag = 1;
                }
                else if (flag == '2')
                {	
                    char* tmp = new char [_strline.str().length()];  
                    strcpy(tmp,_strline.str().substr(0, _strline.str().find("*")).c_str());
                    if(str_white_rear_hash.find(tmp) == str_white_rear_hash.end())
                    {
                        str_white_rear_hash.insert(__gnu_cxx::hash_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1>::value_type(tmp,str_p));					
                    }
                    else
                    {
                        L<<Logger::Error<< tmp<<" insert err !" << endl;
                        std::cout<<tmp<<" insert err !"<<std::endl;
                        return 1;
                    }

                    white_rear_flag = 1;
                }
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
        L<<Logger::Error<<" block not match !" << endl;
        std::cout<<"block not match"<<std::endl;
        return 1;
    }
    return 0;
}

int dns_conf::check(char* str_domain,int len)
{
    in_white_flag = 0;
    in_black_flag = 0;
    __gnu_cxx::hash_map<char*, int,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator b_iter;
    __gnu_cxx::hash_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator w_iter;
    if( ( b_iter = str_black_domain_hash.find(str_domain)) != str_black_domain_hash.end() ) {
        in_black_flag = 1;
        return 1;
    }
    else if( ( w_iter = str_white_domain_hash.find(str_domain)) != str_white_domain_hash.end() ) {
        __gnu_cxx::hash_map<unsigned long, int,__gnu_cxx::hash<unsigned long>,hm_strcmp2>::iterator itr;
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
                        in_white_flag = 0;
                        in_black_flag = 0;
                        return 0;
                    }
                }
            }
        }
    }
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
            tmp_char = *tmp_addr;  
            *tmp_addr = '\0';
            __gnu_cxx::hash_map<char*, int,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator b_iter;
            __gnu_cxx::hash_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator w_iter;
            if(black_rear_flag == 1)
            {
                if( ( b_iter = str_black_rear_hash.find(str_domain)) != str_black_rear_hash.end() ) {
                    *tmp_addr = tmp_char;		
                    return 1;	
                }
            }
            if(white_rear_flag == 1)
            {
                if( ( w_iter = str_white_rear_hash.find(str_domain)) != str_white_rear_hash.end() ) {
                    __gnu_cxx::hash_map<unsigned long, int,__gnu_cxx::hash<unsigned long>,hm_strcmp2>::iterator itr;
                    if( (itr = servers_hash.find((unsigned long)((*w_iter).second))) != servers_hash.end()) {
                        group = itr->second;
                    }
                    in_white_flag = 1;
                    *tmp_addr = tmp_char;		
                }
            }
            *tmp_addr = tmp_char;		
        }
        tmp_addr += 1;
    }
    return 0;	
}

//optimize
int dns_conf::in_head(char * str_domain,int len)
{
    char* tmp_addr;
    tmp_addr = str_domain + len -1;
    while(tmp_addr != str_domain)
    {
        tmp_addr -= 1;
        if( *tmp_addr == '.')
        {
            __gnu_cxx::hash_map<char*, int,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator h_iter;
            __gnu_cxx::hash_map<char*, std::vector<icr_str_t*>*,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator w_iter;
            if( black_head_flag == 1)
            {
                if( ( h_iter = str_black_head_hash.find(tmp_addr)) != str_black_head_hash.end() ) {
                    return 1;				
                }	
            }
            if( white_head_flag == 1)
            {
                if( ( w_iter = str_white_head_hash.find(tmp_addr)) != str_white_head_hash.end() ) {
                    __gnu_cxx::hash_map<unsigned long, int,__gnu_cxx::hash<unsigned long>,hm_strcmp2>::iterator itr;
                    if( (itr = servers_hash.find((unsigned long)((*w_iter).second))) != servers_hash.end()) {
                        group = itr->second;
                    }
                    in_white_flag = 1;
                }	
            }
        }
    }	
    return 0;	
}

