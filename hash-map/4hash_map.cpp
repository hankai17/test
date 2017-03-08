#include<iostream>
#include<ext/hash_map>
#include<vector>
#include<sstream>   
#include<fstream>
#include<string>

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

    /*
    bool operator==(const std::string &str,const std::string &str1) 
    {
        return str==str1;
    }
    bool operator!=(const std::string &str,const std::string &str1) 
    {
        return !(str==str1);
    }
    */
};

__gnu_cxx::hash_map<std::string, std::vector<std::string>*,str_hash,hm_strcmp> domain_hash;
__gnu_cxx::hash_map<std::string, std::vector<std::string>*,str_hash,hm_strcmp> head_hash;
__gnu_cxx::hash_map<std::string, std::vector<std::string>*,str_hash,hm_strcmp> rear_hash;

class dns_conf {
public:
	void open_conf_file();
	void close_conf_file();
	int parse_conf();
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
	cfilename = "./dns_conf.file";
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
	conf.parse_conf();

	//std::hash_map<std::string, std::vector<std::string>*> domain_hash;
    
	//__gnu_cxx::hash_map<std::string, std::vector<std::string>*>::iterator it = head_hash.find(".163.com");
	__gnu_cxx::hash_map<std::string, std::vector<std::string>*>::iterator it; 
    std::cout<<head_hash.find(".163.com")->second[0][0]<<std::endl;;
	if (head_hash.find(".163.com") != head_hash.end())
	{
		//std::cout << "找到了，组ip是：" << (it->second)[0][0] << "和" << (it->second)[0][1] << std::endl;
		//std::cout << "找到了，组ip是：" << (it->second)[0][0]<< std::endl;
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


