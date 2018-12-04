#include"Circle.h"

//设置圆心
void Circle::setPoint(int x, int y){
	mPoint.setX(x);
	mPoint.setY(y);
}
//设置半径
void Circle::setR(int r){
	mR = r;
}
//返回半径
int Circle::getR(){ return mR; }
//返回圆心
Point Circle::getPoint(){ return mPoint; }

void Circle::IsInCircle(Point& point){

	int distance = (getPoint().getX() - point.getX()) * (getPoint().getX() - point.getX()) + (getPoint().getY() - point.getY()) * (getPoint().getY() - point.getY());
	int r = getR() * getR();
	if (distance == r){
		std::cout << "Point(" << point.getX() << "," << point.getY() << ")在圆上!" << std::endl;
	}
	else if (distance > r){
		std::cout << "Point(" << point.getX() << "," << point.getY() << ")在圆外!" << std::endl;
	}
	else{
		std::cout << "Point(" << point.getX() << "," << point.getY() << ")在圆内!" << std::endl;
	}

}