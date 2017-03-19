#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<time.h>
using namespace std;

enum{CAIWU,RENLI,YANFA};

class Worker{
public:
	string mName;
	int mSalary;
};

//创建员工
void Create_Worker(vector<Worker>& wlist){

	string nameSeed = "ABCDE";
	for (int i = 0; i < 5;i++){
		Worker worker;
		worker.mName = "员工";
		worker.mName += nameSeed[i];
		worker.mSalary = rand() % 10000 + 10000;

		wlist.push_back(worker);
	}
}


//员工分组
void Set_Group(vector<Worker>& wlist, multimap<int, Worker>& group){

	srand((unsigned int)time(NULL));
	for (vector<Worker>::iterator it = wlist.begin(); it != wlist.end(); it ++){	
		//随机产生部门编号
		int departmentID = rand() % 3;
		//cout << departmentID << endl;
		//员工插入到分组中
		group.insert(make_pair(departmentID,*it));
	}

}

//按组打印员工信息
void ShowWorkersByGroup(multimap<int, Worker>& group){
	

	cout << "财务部门:" << endl;
	multimap<int, Worker>::iterator pos =  group.find(CAIWU);
	//统计下本组多少人
	int wcount = group.count(CAIWU);
	int index = 0;
	for (; pos != group.end() && index < wcount;pos++,index++){
		cout << "Name:" << pos->second.mName << " Salary:" << pos->second.mSalary << endl;
	}
	cout << "----------------" << endl;



	cout << "人力部门:" << endl;
	pos = group.find(RENLI);
	wcount = group.count(RENLI);
	index = 0;
	for (; pos != group.end() && index < wcount; pos++,index++){
		cout << "Name:" << pos->second.mName << " Salary:" << pos->second.mSalary << endl;
	}
	cout << "----------------" << endl;


	cout << "人力部门:" << endl;
	pos = group.find(YANFA);
	wcount = group.count(YANFA);
	index = 0;
	for (; pos != group.end() && index < wcount; pos++,index++){
		cout << "Name:" << pos->second.mName << " Salary:" << pos->second.mSalary << endl;
	}
	cout << "----------------" << endl;


}

int main(){


	//创建员工
	vector<Worker> vWorker;
	Create_Worker(vWorker);
	//保存员工的分组信息
	multimap<int, Worker> mGroup;
	Set_Group(vWorker, mGroup);
	//打印员工信息
	ShowWorkersByGroup(mGroup);


	system("pause");
	return EXIT_SUCCESS;
}