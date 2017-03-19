#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

class Circle{
public:
	//设置半径
	void setR(int r){
		mR = r;
	}
	//计算并返回圆的周长
	double caculateL(){
		return 2 * 3.14 * mR;
	}
private:
	double mR; //圆的半径
};

void test01(){
	
	//实例化圆 创建圆的对象
	Circle c1;
	c1.setR(10);
	cout << "圆的周长：" << c1.caculateL() << endl;

	c1.setR(20);
	cout << "圆的周长：" << c1.caculateL() << endl;
}


class Student{
public:
    void setName(string name){
		mName = name;
	}
	void setID(int id){
		mID = id;
	}
	void ShowStudent(){
		cout << "Name:" << mName << " Age:" << mID << endl;
	}
private:
	string mName; //学生姓名
	int mID;
};




void test02(){
	Student s1;
	s1.setName("John");
	s1.setID(1001);
	s1.ShowStudent();
}

int main(){

	//test01();
	test02();

	system("pause");
	return EXIT_SUCCESS;
}