#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<algorithm>
#include<deque>
#include<time.h>
#include<numeric>
#include<functional>
using namespace std;


//选手类
class Player{
public:
	string mName;
	int mScore[3];
};




//创建24名选手
void Create_Player(vector<int>& v, map<int,Player>& m){
	
	string seedName = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (int i = 0; i < 24; i ++){
		Player player;
		player.mName = "选手";
		player.mName += seedName[i];
		for (int j = 0; j < 3;j++){
			player.mScore[j] = 0;
		}

		//生成当前选手的编号
		int ID = 100 + i;

		//保存编号
		v.push_back(ID);
		//保存选手信息
		m.insert(make_pair(ID,player));
	}

}

/*
选手抽签 选手比赛 查看比赛结果
*/

//抽签
void Draw(vector<int>& v){
	random_shuffle(v.begin(),v.end());
}

//比赛
void SpeechContest(int index,vector<int>& vIDs, map<int, Player>& m,vector<int>& vLevelUp){
	
	srand((unsigned int)time(NULL));
	//保存分组的信息,指定按照分数从大到小排列
	multimap<int, int, greater<int>> mGroup;
	//遍历每一个选手
	for (vector<int>::iterator it = vIDs.begin(); it != vIDs.end(); it ++){
		

		//保存选手的分数
		deque<int> dScore;
		//对每一个选手打分
		for (int i = 0; i < 10; i++){
			int score = rand() % 40 + 60;
			dScore.push_back(score);
		}
		//排序
		sort(dScore.begin(), dScore.end());
		//去除最高分 去除最低分
		dScore.pop_back();
		dScore.pop_front();
		//求总分
		int totalScore = accumulate(dScore.begin(), dScore.end(), 0);
		//求平均分
		int avgScore = totalScore / dScore.size();
		//保存选手得分
		m[*it].mScore[index - 1] = avgScore;

		//放到分组中
		mGroup.insert(make_pair(avgScore,*it));

		if (mGroup.size() == 6){
			
			int index = 0;
			for (multimap<int, int, greater<int>>::iterator it = mGroup.begin(); it != mGroup.end() && index < 3; it++,index++){
				vLevelUp.push_back(it->second);
			}
			mGroup.clear();
		}

	}

}

//显示晋级名单
void ShowLevelUpPeople(int index,vector<int>& vLevel, map<int, Player>& m){
	cout << "第" << index << "轮晋级名单:" << endl;
	for (vector<int>::iterator it = vLevel.begin(); it != vLevel.end(); it ++){
		cout << "Name:" << m[*it].mName << " 分数:" << m[*it].mScore[index - 1] << endl;
	}

	cout << "------打印信息结束------" << endl;

}


int main(){


	//创建选手
	vector<int> v1; //选手编号
	vector<int> v2;
	vector<int> v3;
	vector<int> v4;

	//选手信息
	map<int, Player> mPlist; 
	Create_Player(v1, mPlist);

	//第一轮比赛
	Draw(v1); //抽签
	SpeechContest(1,v1, mPlist, v2);
	ShowLevelUpPeople(1,v2,mPlist);


	//第二轮
	Draw(v2); //抽签
	SpeechContest(2, v2, mPlist, v3);
	ShowLevelUpPeople(2, v3, mPlist);

	//第三轮
	Draw(v3); //抽签
	SpeechContest(3, v3, mPlist, v4);
	ShowLevelUpPeople(3, v4, mPlist);

	system("pause");
	return EXIT_SUCCESS;
}