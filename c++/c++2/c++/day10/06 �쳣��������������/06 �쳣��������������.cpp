#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;


class BaseException{
public:
	virtual void printError() = 0;
};

class TargetSpaceNullException : public BaseException{
public:
	TargetSpaceNullException(){
		cout << "TargetSpaceNullException 构造函数!" << endl;
	}
	TargetSpaceNullException(const TargetSpaceNullException&){
		cout << "TargetSpaceNullException 拷贝构造函数!" << endl;
	}
	virtual void printError(){
		cout << "目标空间空指针异常!" << endl;
	}
	~TargetSpaceNullException(){
		cout << "TargetSpaceNullException 析构函数!" << endl;
	}
};

class SourceSpaceNullException : public BaseException{
public:
	SourceSpaceNullException(){
		cout << "SourceSpaceNullException 构造函数!" << endl;
	}
	SourceSpaceNullException(const SourceSpaceNullException&){
		cout << "SourceSpaceNullException 拷贝构造函数!" << endl;
	}
	void printError(){
		cout << "源标空间空指针异常!" << endl;
	}
	~SourceSpaceNullException(){
		cout << "SourceSpaceNullException 析构函数!" << endl;
	}
};


void CopyString(char* dst,const char* source){
	if (dst == NULL){
		throw TargetSpaceNullException();
	}

	if(source == NULL){
		throw SourceSpaceNullException();
	}

	const char* temp = source;
	char* target = dst;
	while (*temp != '\0'){
		*target = *temp;
		target++;
		temp++;
	}
}



int main(){

	const char* source = "hello world!";
	char buf[1024] = { 0 };


	try{
		CopyString(buf, NULL);
	}
#if 1
	catch (TargetSpaceNullException ex){
		ex.printError();
	}
	catch (SourceSpaceNullException& ex){
		ex.printError();
	}
#else
	catch (BaseException& ex){
		ex.printError();
	}
#endif
	

	cout << "buf:" << buf << endl;



	system("pause");
	return EXIT_SUCCESS;
}