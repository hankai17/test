#include "conf.hh"
#include "logger.hh"
#include "namespaces.hh"

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

dns_conf::dns_conf(std::string file_p):file_path(file_p){
    max_pointes = -1;
}

void dns_conf::open_conf_file() {
    c_white_filename = file_path + "/domain-white-list";
    c_black_filename = file_path + "/domain-black-list";

    try
    {
        c_white_filestream.open(c_white_filename);
        c_black_filestream.open(c_black_filename);
        if (!(c_black_filestream.is_open() && c_white_filestream.is_open()) ) {
            L<<Logger::Error<<file_path<< " domain-white-list or domain-black-list files open err" << endl;
            //std::cout << "open err" << std::endl;
            std::exit(1);
        }
        //std::cout << "open success" << std::endl;
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
    //std::cout<<"  Black"<<std::endl;
    while (c_black_filestream.get(ch)) {
        if(ch == '\n')
        {
            line_num++;
            posAfterStar(_strline.str().c_str(), pn, flag);
            max_pointes = pn > max_pointes ? pn : max_pointes;
            //std::cout<<"domain: "<<_strline.str();
            //std::cout<<"   flag:"<<flag<<std::endl;

            if(flag == '0') 
            {
                char* tmp = new char [_strline.str().length()+1]; 
                strcpy(tmp,_strline.str().c_str());
                if(str_domain_hash.find(tmp) == str_domain_hash.end())
                {
                    ip_obj* ip_message = new ip_obj;
                    ip_message->group_id = -1;
                    str_domain_hash.insert(__gnu_cxx::hash_map<char*, ip_obj*,__gnu_cxx::hash<char*>,hm_strcmp1>::value_type(tmp,ip_message));					
                }
                else
                {
                    L<<Logger::Error<<"line ="<<line_num<<" "<<tmp<< " warning insert conflict " << endl;
                }
            }
            else if(flag=='1')
            {
                char* tmp = new char [_strline.str().length()]; 
                strcpy(tmp,_strline.str().erase(0,1).c_str());
                if(str_head_hash.find(tmp) == str_head_hash.end())
                {
                    ip_obj* ip_message = new ip_obj;
                    ip_message->group_id = -1;
                    str_head_hash.insert(__gnu_cxx::hash_map<char*, ip_obj*,__gnu_cxx::hash<char*>,hm_strcmp1>::value_type(tmp,ip_message));					
                }
                else
                {
                    L<<Logger::Error<<"line ="<<line_num<<" "<<tmp<< " warning insert conflict " << endl;
                    //L<<Logger::Error<<tmp<< " warning insert conflict" << endl;
                }
                head_flag = 1;
            }
            else if(flag=='2')
            {
                char* tmp = new char [_strline.str().length()]; 
                strcpy(tmp,_strline.str().substr(0, _strline.str().find("*")).c_str());
                if(str_rear_hash.find(tmp) == str_rear_hash.end())
                {
                    ip_obj* ip_message = new ip_obj;
                    ip_message->group_id = -1;
                    str_rear_hash.insert(__gnu_cxx::hash_map<char*, ip_obj*,__gnu_cxx::hash<char*>,hm_strcmp1>::value_type(tmp,ip_message));					
                }
                else
                {
                    //L<<Logger::Error<<tmp<< " warning insert conflict" << endl;
                    L<<Logger::Error<<"line ="<<line_num<<" "<<tmp<< " warning insert conflict " << endl;
                }
                rear_flag = 1;
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
    int group_flag = 1;//flag=0ʱ ՚һ¸
    ip_obj* str_p = NULL;
    int group1=0;//µڼ¸ة

    int servers_flag=0;//servers¿ªʼ
    int domains_flag=0;//domains¿ªʼ

    while (c_white_filestream.get(ch)) {
        if(ch == '{')
            block_num++;
        if(ch == '}')
            block_num--;
        if(block_num == 0)
        {   //µ±һ¸������⋸ ׃±떾λ,ͬʱȥ¿ԟstrline
            //²»Ŝȥ¿
            group_flag=1;
        }

        if(block_num == 1 && group_flag == 1) 
        {
            //ÿһ¸������Ŀªʼ,ֻ½������´
            group1++;
            //std::cout<<"  Group"<<group1<<""<<std::endl;
            new_flag=0; 
            group_flag=0;
            _strline.str("");
        }
        if(ch == '=')//һ¸������鿪ʼ , ¼ډ鐡ةo²»ՙǶ͗ 
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
                L<<Logger::Error<< " not exist the key" << endl;
                return 1;
            }
        }
        if(servers_flag == 1)//½㏶serversСة
        {
            if (new_flag == 0)//ֻ½������´
            {
                ip_obj* str_new_p = new ip_obj;
		str_new_p->group_id = group1;
                str_p = str_new_p;	
                str_servers.push_back(str_p);

                new_flag = 1;            
                _strline.str("");
            }
            if (ch == '"')
            {
                ref_num++;
                if(ref_num == 1)
                    _strline.str("");//µے»¸������������´ȥ¿ԟstrline
                continue;
            }
            if (ref_num == 2)
            {
                std::string tmp_ip (_strline.str());
                if ( tmp_ip.find(':') != std::string::npos) {
                    icr_str_t *ipv6 (new icr_str_t(_strline.str()));
                    (*str_p).ipv6_server.push_back(ipv6);
                } else {
                    icr_str_t *ipv4 (new icr_str_t(_strline.str()));
                    (*str_p).ipv4_server.push_back(ipv4);
                }                                                   
                _strline.str("");
                ref_num = 0;
            }
            if(ch == '}')    //һ¸������鶄½⋸±떾.µ«ˇɧ¹������͗ Ň¾͵Đ޸Ł
            {
                servers_flag = 0;
                _strline.str("");
            }
        }
        if( domains_flag == 1)//½㏶ԲĻСة
        {
            //֢֮ǰref_num±ِ쎪0
            if (ch == '"')
            {
                ref_num++;
                if(ref_num == 1)
                    _strline.str("");//µے»¸������������´ȥ¿ԟstrline
                continue;
            }
            if (ref_num == 2)
            {
                int pn; char flag = '0';
                //std::cout<<"domain: "<<_strline.str()<<std::endl;
                posAfterStar(_strline.str().c_str(), pn, flag);
                max_pointes = pn > max_pointes ? pn : max_pointes;
                if (flag == '0')
                {
                    char* tmp = new char [_strline.str().length()+1]; 
                    strcpy(tmp,_strline.str().c_str());
                    if(str_domain_hash.find(tmp) == str_domain_hash.end())
                    {
                        //L<<Logger::Error<<tmp<< " inserted " << endl;
                        str_domain_hash.insert(__gnu_cxx::hash_map<char*, ip_obj*,__gnu_cxx::hash<char*>,hm_strcmp1>::value_type(tmp,str_p));					
                    }
                    else
                    {
                        L<<Logger::Error<<tmp<< " warning insert conflict " << endl;
                    }
                }
                else if (flag == '1')
                {
                    char* tmp = new char [_strline.str().length()];  
                    strcpy(tmp,_strline.str().erase(0,1).c_str());
                    if(str_head_hash.find(tmp) == str_head_hash.end())
                    {
                        str_head_hash.insert(__gnu_cxx::hash_map<char*, ip_obj*,__gnu_cxx::hash<char*>,hm_strcmp1>::value_type(tmp,str_p));					
                    }
                    else
                    {
                        L<<Logger::Error<<tmp<< " warning insert conflict " << endl;
                    }
                    head_flag = 1;
                }
                else if (flag == '2')
                {	
                    char* tmp = new char [_strline.str().length()];  
                    strcpy(tmp,_strline.str().substr(0, _strline.str().find("*")).c_str());
                    if(str_rear_hash.find(tmp) == str_rear_hash.end())
                    {
                        str_rear_hash.insert(__gnu_cxx::hash_map<char*, ip_obj*,__gnu_cxx::hash<char*>,hm_strcmp1>::value_type(tmp,str_p));					
                    }
                    else
                    {
                        L<<Logger::Error<<tmp<< " warning insert conflict " << endl;
                    }
                    rear_flag = 1;
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
        return 1;
    }
    return 0;
}

int dns_conf::ipv6_button_check(std::string button) 
{
    if (button.find("off",0) != button.npos) { //off
       for (int i = 0; i < (int)str_servers.size(); i++) {
          ip_obj* tmp = str_servers[i];
          if ( (*tmp).ipv6_server.size() > 0) {
             L<<Logger::Error<<" ipv6-mode is off. However find an ipv6 ip " <<std::endl;
             return 1;
          }   
       }   
    } else { //ipv6 on
    int flag = 0;
       for (int i = 0; i < (int)str_servers.size(); i++) {
          ip_obj* tmp = str_servers[i];
          if ( (*tmp).ipv6_server.size() > 0) {
              flag = 1;
          }   
       }   
       if (flag == 0) {
         L<<Logger::Error<<"ipv6-mode is on. However not find ipv6 ip!" << endl;
         return 1;
       }   
    }   
    return 0;
}

int dns_conf::check(char* str_domain, int len, int* gid) {
    __gnu_cxx::hash_map<char*, ip_obj*,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator iter;
    if ( ( iter = str_domain_hash.find(str_domain)) != str_domain_hash.end() ) { //¾«׼ԲĻƐ¶
        if (iter->second->group_id == -1) { //ºھ«׼ԲĻµépةΪ-1
            *gid = -99;
            return 1;
        } else if (iter->second->group_id > 0 && iter->second->group_id <= (int)str_servers.size()) {
	        *gid = iter->second->group_id; //group_id´ѱ¿ªʼ
        }
    }
    if (head_flag == 1) {
        int ret = in_head(str_domain,len,gid);    
        if( 1 == ret ) {
            return 3;
        }
    }
    if (rear_flag == 1) {
        int ret1 = in_rear(str_domain,gid);
        if( 1 == ret1 ) {
            return 5;
        }
    }
    return 0;		
}

int dns_conf::in_rear(char* str_domain, int* gid) {
    char* tmp_addr = str_domain;
    char tmp_char;
    int point_pos = 0;
    while (*tmp_addr != '\0' && point_pos < max_pointes) {
        if (*tmp_addr == '.') {
            point_pos++;
            tmp_addr += 1;
            tmp_char = *tmp_addr;  
            *tmp_addr = '\0';
            __gnu_cxx::hash_map<char*, ip_obj*,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator iter;
            if( ( iter = str_rear_hash.find(str_domain)) != str_rear_hash.end() ) {
                if (iter->second->group_id == -1) {
                    *tmp_addr = tmp_char;		
                    *gid = -99;
                    return 1;	
                } else if (iter->second->group_id > 0 && iter->second->group_id <= (int)str_servers.size()) {
                    *tmp_addr = tmp_char;		
		            *gid = iter->second->group_id;
                }
            }
            *tmp_addr = tmp_char;		
        }
        tmp_addr += 1;
    }
    return 0;	
}

int dns_conf::in_head(char* str_domain,int len, int* gid) {
    char* tmp_addr;
    int point_pos = 0;
    tmp_addr = str_domain + len -1;
    while(tmp_addr != str_domain && point_pos < max_pointes) {
        tmp_addr -= 1;
        if ( *tmp_addr == '.') {
            point_pos++;
            __gnu_cxx::hash_map<char*, ip_obj*,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator iter;
            if( ( iter = str_head_hash.find(tmp_addr)) != str_head_hash.end() ) {
                if (iter->second->group_id == -1) {
                    *gid = -99;
                    return 1;				
                } else if (iter->second->group_id > 0 && iter->second->group_id <= (int)str_servers.size()) {
		            *gid = iter->second->group_id;
                }
            }
        }
    }
    /*
    __gnu_cxx::hash_map<char*, ip_obj*,__gnu_cxx::hash<char*>,hm_strcmp1>::iterator w_iter;
    if( ( w_iter = str_white_head_hash.find((char*)".")) != str_white_head_hash.end() ) {
            *gid = w_iter->second->group_id;
    }
    */
    return 0;	
}
//我的配置规则是 比如白名单 *.com 这个*可以带有很多点 也就是说*.com包含了世界上所有com顶级域名
//那么这样的话会有一个问题比如 *.xigua.com 确实配了 但不想服务诸如ad1.*.xigua.com sport.*.xigua.com这种 
//
