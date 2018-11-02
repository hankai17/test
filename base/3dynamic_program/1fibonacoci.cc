#include<iostream>  
#include<vector>  
using namespace std;  

vector<vector<int>> matrix_multi(vector<vector<int>> arr_a, vector<vector<int>> arr_b) {
  int row_a = arr_a.size(); //行
  int col_a = arr_a[0].size();
  int row_b = arr_b.size();  
  int col_b = arr_b[0].size();  

  vector<vector<int>> res;
  if (col_a != row_b) {
	return res;  
  } else {  
	res.resize(row_a);  
	for (int i = 0; i < row_a; ++i) {
	  res[i].resize(col_b);
	}

	for (int i = 0; i < row_a; ++i) {
	  for (int j = 0; j < col_b; ++j) {
		for (int k = 0; k < col_a; ++k) {
		  res[i][j] += arr_a[i][k] * arr_b[k][j];
		}
	  }
	}
  }  
  return res;
}

//前提是方阵
vector<vector<int>> matrix_power(vector<vector<int>> arr_a, int p) {
  int row_a = arr_a.size(); //行
  int col_a = arr_a[0].size();

  vector<vector<int>> res;
  res.resize(row_a);  
  for (int i = 0; i < row_a; ++i) {
	res[i] = arr_a[i]; //发生拷贝
  }
  
  for (int i = 0; i < p; i++) {
	res = matrix_multi(res, arr_a);
  }
  return res;
}

int f1(int n) {
  if (n < 1) return 0;
  if (n == 1 || n == 2) return 1;
  return f1(n - 1) + f1(n - 2);
}
//pre cur
//1   1   2 3 5 8 13 21
int f2(int n) {
  if (n < 1) return 0;
  if (n == 1 || n == 2) return 1;
  int cur = 1;
  int pre = 1;
  int tmp = 0;
  for(int i = 3; i <= n; i++) {
	tmp = cur;
    cur += pre;	
	pre = tmp; //两个变量解决不了问题
  }
  return cur;
}

int f3(int n) {
  if (n < 1) return 0;
  if (n == 1 || n == 2) return 1;
  vector<vector<int>> base = {{1, 1}, {1, 0}};
  vector<vector<int>> ret = matrix_power(base, n - 2);
  return ret[0][0] + ret[1][0];
}

int main() {
    cout<<f1(10)<<endl; //暴力递归 时间复杂度O2n 一生二 二生四
    cout<<f2(10)<<endl; //on
    cout<<f3(9)<<endl;; //ologn
}

//跨台阶问题 每次可以跨1个或2个 假设有N级 最后一步为N-2或者N-1  所以总方法数为f(N) = f(N-1) + f(N - 2)
//如果F(n) = a*F(n-1) + b*F(n-2) + ... + k*F(n-i) 那么他就是一个i阶递推式 一定可以写成i*i矩阵乘法的表达式 从而加速时间复杂度为ologn
