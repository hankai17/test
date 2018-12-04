#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<deque>
#include<algorithm>
#include<string>
using namespace std;

/*
有5名选手，10个评委分别对每一名选手打分，去除最高分，去除评委中最低分，取平均分。
//1. 创建五名选手，放到vector中
//2. 遍历vector容器，取出来每一个选手，执行for循环，可以把10个评分打分存到deque容器中
//3. sort算法对deque容器中分数排序，pop_back pop_front去除最高和最低分
//4. deque容器遍历一遍，累加分数，累加分数/d.size()
//5. person.score = 平均分
*/


//选手类
class Player{
public:
	Player(){
		cout << "构造函数!" << endl;
	}
	Player(const Player&){
		cout << "拷贝构造函数!" << endl;
	}
	Player(const Player&&){
		cout << "move constrcutor!" << endl;
	}
	~Player(){
		cout << "析构函数!" << endl;
	}
public:
	string mName;
	int mSocre;
};


//创建五名选手
void Create_Player(vector<Player>& vPlist){

	string nameSeed = "ABCDE";

	for (int i = 0; i < 2;i++){
	
		Player player;
		player.mName = "选手";
		player.mName += nameSeed[i];
		player.mSocre = 0;

		vPlist.push_back(player);
	}
}


//评委打分

void Set_Score(vector<Player>& vPlist){

	//遍历每一个选手
	for (vector<Player>::iterator it = vPlist.begin(); it != vPlist.end(); it++){
	
		//保存评委分数
		deque<int> dScore;
		for (int i = 0; i < 10;i++){
			int score = rand() % 40 + 60;
			dScore.push_back(score);
		}
		//对分数排序
		sort(dScore.begin(), dScore.end());
		//去除最高分 去除最低分
		dScore.pop_front();
		dScore.pop_back();
		//求平均分
		int totalScore = 0;
		for (deque<int>::iterator sit = dScore.begin(); sit != dScore.end();sit ++){
			totalScore += *sit;
		}
		//求平均分
		int avgScore = totalScore / dScore.size();
		(*it).mSocre = avgScore;
	}
}

//打印选手信息
void ShowPlayerList(vector<Player>& vPlist){

	for (vector<Player>::iterator it = vPlist.begin(); it != vPlist.end(); it ++){
		
		cout << "Name:" << it->mName << " Score:" << it->mSocre << endl;
	
	}

}


int main(){

	//初始化选手对象
	vector<Player> vPlist;
	Create_Player(vPlist);
	//选手打分
	//Set_Score(vPlist);
	//打印选手信息
	//ShowPlayerList(vPlist);


	system("pause");
	return EXIT_SUCCESS;
}