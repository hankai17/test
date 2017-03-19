#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;


class Building;
class GoodGayFriend{
public:
	void VisitAndPlay();
public:
	Building building;
};

class Building{
	friend class GoodGayFriend;
public:
	Building();
public:
	string mSittingRoom;//客厅，谁都可以访问
private:
	string mBedRoom;  //卧室，只能好基友，或者经过我的允许的人才能进入
};

Building::Building(){
	mSittingRoom = "客厅";
	mBedRoom = "卧室";
}

void GoodGayFriend::VisitAndPlay(){
	cout << "和好基友在基友的" << building.mBedRoom << "Play!" << endl;
}

int main(){


	system("pause");
	return EXIT_SUCCESS;
}