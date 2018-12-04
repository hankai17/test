#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<list>
#include<vector>
using namespace std;

//1. 初始化
/*
list<T> lstT;//list采用采用模板类实现,对象的默认构造形式：
list(beg,end);//构造函数将[beg, end)区间中的元素拷贝给本身。
list(n,elem);//构造函数将n个elem拷贝给本身。
list(const list &lst);//拷贝构造函数。
*/

void test01(){

	list<int> list1; //无参构造
	list<int> list2(10, 10);
	list<int> list3(list2.begin(), list2.end());
	list3.push_back(100);


	//正向打印
	for (list<int>::iterator it = list3.begin(); it != list3.end(); it ++){
		cout << *it << " ";
	}
	cout << endl;

	//逆向打印
	for (list<int>::reverse_iterator rit = list3.rbegin(); rit != list3.rend();rit ++){
		cout << *rit << " ";
	}
	cout << endl;
	
}

//2. 插入和删除
/*
push_back(elem);//在容器尾部加入一个元素
pop_back();//删除容器中最后一个元素
push_front(elem);//在容器开头插入一个元素
pop_front();//从容器开头移除第一个元素
insert(pos,elem);//在pos位置插elem元素的拷贝，返回新数据的位置。
insert(pos,n,elem);//在pos位置插入n个elem数据，无返回值。
insert(pos,beg,end);//在pos位置插入[beg,end)区间的数据，无返回值。
clear();//移除容器的所有数据
erase(beg,end);//删除[beg,end)区间的数据，返回下一个数据的位置。
erase(pos);//删除pos位置的数据，返回下一个数据的位置。
remove(elem);//删除容器中所有与elem值匹配的元素。
*/


void PrintList(const list<int>& L){

	for (list<int>::const_iterator it = L.begin(); it != L.end(); it++){
		cout << *it << " ";
	}
	cout << endl;
}

void test02(){
	
	list<int> L;
	L.push_back(10);
	L.push_back(20);
	L.push_front(100);
	L.push_back(20);
	L.push_front(200);
	L.push_back(20);

	list<int>::iterator it = L.begin();
	it++;
	L.insert(it,300);

	//200 300 100 10 20
	PrintList(L);

	//指定位置删除  
	it = L.begin();
	it++;
	L.erase(it);
	PrintList(L);

	//按值删除 把所有值相同的元素全部删除
	L.remove(20);
	PrintList(L);
}

//3. 大小操作
/*
size();//返回容器中元素的个数
empty();//判断容器是否为空
resize(num);//重新指定容器的长度为num，
若容器变长，则以默认值填充新位置。
如果容器变短，则末尾超出容器长度的元素被删除。
resize(num, elem);//重新指定容器的长度为num，
若容器变长，则以elem值填充新位置。
如果容器变短，则末尾超出容器长度的元素被删除。
*/

void test03(){
	
	list<int> L;
	L.push_back(10);
	L.push_back(20);
	L.push_front(100);
	L.push_back(20);
	L.push_front(200);
	L.push_back(20);

	cout << "size:" << L.size() << endl;
	if (L.empty()){
		cout << "空！" << endl;
	}
	else{
		cout << "不空!" << endl;
	}

	L.resize(10);
	PrintList(L);
	L.resize(3);
	PrintList(L);
}

//4. 赋值操作
/*
assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身。
assign(n, elem);//将n个elem拷贝赋值给本身。
list& operator=(const list &lst);//重载等号操作符
swap(lst);//将lst与本身的元素互换。
*/
void test04(){

	list<int> L1;
	list<int> L2;
	L1.assign(10, 1);
	L2.assign(10, 2);

	PrintList(L1);
	PrintList(L2);

	//交换之后
	cout << "交换之后:" << endl;
	L1.swap(L2);
	PrintList(L1);
	PrintList(L2);
}

//5. 存取和反转 排序
/*
front();//返回第一个元素。
back();//返回最后一个元素。
reverse();//反转链表，比如lst包含1,3,5元素，运行此方法后，lst就包含5,3,1元素。
sort(); //list排序
*/


bool MyCompare(int v1,int v2){
	return v1 > v2;
}

void test05(){
	
	list<int> L;
	L.push_back(90);
	L.push_back(30);
	L.push_back(20);
	L.push_back(70);

	cout << "front:" << L.front() << endl;
	cout << "back:" << L.back() << endl;

	//反转容器的元素
	PrintList(L);
	L.reverse();
	PrintList(L);

	//排序
	L.sort(); //默认的排序规则 从小到大
	PrintList(L);
	L.sort(MyCompare); //指定规则，从大到小
	PrintList(L);
}

class Person{
public:
	Person(int id,int age){
		mID = id;
		mAge = age;
	}
	bool operator==(const Person& p){
		if (this->mID == p.mID && this->mAge == p.mAge){
			return true;
		}
		return false;
	}
public:
	int mID;
	int mAge;
};


bool ComparePerson(Person& p1, Person& p2){
	
	if (p1.mAge == p2.mAge){
		return p1.mID < p2.mID;
	}
	else if (p1.mAge > p2.mAge){
		return true;
	}

	return false;
}

void test06(){
	
	list<Person> lPerson;

	Person p1(1, 10);
	Person p2(2, 10);
	Person p3(3, 30);
	Person p4(4, 40);

	lPerson.push_back(p1);
	lPerson.push_back(p4);
	lPerson.push_back(p2);
	lPerson.push_back(p3);

	//删除对象
	lPerson.remove(p2);

	cout << "remove删除对象 start" << endl;
	for (list<Person>::iterator it = lPerson.begin(); it != lPerson.end(); it ++){
		cout << it->mAge << " " << it->mID << endl;
	}
	cout << "remove删除对象 end" << endl;

	lPerson.sort(ComparePerson);

	for (list<Person>::iterator it = lPerson.begin(); it != lPerson.end(); it ++){
		cout << "Age:" << it->mAge << " ID:" << it->mID << endl;
	}
}


int main(){

	//test01();
	//test02();
	//test03();
	//test04();
	//test05();
	test06();

	system("pause");
	return EXIT_SUCCESS;
}