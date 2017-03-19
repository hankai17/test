#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

class Building{
	//GoodFriend函数是Building的好基友
	friend void GoodFriend(Building& building);
public:
	Building(){
		mSittingRoom = "客厅";
		mBedRoom = "卧室";
	}
public:
	string mSittingRoom;//客厅，谁都可以访问
private:
	string mBedRoom;  //卧室，只能好基友，或者经过我的允许的人才能进入
};

//友元函数，全局函数，不是building类的成员函数，没有this指针
void GoodFriend(Building& building){
	cout << "我去好基友的私密的很多看不到的好几个T非常大的电视房间看看:" << building.mBedRoom << endl;
}


int main(){



	system("pause");
	return EXIT_SUCCESS;
}