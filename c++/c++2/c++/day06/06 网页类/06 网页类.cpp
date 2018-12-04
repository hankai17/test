#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;


//首页
class IndexPage{
public:
	void Header(){
		cout << "显示头部信息-----!" << endl;
	}

	void LeftNav(){
		cout << "显示左侧导航信息!" << endl;
	}

	void Footer(){
		cout << "显示底部信息!" << endl;
	}

	void MainBody(){
		cout << "首页信息内容!" << endl;
	}
};

//新闻页
class NewsPage : public IndexPage{
public:
	void MainBody(){
		cout << "新闻页信息内容!" << endl;
	}
};


//1.继承复用已经写好的代码  复用以前人写代码
//2.继承最终目的为了实现多态
void test01(){
	IndexPage index;
	index.Header();
	index.LeftNav();
	index.MainBody();
	index.Footer();

	cout << "------" << endl;

	NewsPage news;
	news.Header();
	news.LeftNav();
	news.MainBody();
	news.Footer();
}


int main(){

	test01();

	system("pause");
	return EXIT_SUCCESS;
}