#include<iostream>
#include<vector>
#include <algorithm>
using namespace std;
/*
  a  b  3  4  c
a 1  1  1  1  1
1 1  1  1  1  1
b 1  2  2  2  2
c 1  2  2  2  3
2 1  2  2  2  3
*/

vector<vector<int>> get_dp(vector<int>& v1, vector<int>& v2) {
  vector<vector<int>> dp;
  dp.resize(v1.size());
  for(int i = 0; i < v1.size(); i++) {
    dp[i].resize(v2.size());
  }
  dp[0][0] = v1[0] == v2[0] ? 1 : 0;
  for(int i = 1; i < v1.size(); i++) { //填充第一列
	dp[i][0] = max(dp[i - 1][0], v1[i] == v2[0] ? 1 : 0);
  }
  for(int i = 1; i < v2.size(); i++) {
	dp[0][i] = max(dp[0][i - 1], v2[i] == v1[0] ? 1 : 0);
  }

  for(int i = 1; i < v1.size(); i++) { //依次填充每一行
	for(int j = 1; j < v2.size(); j++) {
	  dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]); 
	  if(v1[i] == v2[j]) 
		dp[i][j] = max(dp[i][j], dp[i - 1][j - 1] + 1);
	}
  }
  return dp;
}

int main() {
  vector<int> v1 = {97, 98, 3, 4, 99};
  vector<int> v2 = {97, 1, 98,99, 2};
  vector<vector<int>> dp = get_dp(v1, v2);
/*
  for(int i = 0; i < dp.size(); i++) {
	for(int j = 0; j < dp[0].size(); j++) {
	  cout<<dp[i][j]<<" ";
    }
    cout<<endl;
  } 
*/

  return 0;
}
