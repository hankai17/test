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
	int parse_conf();
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
	cfilename = "./domain-black-list";
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

int dns_conf::parse_conf()
{
	char ch;
	int block_begin_num = 0;
	int block_end_num = 0;
	int ref_num = 0;
	std::ostringstream _strline;
	int new_flag = 0;
	std::vector<std::string>* p=NULL;
	while (cfilestream.get(ch)) {
		if (ch == '{')
			block_begin_num++;
		if (ch == '}')
			block_end_num++;
		if (block_begin_num == 2)//{"111.13.111.233","192.168.1.1"}
			//一个servers组开始了，new出vector<string>,地址插入全局中
			//ip数据插入到vector<string>中
		{
			if (new_flag == 0)
			{
				std::vector<std::string>* new_p = new std::vector < std::string >;
				p = new_p;
				servers.push_back(p);
				new_flag = 1;
			}
			if (ch == '"')
			{
				ref_num++;
				continue;
			}
			if (ref_num == 1)
				_strline << ch;
			if (ref_num == 2)
			{
				(*p).push_back(_strline.str());
				_strline.str("");
				ref_num = 0;
			}
		}

		if (block_begin_num == 3)//处理域名
		{
			if (ch == '"')
			{
				ref_num++;
				continue;
			}
			if (ref_num == 1)
				_strline << ch;
			if (ref_num == 2)
			{
				int pn; char flag = '0';
				//_strline << '.';
				posAfterStar(_strline.str().c_str(), pn, flag);
				if (flag == '0')
				{
					//普通
					//std::cout << "普通的：" << _strline.str() << std::endl;
					//std::hash_map<std::string, std::vector<std::string>*> domain_hash;
					//head_hash.insert(std::pair<std::string, std::vector<std::string>*>(_strline.str().c_str(), p));
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
		}
		if (block_end_num == 3)
		{
			block_begin_num = 0;
			block_end_num = 0;
			ref_num = 0;
			new_flag = 0;
			_strline.str("");
		}
	}


	return 0;
}
int main()
{
	dns_conf conf;
	conf.open_conf_file();
	conf.parse_black_conf();


   if(black_head_hash.find(".sina.com")!=black_head_hash.end()) 
   {
		std::cout << "找到了" << std::endl;
   }
	else
	{
		std::cout << "找不到" << std::endl;
	}
   if(black_rear_hash.find("reg.")!=black_rear_hash.end()) 
   {
		std::cout << "找到了" << std::endl;
   }
	else
	{
		std::cout << "找不到" << std::endl;
	}
   


    return 0;
}

int main11()
{
	dns_conf conf;
	conf.open_conf_file();
	conf.parse_black_conf();
    //__gnu_cxx::hash_map<std::string, str_hash,hm_strcmp> domain_black_hash;
	//rear_hash.insert(__gnu_cxx::hash_map<std::string, std::vector<std::string>*>::value_type(_strline.str().substr(0, _strline.str().find("*")), p));
    //domain_black_hash.insert(__gnu_cxx::hash_map<std::string,str_hash,hm_strcmp>::value_type("hello"));
    std::string s1("hello");
	//std::hash_map<std::string, std::vector<std::string>*> domain_hash;
    //domain_black_hash.insert(s1); 
	//__gnu_cxx::hash_map<std::string, std::vector<std::string>*>::iterator it = head_hash.find(".163.com");
	__gnu_cxx::hash_map<std::string, std::vector<std::string>*>::iterator it; 
    std::cout<<head_hash.find(".163.com")->second[0][0]<<std::endl;;
	if (head_hash.find(".163.com") != head_hash.end())
	{
		//std::cout << "找到了，组ip是：" << (it->second)[0][0] << "和" << (it->second)[0][1] << std::endl;
		//std::cout << "找到了，组ip是：" << (it->second)[0][0]<< std::endl;
        std::cout<<"本ip组中的个数"<<(*(head_hash.find(".163.com")->second)).size()<<std::endl;
		std::cout << "找到了" << std::endl;
        
	}
	else
	{
		std::cout << "找不到" << std::endl;
	}
	return 0;
}


/*
int main2()
{
	dns_conf conf;
	conf.open_conf_file();
	conf.parse_conf();
	

	for (std::vector<std::vector<std::string>*>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		
		std::cout << "-----" << std::endl;
		for (std::vector<std::string>::iterator it1 = (*(*it)).begin(); it1 != (*(*it)).end(); it1++)
			std::cout << "ip " << (*it1) << std::endl;
	}

	for (std::vector<std::string>::iterator it = domain.begin(); it != domain.end(); it++)
	{
		std::cout << "domain " << *it << std::endl;
	}
	for (std::hash_map<std::string, std::vector<std::string>*>::iterator it = head_hash.begin(); it != head_hash.end(); it++)
	{
		std::cout << "head_hash:" << it->first << std::endl;
	}
	for (std::hash_map<std::string, std::vector<std::string>*>::iterator it = rear_hash.begin(); it != rear_hash.end(); it++)
	{
		std::cout << "rear_hash:" << it->first << std::endl;
	}

	std::cout << "*********************" << std::endl;
	//std::hash_map<std::string, std::vector<std::string>*> domain_hash;
	//std::hash_map<std::string, std::vector<std::string>*>::iterator it = domain_hash.begin();
	//std::cout << it->first << std::endl;
	
	std::cout <<domain_hash.size()<< std::endl;

	if (domain_hash.find("www.jd.com") != domain_hash.end())
	{
		std::cout << "找到了" << std::endl;
	}
	else
	{
		std::cout << "找不到" << std::endl;
	}

	system("pause");
	return 0;
}
*/


