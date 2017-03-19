#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<functional>
#include<algorithm>
#include<iterator>
using namespace std;

//copy
void test01(){
	
	vector<int> v1;
	for (int i = 0; i < 10;i++){
		v1.push_back(i + 1);
	}
	vector<int> v2;
	v2.resize(v1.size());
	copy(v1.begin(), v1.end(), v2.begin());
	copy(v2.begin(), v2.end(), ostream_iterator<int>(cout, " "));
}

//replace
bool MyCompare02(int v){
	return v > 2;
}
void test02(){

	vector<int> v1;
	v1.push_back(2);
	v1.push_back(7);
	v1.push_back(2);
	v1.push_back(9);
	v1.push_back(10);
	v1.push_back(3);
	v1.push_back(0);

	//replace(v1.begin(), v1.end(), 2, 100);
	replace_if(v1.begin(), v1.end(),MyCompare02,100);

	copy(v1.begin(), v1.end(), ostream_iterator<int>(cout, " "));
}

//swap
void test03(){
	
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++){
		v1.push_back(i + 1);
		v2.push_back(i);
	}

	for_each(v1.begin(), v1.end(), [](int val){cout << val << " "; }); cout << endl;
	for_each(v2.begin(), v2.end(), [](int val){cout << val << " "; }); cout << endl;
	swap(v1, v2);
	cout << "------------" << endl;
	for_each(v1.begin(), v1.end(), [](int val){cout << val << " "; }); cout << endl;
	for_each(v2.begin(), v2.end(), [](int val){cout << val << " "; }); cout << endl;
}

int main(){

	//test01();
	//test02();
	test03();

	system("pause");
	return EXIT_SUCCESS;
}