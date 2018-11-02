#include<iostream>
#include<vector>
#include <algorithm>
using namespace std;
/*
arr=[5,2,3] aim=20 如果各币值无限制  四张五元可组成20 如果都限制为一张则无法组成
arr=[5,2,5,3] aim=10 两张五元可组成10
*/

/*
[5 2 3 1]  aim:5

  0   1   2   3   4   5
  ---------------------
5 0 max  max max max  1
2 0 max  1   max 2    1
3 0 max  1   1   2    1
1 0 1    1   1   2    1

*/
void print_v(vector<vector<int>>& v) {
  for(int i = 0; i < v.size(); i++) {
	for(int j = 0; j < v[i].size(); j++) {
	  cout<<v[i][j]<<" ";
    }
	cout<<endl;
  }
}

int get_min_money(vector<int>& v, int aim) {
  if(v.size() == 0 || aim == 0) return 0; 
  int n = v.size(); 
  int max = 0xFFFF;
  vector<vector<int>> dp;
  //n行 aim+1列
  dp.resize(n);
  for(int i = 0; i < dp.size(); i++) {
	dp[i].resize(aim + 1);
    dp[i][0] = 0;
  }

  //填第一行  j意为要组成的币值
  for(int j = 1; j < aim + 1; j++) {
	dp[0][j] = max;
    if(j - v[0] >= 0 && dp[0][j - v[0]] != max) { //看不懂后面那个条件
	  dp[0][j] = dp[0][j - v[0]] + 1; //j当前币值的个数等于 不用当前币值的张数+1
    }
  }

  //
  int left = 0;
  for(int i = 1; i < n; i++) {
	for(int j = 1; j <= aim; j++) {
	  left = max;
      if(j - v[i] >= 0 && dp[i][j - v[i]] != max) {
		left = dp[i][j - v[i]] + 1;
      }
      dp[i][j] = min(left, dp[i - 1][j]);
    }
  }
  print_v(dp);
  return dp[n - 1][aim] != max ? dp[n - 1][aim] : -1; 
}

int main() {
  vector<int> v = {5, 2, 3, 1};
  int aim = 5;
  cout<<"min_money = "<<get_min_money(v, aim)<<endl;
  return 0;
}
