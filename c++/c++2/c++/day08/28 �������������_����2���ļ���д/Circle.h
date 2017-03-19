#pragma once

#include<iostream>
using namespace std;
#include"Point.h"

//圆类
class Circle{
public:
	void setP(int x, int y);
	void setR(int r);
	Point& getP();
	int getR();
	//判断点和圆的关系
	void IsPointInCircle(Point& point);
private:
	Point mP; //圆心
	int mR; //半径
};
