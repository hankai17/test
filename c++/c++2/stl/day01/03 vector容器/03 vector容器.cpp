#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
using namespace std;

//1. 初始化
/*
vector<T> v; //采用模板实现类实现，默认构造函数
vector(v.begin(), v.end());//将v[begin(), end())区间中的元素拷贝给本身。
vector(n, elem);//构造函数将n个elem拷贝给本身。
vector(const vector &vec);//拷贝构造函数。

//例子 使用第二个构造函数 我们可以...
int arr[] = {2,3,4,1,9};
vector<int> v1(arr, arr + sizeof(arr) / sizeof(int));
*/


void PrintVector(vector<int>& v){

	for (vector<int>::iterator it = v.begin(); it != v.end(); it ++){
		cout << *it << " ";
	}
	cout << endl;
}

void test01(){
	
	vector<int> v1; //无参构造
	int arr[] = { 2, 3, 4, 1, 9 };
	vector<int> v2(arr, arr + sizeof(arr) / sizeof(int));
	vector<int> v3(v2.begin(),v2.end());
	vector<int> v4(10,0);
	vector<int> v5(v4);
	PrintVector(v2);
	PrintVector(v3);
	PrintVector(v4);
}

//2. 赋值操作
/*
assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身。
assign(n, elem);//将n个elem拷贝赋值给本身。
vector& operator=(const vector  &vec);//重载等号操作符
swap(vec);// 将vec与本身的元素互换。
*/

void test02(){
	
	int arr[] = { 2, 3, 4, 1, 9 };
	vector<int> v1(arr, arr + sizeof(arr) / sizeof(int));
	vector<int> v2;
	vector<int> v3;
	v2.assign(v1.begin(),v1.end());
	v3 = v2;
	PrintVector(v1);
	PrintVector(v2);
	PrintVector(v3);

	//swap交换
	int arr1[] = { 1, 2, 3,4, 5 };
	vector<int> v4(arr1, arr1 + sizeof(arr1) / sizeof(int));

	cout << "交换前:" << endl;
	PrintVector(v1);
	PrintVector(v4);

	v1.swap(v4);
	cout << "交换后:" << endl;
	PrintVector(v1);
	PrintVector(v4);	
}

//3 .大小操作
/*
size();//返回容器中元素的个数
empty();//判断容器是否为空
resize(int num);//重新指定容器的长度为num，若容器变长，则以默认值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。
resize(int num, elem);//重新指定容器的长度为num，若容器变长，则以elem值填充新位置。如果容器变短，则末尾超出容器长>度的元素被删除。
capacity();//容器的容量
reserve(int len);//容器预留len个元素长度，预留位置不初始化，元素不可访问。
*/

void test03(){
	int arr[] = { 2, 3, 4, 1, 9 };
	vector<int> v1(arr, arr + sizeof(arr) / sizeof(int));

	if (v1.empty()){
		cout << "空!" << endl;
	}
	else{
		cout << "不空!" << endl;
	}

	cout << "size:" << v1.size() << endl;
	v1.resize(10);
	PrintVector(v1);
	cout << "size:" << v1.size() << endl;
	v1.resize(4);
	PrintVector(v1);
	cout << "size:" << v1.size() << endl;
}

//4. 巧用swap收缩内存
void test04(){

	vector<int> v;
	for (int i = 0; i < 100000;i++){
		v.push_back(i);
	}

	cout << "capacity:" << v.capacity() << endl;
	cout << "size:" << v.size() << endl;

	v.resize(3);

	cout << "capacity:" << v.capacity() << endl;
	cout << "size:" << v.size() << endl;

	//收缩内存
	//vector<int>(v).swap(v);

	vector<int>(v).swap(v); //匿名对象

	cout << "capacity:" << v.capacity() << endl;
	cout << "size:" << v.size() << endl;
}

//5.reserve
void test05(){

	vector<int> v;
	v.reserve(100000);
	int num = 0;
	int* p = NULL;
	for (int i = 0; i < 100000;i++){
		v.push_back(i);
		if (p != &v[0]){
			p = &v[0];
			num++;
		}
	}

	cout << "num:" << num << endl;
}

//6. 插入和删除
/*
	insert(const_iterator pos, int count, ele);//迭代器指向位置pos插入count个元素ele.
	push_back(ele); //尾部插入元素ele
	pop_back();//删除最后一个元素
	erase(const_iterator start, const_iterator end);//删除迭代器从start到end之间的元素
	erase(const_iterator pos);//删除迭代器指向的元素
*/
void test06(){
	
	int arr[] = { 2, 3, 4, 1, 9 };
	vector<int> v1(arr, arr + sizeof(arr) / sizeof(int));

	//尾插法
	v1.push_back(100);
	//头插法
	v1.insert(v1.begin(),200);
	PrintVector(v1);
	cout << "尾删法之后：" << endl;
	v1.pop_back();
	PrintVector(v1);
	v1.erase(v1.begin());
	cout << "头删法之后：" << endl;
	PrintVector(v1);
	v1.erase(v1.begin(), v1.end()); //全部删除 v1.clear();
	cout << "size:" << v1.size() << endl;
}

//7.注意点
void test07(){

	//什么随机迭代器？ vector容器支持随机访问
	int arr[] = { 2, 3, 4, 1, 9 };
	vector<int> v1(arr, arr + sizeof(arr) / sizeof(int));

	vector<int>::iterator it = v1.begin();
	cout << *(it + 2) << endl;
	cout << v1[2] << endl;

	//逆序遍历
	PrintVector(v1);

	for (vector<int>::reverse_iterator it = v1.rbegin(); it != v1.rend();it ++){
		cout << *it << " ";
	}
	cout << endl;

}

int main(){

	//test01();
	//test02();
	//test03();
	//test04();
	//test05();
	//test06();
	test07();

	system("pause");
	return EXIT_SUCCESS;
}