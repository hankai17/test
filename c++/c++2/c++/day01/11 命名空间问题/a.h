#ifndef A_H
#define A_H

#include<iostream>

namespace A{
	extern int a; //表明a是个声明，a在其他文件里面定义
	void func();
}

#endif