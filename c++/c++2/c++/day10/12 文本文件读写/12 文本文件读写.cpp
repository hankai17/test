#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
using namespace std;

int main(){

	//如果文件不存在，则创建文件？  写文件，文件不存在创建文件。
	ifstream ism("./source.txt",ios::in);
	ofstream osm("./target.txt", ios::out);
	if (!ism){
		cout << "打开文件失败!" << endl;
	}
	if (!osm){
		cout << "文件打开失败!" << endl;
	}

	while (!ism.eof()){
		char buf[1024] = { 0 };
		ism.getline(buf,1024);
		cout << buf << endl;
		osm << buf << endl;
	}

	//文件关闭
	ism.close(); 
	osm.close();

	system("pause");
	return EXIT_SUCCESS;
}