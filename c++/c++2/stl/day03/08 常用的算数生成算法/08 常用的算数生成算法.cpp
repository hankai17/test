#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>
#include<iterator>

using namespace std;

//accumulate ÀÛ¼ÓÈÝÆ÷ÖÐÔªËØ
void test01(){
	
	vector<int> v;
	for (int i = 0; i <= 100;i++){
		v.push_back(i);
	}

	int total = accumulate(v.begin(),v.end(),0);
	cout << "total:" << total << endl;
}

//fillÌî³ä
void test02(){
	
	vector<int> v;
	v.resize(10);
	fill(v.begin(), v.end(), 100);
	copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
}



int main(){

	//test01();
	test02();

	system("pause");
	return EXIT_SUCCESS;
}