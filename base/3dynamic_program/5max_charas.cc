#include<iostream>
#include<vector>
#include <algorithm>
using namespace std;

/*
  v1 = 2, 1
  v2 = 1, 2, 3, 4
------------------------------

    1   2   3   4
 2  x   1   x   x

 1  0

*/

vector<vector<int>> get_dp(vector<int>& v1, vector<int>& v2) {
  vector<vector<int>> dp;
  dp.resize(v1.size());
  for(int i = 0; i < v1.size(); i++) {
	dp[i].resize(v2.size());
  }
  
  for(int i = 0; i < v1.size(); i++) { //填充第一列
	if(v1[i] == v2[0]) {
	  dp[i][0] = 1;
    }
  }
  for(int i = 1; i < v2.size(); i++) {
	if(v2[i] == v1[0]) {
	  dp[0][i] = 1;
    }
  }

  for(int i = 1; i < v1.size(); i++) {
	for(int j = 1; j < v2.size(); j++) { //填充每一行
	  if(v1[i] == v2[j]) 
		dp[i][j] = dp[i - 1][j - 1] + 1;
    }
  }
  return dp;
}

int main() {
  vector<int> v1 = {1, 97, 98, 2, 3, 4, 5, 99, 100};
  vector<int> v2 = {1, 2, 3, 4, 5, 101, 102};
  //vector<int> v1 = {2, 1};
  //vector<int> v2 = {1, 2, 3, 4};
  int tmp = 0;
  int max = 0;
  vector<vector<int>> dp = get_dp(v1, v2);
  for(int i = 0; i < dp.size(); i++) {
	for(int j = 0; j < dp[0].size(); j++) {
	  //cout<<dp[i][j]<<" ";
	  if(dp[i][j] > tmp) {
		tmp = dp[i][j];
 		max = i;
	  }
    }
	//cout<<endl;
  }
  //cout<<tmp<<" "<<max<<endl; //4 6
  int begin_index = max - tmp + 1;
  for(int i = tmp; i != 0; i--) {
	cout<<v1[begin_index];
	begin_index++;
  }
  return 0;
}

