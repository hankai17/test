#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
using namespace std;

class Person{
public:
	Person(){}
	Person(const char* name,int age){
		memset(mName, 0, 64);
		strcpy(mName, name);
		mAge = age;
	}
	void ShowPerson(){
		cout << "Name:" << mName << " Age:" << mAge << endl;
	}
public:
	char mName[64];
	int mAge;
};

int main(){

	//创建两个对象
	Person p1("John", 20);
	Person p2("Obama", 22);

	//创建二进制文件输出流
	ofstream osm("./obj.txt", ios::out | ios::binary | ios::trunc);
	if (!osm){
		cout << "./obj.txt" << "打开文件失败!" << endl;
	}

	//以二进制方式写文件
	osm.write((const char*)&p1, sizeof(Person));
	osm.write((const char*)&p2, sizeof(Person));

	//关闭文件
	osm.close();

	//创建二进制文件输入流
	ifstream ism("./obj.txt",ios::in | ios::binary);
	if (!ism){
		cout << "文件打开失败!" << endl;
	}
	
	//开辟两个空间，用来存储从文件读取的对象
	Person p3;
	Person p4;

	ism.read((char*)&p3, sizeof(Person));
	ism.read((char*)&p4, sizeof(Person));

	p3.ShowPerson();
	p4.ShowPerson();

	//文件关闭
	ism.close();

	system("pause");
	return EXIT_SUCCESS;
}