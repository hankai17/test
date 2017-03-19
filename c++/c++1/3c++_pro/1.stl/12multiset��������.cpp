#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#include "set"

void main1001()
{
	multiset<int> set1;
	int tmp = 0;
	printf("请输入multiset集合的值：");
	scanf("%d", &tmp);
	while (tmp != 0)
	{
		set1.insert(tmp);
		printf("请输入multiset集合的值：");
		scanf("%d", &tmp);
	}

	for (multiset<int>::iterator it = set1.begin(); it != set1.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;

	while (!set1.empty())
	{
		multiset<int>::iterator it = set1.begin();
		cout << *it << " ";
		set1.erase(it);
	}
}

void main()
{
	main1001();
	system("pause");
	return;
}
//错误	1	error C4996: 'scanf': This function or variable may be unsafe. Consider using scanf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.	h:\c++\1.stl\1.stl\12multiset容器操作.cpp	10	1	1.stl
