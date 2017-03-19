#include <iostream>
using namespace std;
#include "queue"
#include <functional>

void main81()
{
	priority_queue<int> p1;//默认情况下 最大值优先级队列
	priority_queue< int, vector<int>, less<int> > p2;//提前定义好的函数 谓词
	priority_queue<int, vector<int>, greater<int>> p3;//最小值有限队列

	p1.push(33);
	p1.push(11);
	p1.push(55);
	p1.push(22);

	cout << "对头元素：" << p1.top() << endl;
	cout << "队列大小：" << p1.size() << endl;

	while (p1.size() > 0)
	{
		cout << p1.top() << " ";
		p1.pop();
	}

	cout << "测试最小值优先队列" << endl;
	p3.push(33);
	p3.push(11);
	p3.push(55);
	p3.push(22);

	cout << "对头元素：" << p3.top() << endl;
	cout << "队列大小：" << p3.size() << endl;

	while (p3.size() > 0)
	{
		cout << p3.top() << " ";
		p3.pop();
	}

}

void main()
{
	main81();
	system("pause");
	return;
}
