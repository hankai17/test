/*
1 3 5 9
8 1 3 4
5 0 6 1
8 8 4 0

先生成最小路径和矩阵dp
1 4 9 18
9
14
22

1 4 9 18
9 5 8 12
14 5 11 12
22 13 15 12
*/

#include<iostream>
#include<vector>
using namespace std;

int get_min_path(vector<vector<int>>& m) {
  if(m.size() == 0 || m[0].size() == 0) {
	return 0;
  }
  int row = m.size();
  int col = m[0].size();
  vector<vector<int>> dp;
  dp.resize(row);
  for(int i = 0; i < col; i++) dp[i].resize(col);
  dp[0][0] = m[0][0];
  
  for(int i = 1; i < row; i++) {
	dp[i][0] = dp[i - 1][0] + m[i][0];
  }
  for(int i = 1; i < col; i++) {
	dp[0][i] = dp[i - 1][0] + m[0][i];
  }

  for(int i = 1; i < row; i++) { //按行填充
	for(int j = 1; j < col; j++) {
	  if(dp[i - 1][j] < dp[i][j - 1] ) { //上面的小
		dp[i][j] = m[i][j] + dp[i -1][j];
      } else {
		dp[i][j] = m[i][j] + dp[i][j - 1];
 	  }
    }
  }
  return dp[row -1][col - 1];
}

int main() {
  vector<vector<int>> m = {{1, 3, 5, 9}, {8, 1, 3, 4}, {5, 0, 6, 1}, {8, 8, 4, 0}};
  cout<<"min = "<<get_min_path(m)<<endl;
  return 0;
}

