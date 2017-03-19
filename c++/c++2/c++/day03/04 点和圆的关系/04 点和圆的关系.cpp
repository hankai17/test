#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//点类
class Point{
public:
	void setX(int x){ mX = x; }
	void setY(int y){ mY = y; }
	int getX(){ return mX; }
	int getY(){ return mY; }
private:
	int mX; //x坐标
	int mY; //y坐标
};


//圆类
class Circle{
public:
	//设置圆心
	void setPoint(int x,int y){
		mPoint.setX(x);
		mPoint.setY(y);
	}
	//设置半径
	void setR(int r){
		mR = r;
	}
	//返回半径
	int getR(){ return mR; }
	//返回圆心
	Point getPoint(){ return mPoint; }

	void IsInCircle(Point& point){
		int distance = (getPoint().getX() - point.getX()) * (getPoint().getX() - point.getX()) + (getPoint().getY() - point.getY()) * (getPoint().getY() - point.getY());
		int r = getR() * getR();
		if (distance == r){
			cout << "Point(" << point.getX() << "," << point.getY() << ")在圆上!" << endl;
		}
		else if (distance > r){
			cout << "Point(" << point.getX() << "," << point.getY() << ")在圆外!" << endl;
		}
		else{
			cout << "Point(" << point.getX() << "," << point.getY() << ")在圆内!" << endl;
		}

	}

private:
	Point mPoint; //圆心
	int mR; //半径
};


void IsInCircle(Circle& circle,Point& point){
	int distance = (circle.getPoint().getX() - point.getX()) * (circle.getPoint().getX() - point.getX()) + (circle.getPoint().getY() - point.getY()) * (circle.getPoint().getY() - point.getY());
	int r = circle.getR() * circle.getR();
	if (distance == r){
		cout << "Point(" << point.getX() << "," << point.getY() << ")在圆上!" << endl;
	}
	else if (distance > r){
		cout << "Point(" << point.getX() << "," << point.getY() << ")在圆外!" << endl;
	}
	else{
		cout << "Point(" << point.getX() << "," << point.getY() << ")在圆内!" << endl;
	}

}

int main(){


	//定义一个点
	Point point;
	point.setX(10);
	point.setY(15);

	//定义一个圆
	Circle circle;
	circle.setPoint(10,10);
	circle.setR(5);

	//调用全局函数
	//IsInCircle(circle, point);
	//调用成员函数
	circle.IsInCircle(point);


	system("pause");
	return EXIT_SUCCESS;
}