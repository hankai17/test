#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class Person{
public:

	//构造函数的名称和类名相同
	//构造函数没有返回值，也不能写void

	Person(char* name, int age){
		cout << "构造函数!" << endl;
		pName = (char*)malloc(strlen(name) + 1);
		strcpy(pName, name);
		mAge = age;
	}
#if 0
	void Init(char* name,int age){
		pName = (char*)malloc(strlen(name) + 1);
		strcpy(pName, name);
		mAge = age;
	}
#endif

	void ShowPerson(){
		cout << "Name:" << pName << " Age:" << mAge << endl;
	}

	//析构函数和类名名字也一样，在名字前面加上~,没有返回值，不能写void ，析构函数不能有参数
	~Person(){
		cout << "析构函数！" << endl;
		if (NULL != pName){
			free(pName);
		}
	}
#if 0
	void Clean(){
		if (NULL != pName){
			free(pName);
		}
	}
#endif
private:
	char* pName;
	int mAge;
};

void test01(){

	//对象创建必须有个初始化  再来个清理函数
	//c++提供了构造函数和析构函数
	Person person("Smith",20);
	//记着调用初始化函数
	//person.Init("Smith", 20);
	person.ShowPerson();
	//person.Clean();

	//构造函数在内存开辟之后，调用构造函数
	//析构函数在内存释放之前，调用析构函数
	//构造函数和析构函数都是由编译器自动调用
}

int main(){

	test01();


	system("pause");
	return EXIT_SUCCESS;
}