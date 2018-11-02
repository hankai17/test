#include<iostream>  
using namespace std;  
#define SIZE_M 2  
#define SIZE_N 3  
#define SIZE_S 4  

int main1() {  
  int matrix_a[SIZE_M][SIZE_N]={{1,2,3},{4,5,6}}; //a 2X3  
  int matrix_b[SIZE_N][SIZE_S]={{1,2,3,4},{5,6,7,8},{1,2,3,4}}; //b 3X4  
  int matrix_result[SIZE_M][SIZE_S]; //结果2X4  

  for(int m = 0; m < SIZE_M; m++) {
	for(int s = 0; s < SIZE_S; s++) {
	  matrix_result[m][s] = 0;
	  for(int n = 0; n < SIZE_N; n++) {  
		matrix_result[m][s] += matrix_a[m][n] * matrix_b[n][s];  
	  }
	}
  }
  for(int m = 0; m < SIZE_M; m++) {
	for(int s = 0; s < SIZE_S; s++) {
	  cout<<matrix_result[m][s]<<"\t";  
	}  
	cout<<endl;  
  }  
  return 0;  
}  

#include<vector>  

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

int main(void)  {  
  vector<vector<int>> arrA = { { 2, 1 }, { 4, 3 } };  
  vector<vector<int>> arrB = { { 1, 2 }, { 1, 0 } };  
  //vector<vector<int>> arrA = { { 1, 2, 3 }, { 4, 5, 6 } };  
  //vector<vector<int>> arrB = { { 1, 4 }, { 2, 5 }, { 3, 6 } };  
  vector<vector<int>> res = matrix_power(arrA, 2);  
  return 0;  
}  

