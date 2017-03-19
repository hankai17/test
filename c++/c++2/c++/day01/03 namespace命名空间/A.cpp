#include"A.h"

//static int a = 10; //只能在当前文件内有效 内部连接

void A_namespace::func1(){
	std::cout << "hello wrold!" << std::endl; //增加换行并且刷新缓冲区
}

void A_namespace::func2(int x){
	std::cout << "x:" << std::endl;
}


