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
		cout << "TargetSpaceNullException ���캯��!" << endl;
	}
	TargetSpaceNullException(const TargetSpaceNullException&){
		cout << "TargetSpaceNullException �������캯��!" << endl;
	}
	virtual void printError(){
		cout << "Ŀ��ռ��ָ���쳣!" << endl;
	}
	~TargetSpaceNullException(){
		cout << "TargetSpaceNullException ��������!" << endl;
	}
};

class SourceSpaceNullException : public BaseException{
public:
	SourceSpaceNullException(){
		cout << "SourceSpaceNullException ���캯��!" << endl;
	}
	SourceSpaceNullException(const SourceSpaceNullException&){
		cout << "SourceSpaceNullException �������캯��!" << endl;
	}
	void printError(){
		cout << "Դ��ռ��ָ���쳣!" << endl;
	}
	~SourceSpaceNullException(){
		cout << "SourceSpaceNullException ��������!" << endl;
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