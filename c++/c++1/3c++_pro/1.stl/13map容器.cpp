#include <iostream>
using namespace std;
#include "map"
#include"string"

void main1101()//基本操作
{
	map<int, string> map1;
	map1.insert(pair<int, string>(1, "teacher1"));
	map1.insert(pair<int, string>(2, "teacher2"));

	map1.insert(make_pair(3, "teacher3"));
	map1.insert(make_pair(4, "teacher4"));
	
	map1.insert(map<int, string>::value_type(5, "teacher5"));
	map1.insert(map<int, string>::value_type(6, "teacher6"));

	map1[7] = "teacher7";
	map1[8] = "teacher8";

	//遍历 迭代器是最快的方法
	for (map<int, string>::iterator it = map1.begin(); it != map1.end(); it++)
	{
		cout << it->first << "\t" << it->second << endl;

	}
	//删除
	while (!map1.empty())
	{
		map<int, string>::iterator it = map1.begin();
		cout << it->first << "\t" << it->second << endl;
		map1.erase(it);
	}


}
//插入的四种方法异同
//前三种 insert的返回的是pair<iterator,bool> 兼职对
void main1102()
{
	map<int, string> map1;
	pair<map<int, string>::iterator, bool> mypair1=map1.insert(pair<int, string>(1, "teacher1"));
	map1.insert(pair<int, string>(2, "teacher2"));

	pair<map<int, string>::iterator, bool> mypair3=map1.insert(make_pair(3, "teacher3"));
	map1.insert(make_pair(4, "teacher4"));

	pair<map<int, string>::iterator, bool> mypair5=map1.insert(map<int, string>::value_type(5, "teacher5"));
	if (mypair5.second != true)
	{
		cout << "key5插入不成功" << endl;
	}
	else
	{
		cout << mypair5.first->first <<"\t"<<mypair5.first->second<<endl;
	}
	pair<map<int, string>::iterator, bool> mypair6 = map1.insert(map<int, string>::value_type(5, "teacher6"));
	if (mypair6.second != true)
	{
		cout << "key6插入不成功" << endl;
	}
	else
	{
		cout << mypair6.first->first << "\t" << mypair6.first->second << endl;
	}
	map1[7] = "teacher7";
	map1[7] = "teacher8";//若以上3个方法 key已经存在则报错 ，若用这种方法不会错

	//遍历 迭代器是最快的方法
	for (map<int, string>::iterator it = map1.begin(); it != map1.end(); it++)
	{
		cout << it->first << "\t" << it->second << endl;

	}
}

void main1103()
{
	map<int, string> map1;
	map1.insert(pair<int, string>(1, "teacher1"));
	map1.insert(pair<int, string>(2, "teacher2"));

	map1.insert(make_pair(3, "teacher3"));
	map1.insert(make_pair(4, "teacher4"));

	map1.insert(map<int, string>::value_type(5, "teacher5"));
	map1.insert(map<int, string>::value_type(6, "teacher6"));

	map1[7] = "teacher7";
	map1[8] = "teacher8";

	//遍历 迭代器是最快的方法
	for (map<int, string>::iterator it = map1.begin(); it != map1.end(); it++)
	{
		cout << it->first << "\t" << it->second << endl;

	}
	//map查找
	map<int, string>::iterator it2=map1.find(100);//返回的是迭代器位置
	if (it2 == map1.end())
	{
		cout << "key 100不存在" << endl;
	}
	else
	{
		cout << it2->first << "\t" << it2->second << endl;
	}

	//equal_range异常处理 比较重要！
	pair<map<int, string>::iterator, map<int, string>::iterator > mypair= map1.equal_range(5);//返回两个迭代器 形成一个pair
	//第一个迭代器>5的位置 第二个是=5的位置
	if (mypair.first == map1.end())
	{
		cout << "第一个迭代器>=5的位置不存在" << endl;
	}
	else
	{
		cout << mypair.first->first << "\t" << mypair.first->second << endl;
	}

	if (mypair.second==map1.end())
	{
		cout << "第二个迭代器>5的位置不存在" << endl;
	}
	else
	{
		cout << mypair.second->first << "\t" << mypair.second->second << endl;
	}


}

void main()
{
	main1103();
	system("pause");
	return;
}