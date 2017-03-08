#include<iostream>
#include<ext/hash_map>
#include<vector>
#include<sstream>   
#include<fstream>
#include<string>
//#include<hash>

std::vector<std::vector<std::string>*>servers;
std::vector<std::string>ip;
std::vector<std::string>domain;

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

//std::hash<std::string> domain_black_hash;
__gnu_cxx::hash_map<std::string,int,str_hash,hm_strcmp> domain_black_hash;
__gnu_cxx::hash_map<std::string,int,str_hash,hm_strcmp> black_head_hash;
__gnu_cxx::hash_map<std::string,int,str_hash,hm_strcmp> black_rear_hash;
//this is error __gnu_cxx::hash_map<std::string, str_hash,hm_strcmp> domain_black_hash;
__gnu_cxx::hash_map<std::string, std::vector<std::string>*,str_hash,hm_strcmp> domain_hash;
__gnu_cxx::hash_map<std::string, std::vector<std::string>*,str_hash,hm_strcmp> head_hash;
__gnu_cxx::hash_map<std::string, std::vector<std::string>*,str_hash,hm_strcmp> rear_hash;

class dns_conf {
public:
	void open_conf_file();
	void close_conf_file();
    int parse_white_conf();
	int parse_black_conf();
public:
	std::string      cfilename;
	std::ifstream    cfilestream;
	int          lineNum;
	int			block_index;
	int  unkown_block_start;
};
/*传入域名，传出点数，传出标记
标记：返回1前缀 返回2后缀 返回0正常
返回值：*的位置
*/
int posAfterStar(const char *str, int &point_num, char &wildcard_flash) {
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
	//cfilename = "./dns_conf.file";
	cfilename = "./domain-white-list";
	try
	{
		cfilestream.open(cfilename);
		if (!cfilestream.is_open()) {
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
	if (cfilestream.is_open()) {
		cfilestream.close();
	}
}

int dns_conf::parse_black_conf()
{
    char ch;
    int pn;
    char flag='0';
    int line_num=0;
	std::ostringstream _strline;
	while (cfilestream.get(ch)) {
        //std::cout<<"ch:"<<ch <<"  ascii:"<<(int)ch<<std::endl;    
        if(ch == '\n')
        {
            line_num++;
			posAfterStar(_strline.str().c_str(), pn, flag);
            std::cout<<"domain:"<<_strline.str();
            std::cout<<"   flag:"<<flag<<std::endl;
            
           if(flag == '0') 
           {
			    domain_black_hash.insert(__gnu_cxx::hash_map<std::string, int>::value_type(_strline.str(), line_num));
           }
           else if(flag=='1')
           {
			    black_head_hash.insert(__gnu_cxx::hash_map<std::string, int>::value_type(_strline.str().erase(0,1), line_num));
           }
           else if(flag=='2')
           {
			    black_rear_hash.insert(__gnu_cxx::hash_map<std::string, int>::value_type(_strline.str().substr(0, _strline.str().find("*")), line_num));
           }
           flag='0'; 
            _strline.str("");

            continue;
        }
	    _strline << ch;
    }
}

int dns_conf::parse_white_conf()
{
	char ch;
	int block_num = 0;
	int block_end_num = 0;
	int ref_num = 0;
	std::ostringstream _strline;
	int new_flag = 0;
    int group_flag = 1;//flag=0时 在一个组
	std::vector<std::string>* p=NULL;//存ip
    int group=0;//第几组

    int servers_flag=0;//servers开始
    int domains_flag=0;//domains开始

	while (cfilestream.get(ch)) {
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
            group++;
            //std::cout<<"组名:"<<_strline.str()<<"  第"<<group<<"组"<<std::endl;
            std::cout<<"  第"<<group<<"组"<<std::endl;
           new_flag=0; 
            group_flag=0;
            _strline.str("");
        }
        if(ch == '=')//一个小组开始 , 假设小组里不再嵌套
        {
            if(_strline.str().find("servers",0) != -1)
            {
                servers_flag = 1;
            }else if(_strline.str().find("domains",0) != -1)
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
				std::vector<std::string>* new_p = new std::vector < std::string >;
				p = new_p;
				servers.push_back(p);
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
                std::cout<<"ip: "<<_strline.str()<<std::endl;
				(*p).push_back(_strline.str());
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
					domain_hash.insert(__gnu_cxx::hash_map<std::string, std::vector<std::string>*>::value_type(_strline.str(), p));
				}
				else if (flag == '1')
				{
					//前缀  //需要去掉前缀的* 
					head_hash.insert(__gnu_cxx::hash_map<std::string, std::vector<std::string>*>::value_type(_strline.str().erase(0,1), p));
				}
				else if (flag == '2')
				{	
					//后缀  //需要去掉后缀的*
					//std::cout << "后缀：" << _strline.str().substr(0, _strline.str().find("*")) << std::endl;
					rear_hash.insert(__gnu_cxx::hash_map<std::string, std::vector<std::string>*>::value_type(_strline.str().substr(0, _strline.str().find("*")), p));
				}
				domain.push_back(_strline.str());
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

int main()
{
	dns_conf conf;
	conf.open_conf_file();
	int ret=conf.parse_white_conf();
    if( 1 == ret)
    {
        std::cout<<"parse err"<<std::endl;
    }
    else if( 0 == ret)
    {
        std::cout<<"parse success"<<std::endl;
    }
    std::cout<<"servers count: " <<servers.size()<<std::endl;
//std::vector<std::vector<std::string>*>servers;
    std::cout<<(*servers[1])[0]<<std::endl;
    std::cout<<(*servers[1])[1]<<std::endl;

    std::cout<<"------------------------------"<<std::endl;
    //std::cout<<domain_black_hash.size()<<std::endl;
    //std::cout<<black_head_hash.size()<<std::endl;
    //std::cout<<black_rear_hash.size()<<std::endl;

    std::cout<<domain_hash.size()<<std::endl;
    std::cout<<head_hash.size()<<std::endl;
    std::cout<<rear_hash.size()<<std::endl;

    std::cout<<"------------------------------"<<std::endl;
    //clear room
    /*
    domain_hash.clear();
    head_hash.clear();
    rear_hash.clear();
    */

    /*
    domain_hash.erase(domain_hash.begin(),domain_hash.end());
    head_hash.erase(head_hash.begin(),head_hash.end());
    rear_hash.erase(rear_hash.begin(),rear_hash.end());
    */
    

   __gnu_cxx::hash_map<std::string,std::vector<std::string>*,str_hash,hm_strcmp>().swap(domain_hash); 
   __gnu_cxx::hash_map<std::string,std::vector<std::string>*,str_hash,hm_strcmp>().swap(head_hash); 
   __gnu_cxx::hash_map<std::string,std::vector<std::string>*,str_hash,hm_strcmp>().swap(rear_hash); 


    std::cout<<domain_hash.size()<<std::endl;
    std::cout<<head_hash.size()<<std::endl;
    std::cout<<rear_hash.size()<<std::endl;







    return 0;
}

