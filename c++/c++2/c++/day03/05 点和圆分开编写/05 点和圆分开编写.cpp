#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include"Circle.h"

int main(){


	//定义一个点
	Point point;
	point.setX(10);
	point.setY(15);

	//定义一个圆
	Circle circle;
	circle.setPoint(10, 10);
	circle.setR(5);

	//调用全局函数
	//IsInCircle(circle, point);
	//调用成员函数
	circle.IsInCircle(point);

	system("pause");
	return EXIT_SUCCESS;
}