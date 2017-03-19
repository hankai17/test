#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<functional>
#include<iterator>
#include<algorithm>

using namespace std;

//求交集 set_intersection
void test01(){
	
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10;i++){
		v1.push_back(i);
	}

	for (int i = 5; i < 15;i ++){
		v2.push_back(i);
	}

	vector<int> v3;
	v3.resize(min(v1.size(),v2.size()));

	vector<int>::iterator MyEnd = set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());

	copy(v3.begin(), MyEnd, ostream_iterator<int>(cout, " "));

}

//求并集 set_union
void test02(){

	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++){
		v1.push_back(i);
	}

	for (int i = 5; i < 15; i++){
		v2.push_back(i);
	}

	vector<int> v3;
	v3.resize(v1.size()+ v2.size());

	vector<int>::iterator MyEnd =  set_union(v1.begin(),v1.end(),v2.begin(),v2.end(),v3.begin());
	copy(v3.begin(), MyEnd, ostream_iterator<int>(cout, " "));
}


//求差集 set_difference
void test03(){

	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++){
		v1.push_back(i);
	}

	for (int i = 5; i < 15; i++){
		v2.push_back(i);
	}

	vector<int> v3;
	v3.resize(max(v1.size(),v2.size()));
	
	vector<int>::iterator MyEnd =  set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
	copy(v3.begin(), MyEnd, ostream_iterator<int>(cout, " "));
}

int main(){

	//test01();
	//test02();
	test03();


	system("pause");
	return EXIT_SUCCESS;
}