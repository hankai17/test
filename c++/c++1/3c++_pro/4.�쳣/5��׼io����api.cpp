#include <iostream>
using namespace std;
#include "string"



void main1()
{
	char myBuf[1024]; 
	int myInt;
	long myLong;
	cin >> myInt;
	cin >> myLong;
	cin >> myBuf;//遇到空格停止接受数据
	cout << "myInt" << myInt << " myLong" << myLong << " Mybuf" << myBuf << endl;
	system("pause");
	return;
}

void main2()
{
	char ch;
	while ((ch = cin.get()) != EOF)
	{
		cout << ch << endl;//输入进入缓冲区，找不到eof就阻塞在那里
	}


	system("pause");
}

void main3()
{
	char a, b, c;
	cin.get(a);//如果缓冲区没有数据 则程序阻塞
	cin.get(b);
	cin.get(c);
	cout << a << b << c<<"因为缓冲区有数据，程序不会阻塞";
	cin.get(a).get(b).get(c);//支持链式编程
	cout << a << b << c;
	system("pause");
}
void main4()
{
	
	char buf1[256];
	char buf2[256];
	//输入一个字符串含有多个空格
	cin >> buf1;
	

	cin.getline(buf2,256);
	cout << "buf1:" << buf1 << "buf2:" << buf2 << endl;
	system("pause");
}

void main()
{

	char buf1[256];
	char buf2[256];
	//输入aa fffdsfs一个字符串含有多个空格
	cin >> buf1;
	cin.ignore(2);//吃掉两个空格
	int myint = cin.peek();
	cout << "myint:" << myint << endl;
	cin.getline(buf2, 256);
	cout << "buf1:" << buf1 << "buf2:" << buf2 << endl;
	system("pause");
}

//peek一把 偷窥一把 不长时间的占用内核资源
//putback 吐出去 输出的与输入的一样