#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<set>
using namespace std;

//1. 初始化
/*
3.7.2.1 set构造函数
set<T> st;//set默认构造函数：
mulitset<T> mst; //multiset默认构造函数:
set(const set &st);//拷贝构造函数

3.7.2.2 set赋值操作
set& operator=(const set &st);//重载等号操作符
swap(st);//交换两个集合容器

3.7.2.3 set大小操作
size();//返回容器中元素的数目
empty();//判断容器是否为空

3.7.2.4 set插入和删除操作
insert(elem);//在容器中插入元素。
clear();//清除所有元素
erase(pos);//删除pos迭代器所指的元素，返回下一个元素的迭代器。
erase(beg, end);//删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
erase(elem);//删除容器中值为elem的元素。
*/


void PrintSet(set<int>& s){

	for (set<int>::iterator it = s.begin(); it != s.end(); it ++){
		cout << *it << " ";
	}
	cout << endl;
}

void test01(){
	
	set<int> s1;
	set<int> s2;
	s1.insert(10);
	s1.insert(90);
	s1.insert(60);
	s1.insert(30);
	s1.insert(50);

	s2.insert(100);
	s2.insert(90);
	s2.insert(60);
	s2.insert(20);
	s2.insert(50);

	//根据值删除
	//s.erase(60);
	//s.erase(s.begin());

	PrintSet(s1);
	PrintSet(s2);

	s1.swap(s2);

	PrintSet(s1);
	PrintSet(s2);
}

//2. 查找操作
/*
find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；若不存在，返回set.end();
count(key);//查找键key的元素个数
lower_bound(keyElem);//返回第一个key>=keyElem元素的迭代器。
upper_bound(keyElem);//返回第一个key>keyElem元素的迭代器。
equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。
*/
void test02(){

	set<int> s1;
	s1.insert(10);
	s1.insert(90);
	s1.insert(60);
	s1.insert(30);
	s1.insert(50);

	//find返回迭代器，如果没有找到返回end迭代器
	set<int>::iterator pos = s1.find(900);
	if (pos == s1.end()){
		cout << "No 找到!" << endl;
	}
	else{
		cout << "找到:" << *pos << endl;
	}

	//统计元素个数
	cout << s1.count(60) << endl;

	PrintSet(s1);

	//lower_bound  upper_bound
	pos =  s1.lower_bound(30);
	if (pos == s1.end()){
		cout << "No 找到!" << endl;
	}
	else{
		cout << "找到:" << *pos << endl;
	}

	pos = s1.upper_bound(30);
	if (pos == s1.end()){
		cout << "No 找到!" << endl;
	}
	else{
		cout << "找到:" << *pos << endl;
	}

	//equal_range 返回lower_bound 和 upper_bound的迭代器
	pair<set<int>::iterator,set<int>::iterator>  it = s1.equal_range(30);
	if (it.first == s1.end()){
		cout << "没有找到!" << endl;
	}
	else{
		cout << "找到:" << *(it.first) << endl;
	}

	if (it.second == s1.end()){
		cout << "没有找到!" << endl;
	}
	else{
		cout << "找到:" << *(it.second) << endl;
	}
}

//3. set、multiset区别
void test03(){

	set<int> s;
	pair<set<int>::iterator, bool>  ret = s.insert(10);
	if (ret.second){
		cout << "第一次插入成功!" << endl;
	}
	else{
		cout << "第一次插入失败!" << endl;
	}

	ret = s.insert(10);
	if (ret.second){
		cout << "第二次插入成功!" << endl;
	}
	else{
		cout << "第二次插入失败!" << endl;
	}
	

	//multiset
	multiset<int> ms;
	ms.insert(10);
	ms.insert(10);

	for (multiset<int>::iterator it = ms.begin(); it != ms.end(); it ++){
		cout << *it << " ";
	}
	cout << endl;
}

//4. 指定set容器的排序规则

//写个函数对象
class MyCompare{
public:
	bool operator()(int v1,int v2){
		return v1 > v2;
	}
};
void test04(){
	

	set<int, MyCompare> s1;
	s1.insert(10);
	s1.insert(90);
	s1.insert(60);
	s1.insert(30);
	s1.insert(50);


	//默认从小到大
	//PrintSet(s1);

	for (set<int, MyCompare>::iterator it = s1.begin(); it != s1.end(); it ++){
		cout << *it << " ";
	}
	cout << endl;
}

//作业:set容器中添加对象




int main(){

	//test01();
	//test02();
	//test03();
	test04();

	system("pause");
	return EXIT_SUCCESS;
}