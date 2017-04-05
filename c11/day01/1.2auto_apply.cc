#include<map>
#include<string>
#include<iostream>
#include<unordered_map>

int main1()
{
	std::map<int, std::string> map1;
	for(int i=0; i<5; i++)
	{
		map1.insert(std::make_pair(i,"1"));
	}
	//std::map<int,std::string>::iterator it = map1.begin
	//有了auto就不用再写很长的it了
	for(auto it = map1.begin(); it != map1.end(); ++it)
	{
		std::cout<<it->second<<std::endl;	
	}
	return 0;
}
/*
int main()
{
	//std::unordered_multimap<int, int> map1;
	std::unordered_map<int, int> map1;
	for(int i=0; i<5; i++)
	{
		map1.insert(std::make_pair(i,"1"));
	}
	//定义返回值对组
	//std::pair<std::unordered_multimap<int,int>::iterator, std::unordered_multimap<int,int>::iterator >
	//range = map1.equal_range(2);
	auto range = map1.equal_range(key);
	
	return 0;
}
*/

