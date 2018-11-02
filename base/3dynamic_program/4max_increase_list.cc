#include<iostream>
#include<vector>
#include <algorithm>
using namespace std;

/*
arr=[2, 1, 5, 3, 6, 4, 8, 9, 7]
结果为[1, 3, 4, 8, 9]
*/

/*
生成dp[i] 即以arr[i]结尾(eg:最后一个元素7)的情况下 arr[0...i]中的最大递增子序列长度
dp[0] 为1
*/

vector<int> get_dp(vector<int>& v) {
  //if(v.size() == 0) return ;
  vector<int> dp;
  dp.resize(v.size());
  for(int i = 0; i < dp.size(); i++) {
	dp[i] = 1;
    for(int j = 0; j < i; j++) {
	  if(v[i] > v[j]) { //5大于2 5大于1
		dp[i] = max(dp[i], dp[j] + 1);
      }
    }
  }
  return dp;
}

int main() {
  vector<int> v = {2, 1, 5, 3, 6, 4, 8, 9, 7};
  vector<int> dp = get_dp(v);
  for(auto it = dp.begin(); it != dp.end(); it++) {
	cout<<*it<<" ";
  } //1, 1, 2, 2, 3, 3, 4, 5, 4
  cout<<endl;
  //直觉 果然是逆序
/*
  reverse(dp.beign(), dp.end());
  int max = 0;
  for(auto it = dp.begin(); it != dp.end(); it++) {
	
  }
*/
  int len = 0;
  int index = 0;
  for(int i = 0; i < dp.size(); i++) {
    if(dp[i] > len) {
	  len = dp[i];
      index = i;
    }
  } //len记录最大值5 i记录其下标7
  vector<int> lis;
  lis.resize(len);
  lis[--len] = v[index];

  for(int i = index; i >= 0; i--) {
    if(v[i] < v[index] && dp[i] == dp[index] - 1) { //不是很懂
	  lis[--len] = v[i];
	  index = i;
    }
  }

  for(auto it = lis.begin(); it != lis.end(); it++) {
	cout<<*it<<" ";
  }
  return 0;
}
