//list是双向链表容器 可高效的插入删除元素 不可随机存取元素
//是重点
#include <iostream>
using namespace std;
#include <list>

void main71()//基本操作
{
	list<int> l;
	cout << "list的大小：" << l.size() << endl;
	for (int i = 0; i < 10; i++)
	{
		l.push_back(i);//尾插法
	}
	cout << "list的大小：" << l.size() << endl;

	list<int>::iterator it = l.begin();
	while (it!=l.end())
	{
		cout << *it << " ";
		it++;
	}
	cout << endl;
	//list不能随机访问
	it = l.begin();
	it++;
	it++;
	it++;
	//it = it + 5; 不支持速记访问容器
	//it指向 3!!!!!!!!
	l.insert(it, 100);
	for (list<int>::iterator it = l.begin(); it != l.end(); it++)
	{
		cout << *it << " ";
	}
	//结论1 链表的节点index序号是从0号位置开始的
	//在3号位置插入元素 让原来的3号位置变4号位置。。。。。


}

void main72()//list删除
{
	list<int> l;
	cout << "list的大小：" << l.size() << endl;
	for (int i = 0; i < 10; i++)
	{
		l.push_back(i);//尾插法
	}
	cout << "list的大小：" << l.size() << endl;
	for (list<int>::iterator it = l.begin(); it != l.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;

	list<int>::iterator it1 = l.begin();
	list<int>::iterator it2 = l.begin();
	it2++;
	it2++;
	it2++;

	l.erase(it1, it2);//左闭右开的删除
	for (list<int>::iterator it = l.begin(); it != l.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;

	l.insert(l.begin(), 100);
	l.insert(l.begin(), 100);
	l.insert(l.begin(), 100);

	l.erase(l.begin());//删除头部元素
	l.remove(100);//删除所有100

}

void main()
{
	main72();
	system("pause");
	return;
}