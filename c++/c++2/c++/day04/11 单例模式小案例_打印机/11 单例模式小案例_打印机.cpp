#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

class Printer{
private:
	Printer(){
		mCount = 0;
	}
	Printer(const Printer&){}
	~Printer(){}
public:
	void PrintText(string content){
		cout << "打印内容：" << content << endl;
		cout << "当前打印次数:" << mCount << endl;
		cout << "---------------" << endl;
		mCount++;
	}
	static Printer* getInstance(){
		return pPrinter;
	}
private:
	static Printer* pPrinter;
	int mCount;
};

//类外初始化
Printer* Printer::pPrinter = new Printer;

int main(){


	Printer* p1 = Printer::getInstance();
	Printer* p2 = Printer::getInstance();

	p1->PrintText("离职报告！");
	p2->PrintText("健康证明!");


	system("pause");
	return EXIT_SUCCESS;
}