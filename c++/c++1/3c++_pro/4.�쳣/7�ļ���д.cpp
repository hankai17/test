#include <iostream>
using namespace std;
#include "fstream"


void main()
{
	char * fname = "c:/2.txt";
	ofstream fout(fname);//建一个 输出流对象和文件关联
	cout << "hello....." << endl;
	fout.close();

	//读文件
	ifstream fin(fname);//建一个输入流和文件关联
	char ch;
	while(fin.get(ch))
	{
		cout << ch;
	}
	fin.close();




	system("pause");
	return;
}
