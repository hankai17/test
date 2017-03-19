#ifndef CIRCLE_H
#define CIRCLE_H

#include"Point.h"
#include<iostream>

class Circle{
public:
	//设置圆心
	void setPoint(int x, int y);
	//设置半径
	void setR(int r);
	//返回半径
	int getR();
	//返回圆心
	Point getPoint();
	void IsInCircle(Point& point);

private:
	Point mPoint; //圆心
	int mR; //半径
};

#endif