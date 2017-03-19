#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
using namespace std;

class myclass{
public:
	myclass(char* data){
		int len = strlen(data) + 1; //计算传进来的字符串长度
		this->data = new char[len];  //在堆区分配了len字节内存
		strcpy(this->data, data); //将数据拷贝到我们在堆分配的内存中
	}


	//增加拷贝构造函数
	myclass(const myclass& mc){
		int len = strlen(mc.data) + 1;
		this->data = new char[len];
		strcpy(this->data, mc.data);
	}
	//重载operator=操作符
	myclass& operator=(const myclass& mc){

		if (this->data != NULL){
			delete[] this->data;
			this->data = NULL;
		}

		int len = strlen(mc.data) + 1;
		this->data = new char[len];
		strcpy(this->data, mc.data);
		return *this;
	}

	//既然我们在堆区分配了内存，需要在析构函数中释放内存
	~myclass(){
		if (NULL != this->data){
			delete[] this->data;
			this->data = NULL;
		}
	}
private:
	char* data;
};

void test_deep_copy(){

	//STL中所有的容器都是值寓意，用户提供的元素必须能够被拷贝
	vector<myclass> v;

	myclass m("hello");

	v.push_back(m);

}

int main(){

	test_deep_copy();

	system("pause");
	return EXIT_SUCCESS;
}