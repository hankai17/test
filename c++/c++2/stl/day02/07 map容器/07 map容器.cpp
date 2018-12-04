#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<map>
#include<string>
using namespace std;

//1. 初始化
/*
map<T1, T2> mapTT;//map默认构造函数:
map(const map &mp);//拷贝构造函数
map所有元素都是一个对组
*/
void test01(){
	map<int, int> m;
	//第一种插入方式
	m.insert(pair<int,int>(1,1));
	//第二种插入方式
	m.insert(make_pair(2, 2));
	//第三种插入方式
	m.insert(map<int, int>::value_type(3, 3));
	//第四种插入方式
	m[4] = 4;  //map只是重载了[]，但是并不支持随机访问

	for (map<int, int>::iterator it = m.begin(); it != m.end();it ++){
		cout << "key:" << it->first << " value:" << it->second << endl;
	}

	//如果通过[]方式访问一个不存在的key，那么map会创建一个指定key的元素，value默认值，插入到map中
	cout << m[5] << endl;

	for (map<int, int>::iterator it = m.begin(); it != m.end(); it++){
		cout << "key:" << it->first << " value:" << it->second << endl;
	}
}

//2. 删除
void test02(){

	map<int, int> m;
	//第一种插入方式
	m.insert(pair<int, int>(1, 1));
	//第二种插入方式
	m.insert(make_pair(2, 2));
	//第三种插入方式
	m.insert(map<int, int>::value_type(3, 3));

	for (map<int, int>::iterator it = m.begin(); it != m.end(); it++){
		cout << "key:" << it->first << " value:" << it->second << endl;
	}

	cout << "------------" << endl;
	m.erase(2);

	for (map<int, int>::iterator it = m.begin(); it != m.end(); it++){
		cout << "key:" << it->first << " value:" << it->second << endl;
	}
}

//3. map查找
/*
find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；/若不存在，返回map.end();
count(keyElem);//返回容器中key为keyElem的对组个数。对map来说，要么是0，要么是1。对multimap来说，值可能大于1。
lower_bound(keyElem);//返回第一个key<=keyElem元素的迭代器。
upper_bound(keyElem);//返回第一个key>keyElem元素的迭代器。
equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。
*/

void test03(){
	
	map<int, int> m;
	//第一种插入方式
	m.insert(pair<int, int>(1, 1));
	//第二种插入方式
	m.insert(make_pair(2, 2));
	//第三种插入方式
	m.insert(map<int, int>::value_type(3, 3));


	map<int, int>::iterator it =  m.find(2);
	if (it == m.end()){
		cout << "找不到！" << endl;
	}
	else{
		cout << "找到：" << it->first << ":" << it->second << endl;
	}

	cout << "----------------" << endl;

	//lower_bound upper_bound equal_range
	it = m.lower_bound(2);
	if (it == m.end()){
		cout << "找不到！" << endl;
	}
	else{
		cout << "找到：" << it->first << ":" << it->second << endl;
	}

	it = m.upper_bound(2);
	if (it == m.end()){
		cout << "找不到！" << endl;
	}
	else{
		cout << "找到：" << it->first << ":" << it->second << endl;
	}

	cout << "----------------" << endl;

	pair< map<int, int>::iterator, map<int, int>::iterator> ret = m.equal_range(2);
	if (ret.first == m.end()){
		cout << "没有找到!" << endl;
	}
	else{
		cout << "找到:" << ret.first->first << ":" << ret.first->second << endl;
	}

	if (ret.second == m.end()){
		cout << "没有找到!" << endl;
	}
	else{
		cout << "找到:" << ret.second->first << ":" << ret.second->second << endl;
	}
}

//4. 指定map的排序规则

class MyCompare{
public:
	bool operator()(int v1,int v2){
		return v1 > v2;
	}
};

void test04(){

	//默认从小到大排序
	map<int, int, MyCompare> m;

	m.insert(make_pair(7, 2));
	m.insert(make_pair(2, 4));
	m.insert(make_pair(3, 9));
	m.insert(make_pair(8, 1));
	m.insert(make_pair(9, 9));

	for (map<int, int, MyCompare>::iterator it = m.begin(); it != m.end(); it++){
		cout << "key:" << it->first << " value:" << it->second << endl;
	}
}



int main(){

	//test01();
	//test02();
	//test03();
	test04();

	MyCompare compare;
	compare(10,20);

	system("pause");
	return EXIT_SUCCESS;
}