#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<deque>
#include<algorithm>
using namespace std;

//double end queue

//1.初始化
/*
deque<T> deqT;//默认构造形式
deque(beg, end);//构造函数将[beg, end)区间中的元素拷贝给本身。
deque(n, elem);//构造函数将n个elem拷贝给本身。
deque(const deque &deq);//拷贝构造函数。
*/


//每个容器都提供自己的迭代器
void PrintDeque(const deque<int>& d){
	//告诉编译器，我不会通过迭代器去修改容器中元素的值
	//const_iterator 只读
	//reverse_iterator 逆序迭代器
	//iterator 最普通的正向迭代器
	for (deque<int>::const_iterator it = d.begin(); it != d.end(); it ++){
		cout << *it << " ";
		//*it = 100;
	}
	cout << endl;

}

void test01(){

	deque<int> d1; //无参构造函数
	int arr[] = { 6, 2, 8, 1, 0 };
	deque<int> d2(arr,arr + sizeof(arr)/sizeof(int));
	deque<int> d3(10, 0);
	deque<int> d4(d2);
	PrintDeque(d2);
	PrintDeque(d3);
	PrintDeque(d4);

}

//2. 赋值操作
/*
assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身。
assign(n, elem);//将n个elem拷贝赋值给本身。
deque& operator=(const deque &deq); //重载等号操作符
swap(deq);// 将deq与本身的元素互换
*/
void test02(){
	
	deque<int> d1;
	deque<int> d2;
	d1.assign(10, 10);
	d2.assign(10, 20);
	PrintDeque(d1);
	PrintDeque(d2);
	cout << "---------" << endl;
	cout << "交换之后:" << endl;
	d1.swap(d2);
	PrintDeque(d1);
	PrintDeque(d2);
	cout << "---------" << endl;
	d1 = d2;
	PrintDeque(d1);
	PrintDeque(d2);
}

//3. 大小操作
/*
deque.size();//返回容器中元素的个数
deque.empty();//判断容器是否为空
deque.resize(num);//重新指定容器的长度为num,若容器变长，则以默认值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。
deque.resize(num, elem); //重新指定容器的长度为num,若容器变长，则以elem值填充新位置,如果容器变短，则末尾超出容器长度的元素被删除。
*/
void test03(){

	deque<int> d1;
	d1.assign(10, 100);

	cout << "size:" << d1.size() << endl;
	if (d1.empty()){
		cout << "空!" << endl;
	}
	else{
		cout << "不空!" << endl;
	}

	d1.resize(15, 1);
	PrintDeque(d1);

}

//4. 两端插入删除
/*
push_back(elem);//在容器尾部添加一个数据
push_front(elem);//在容器头部插入一个数据
pop_back();//删除容器最后一个数据
pop_front();//删除容器第一个数据
*/
void test04(){
	
	deque<int> d;
	d.push_back(10);
	d.push_back(20);
	d.push_front(100);
	d.push_front(200);

	PrintDeque(d);

	d.pop_back();
	d.pop_front();

	PrintDeque(d);
}

//5. 存取操作
/*
at(idx);//返回索引idx所指的数据，如果idx越界，抛出out_of_range。
operator[];//返回索引idx所指的数据，如果idx越界，不抛出异常，直接出错。
front();//返回第一个数据。
back();//返回最后一个数据
*/
void test05(){
	
	deque<int> d;
	d.push_back(10);
	d.push_back(20);
	d.push_front(100);
	d.push_front(200);

	for (int i = 0; i < d.size();i ++){
		cout << d[i] << " ";
	}
	cout << endl;


	for (int i = 0; i < d.size(); i++){
		cout << d.at(i) << " ";
	}
	cout << endl;

	cout << "front:" << d.front() << endl;
	d.front() = 100;
	cout << "front:" << d.front() << endl;
	cout << "back:" << d.back() << endl;
	d.back() = 200;
	cout << "back:" << d.back() << endl;

}

//6. 插入和删除
/*
insert(pos,elem);//在pos位置插入一个elem元素的拷贝，返回新数据的位置。
insert(pos,n,elem);//在pos位置插入n个elem数据，无返回值。
insert(pos,beg,end);//在pos位置插入[beg,end)区间的数据，无返回值。
clear();//移除容器的所有数据
erase(beg,end);//删除[beg,end)区间的数据，返回下一个数据的位置。
erase(pos);//删除pos位置的数据，返回下一个数据的位置。
*/

void test06(){

	deque<int> d;
	d.push_back(10);
	d.push_back(20);
	d.push_front(100);
	d.push_front(200);
	d.insert(d.begin(),500);
	d.insert(d.end(), 500);

	deque<int> d2;
	d2.push_back(0);
	d2.push_back(1);
	d2.push_back(2);

	d.insert(d.begin() + 1, d2.begin(), d2.end());

	//500 0 1 2 200 100 10 20 500
	PrintDeque(d);

	//删除
	d.erase(d.begin()); //头删
	d.erase(d.begin(), d.end()); //等价于 d.clear();

	cout << "size:" << d.size() << endl;
}


/*
	有5名选手，10个评委分别对每一名选手打分，去除最高分，去除评委中最低分，取平均分。
	//1. 创建五名选手，放到vector中
	//2. 遍历vector容器，取出来每一个选手，执行for循环，可以把10个评分打分存到deque容器中
	//3. sort算法对deque容器中分数排序，pop_back pop_front去除最高和最低分
	//4. deque容器遍历一遍，累加分数，累加分数/d.size()
	//5. person.score = 平均分
*/
bool MyCompare(int v1,int v2){
	return v1 < v2;
}
void test07(){

	deque<int> d;
	d.push_back(10);
	d.push_back(20);
	d.push_front(100);
	d.push_front(200);

	PrintDeque(d);
	sort(d.begin(), d.end(), MyCompare);
	PrintDeque(d);

	deque<int>::iterator max =  max_element(d.begin(), d.end());
	deque<int>::iterator min =  min_element(d.begin(), d.end());
	d.erase(max);
	d.erase(min);
	PrintDeque(d);

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