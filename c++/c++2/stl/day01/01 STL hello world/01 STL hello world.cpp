#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>  //引入容器头文件
#include<string>
#include<algorithm> //算法头文件
using namespace std;

void MyPrint(int val){
	cout << val << " ";
}

//1. 容器存放基础数据类型
void test01(){

	//创建vector容器对象，并且通过模板参数指定容器中存放的数据的类型
	vector<int> v;
	//向容器中放数据
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);

	//每一个容器都有自己的迭代器，迭代器是用来遍历容器中的元素
	//如何拿到vector<int>迭代器

	//v.begin()返回迭代器，这个迭代器指向容器中第一个数据
	//v.end()返回迭代器，这个迭代器指向容器元素的最后一个元素的下一个位置
	//vector<int>::iterator 拿到vector<int>这种容器的迭代器类型
	vector<int>::iterator pBegin = v.begin(); 
	vector<int>::iterator pEnd = v.end(); 
	//比如容器的类型是 vector<string> 那么可以通过vector<string>::iterator拿到迭代器类型

	while (pBegin != pEnd){
		cout << *pBegin << " ";
		//MyPrint(*pBegin);
		pBegin++;
	}
	cout << endl;

	//pBegin = v.begin();

	for (vector<int>::iterator it = v.begin(); it != v.end(); it ++){
		cout << *it << " ";
	}
	cout << endl;

	//使用算法遍历
	for_each(v.begin(), v.end(), MyPrint);
}


//2. 自定义数据类型
class Person{
public:
	Person(string name,int age){
		mName = name;
		mAge = age;
	}
public:
	string mName;
	int mAge;
};
void test02(){

	vector<Person> v;

	//创建数据
	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);
	Person p5("eee", 50);
	
	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);
	v.push_back(p5);

	for (vector<Person>::iterator it = v.begin(); it != v.end(); it ++){
		cout << "Name:" << (*it).mName << " Age:" << (*it).mAge << endl;
	}
}

//3. 放对象指针
void test03(){
	
	vector<Person*> v;

	//创建数据
	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);
	Person p5("eee", 50);

	v.push_back(&p1);
	v.push_back(&p2);
	v.push_back(&p3);
	v.push_back(&p4);
	v.push_back(&p5);

	for (vector<Person*>::iterator it = v.begin(); it != v.end(); it ++){
		cout << "Name:" << (*it)->mName << " Age:" << (*it)->mAge << endl;
	}
}

//4. 容器嵌套容器
void test04(){
	
	vector< vector<int> >  v;

	vector<int> v1;
	vector<int> v2;
	vector<int> v3;
	vector<int> v4;

	for (int i = 0; i < 4;i++){
		v1.push_back(i + 1);
		v2.push_back(i + 2);
		v3.push_back(i + 3);
		v4.push_back(i + 4);
	}

	//将容器元素插入到vector v中
	v.push_back(v1);
	v.push_back(v2);
	v.push_back(v3);
	v.push_back(v4);


	for (vector<vector<int>>::iterator it = v.begin(); it != v.end(); it ++){
		
		for (vector<int>::iterator vit = (*it).begin(); vit != (*it).end(); vit++){
			cout << *vit << " ";
		}
		cout << endl;

	}

}

int main(){

	//test01();
	//test02();
	//test03();
	test04();

	system("pause");
	return EXIT_SUCCESS;
}