#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Cube{
public:
	//初始化函数
	void Init(){
		mL = 0;
		mW = 0;
		mH = 0;
	}

	//set get对外读写函数
	void setL(int length){
		mL = length;
	}
	void setW(int width){
		mW = width;
	}
	void setH(int height){
		mH = height;
	}
	int getL() { return mL; }
	int getW() { return mW; }
	int getH() { return mH; }

	//求面积
	int caculateS(){
		return  2 * mL*mW + 2 * mW*mH + 2 * mL*mH;
	}
	//求体积
	int caculateV(){
		return mL*mH*mW;
	}

	//比较两个立方相等
	bool CompareCub(Cube& c){
		if (getH() == c.getH() && getL() == c.getL() && getW() == c.getW()){
			return true;
		}
		return false;
	}

private:
	int mL;//长
	int mW;//宽
	int mH;//高
};


//全局函数 比较两个立方体是否相等
bool CompareCub( Cube& c1, Cube& c2){

	if (c1.getH() == c2.getH() && c1.getL() == c2.getL() && c1.getW() == c2.getW()){
		return true;
	}
	return false;
}

int main(){

	Cube c1;
	Cube c2;
	//初始化函数
	c1.Init();
	c2.Init();

	c1.setH(10);
	c1.setL(10);
	c1.setW(10);

	c2.setH(10);
	c2.setL(10);
	c2.setW(10);

	//全局函数
	if (CompareCub(c1, c2)){
		cout << "c1和c2是两个相等的立方体！" << endl;
	}else{
		cout << "c1和c2是两个不相等的立方体！" << endl;
	}

	//成员函数
	c1.CompareCub(c2);
	//c2.CompareCub(c1);
	if (c1.CompareCub(c2)){
		cout << "c1和c2是两个相等的立方体！" << endl;
	}
	else{
		cout << "c1和c2是两个不相等的立方体！" << endl;
	}


	system("pause");
	return EXIT_SUCCESS;
}