#include<iostream>
#include<vector>
using namespace std;

/*
arr=[5,2,3] aim=20 如果各币值无限制 组成20有n多种方法 
0张5  让2,3组成20
1张5  让2,3组成15
2张5  让2,3组成10
...
4张5  让2,3组成0
*/

int process(vector<int>& v, int index, int aim) { //用v[index... N-1] 这些面值的钱组成aim 返回总方法数目
  int res = 0;
  if(index == v.size()) {
	res = aim == 0 ? 1 : 0; //递归终止条件
  } else {
	for(int i = 0; v[index] * i <= aim; i++) {
	  res += process(v, index + 1, aim - v[index]*i);
    }
  }
  return res;
}

int coins(vector<int>& v, int aim) {
  if(v.size() == 0 || aim < 0) {
	return 0;
  }
  return process(v, 0, aim);
}

/*
arr=[5, 10, 25, 1] aim=1000
当已经使用过0张5 1张10的情况下 计算[25, 1]组成990
然而在使用2张5 0张10的情况下 仍计算[25, 1]组成990
所以有重复
把process(v, index, aim) 抽象成p(i,j) //即用i序列之后的币值 组合成j
*/
int process1(vector<int>& v, int index, int aim, vector<vector<int>>& map_v) { //用v[index... N-1] 这些面值的钱组成aim 返回总方法数目
  int res = 0;
  if(index == v.size()) {
	res = aim == 0 ? 1 : 0;
  } else {
	for(int i = 0; v[index] * i <= aim; i++) {
      int map_val = map_v[index + 1][aim - v[index]*i];
	  if(map_val != 0) {
		res += map_val == -1 ? 0 : map_val;
	  } else {
		res += process1(v, index + 1, aim - v[index]*i, map_v);
	  }
    }
  }
  map_v[index][aim] = res == 0 ? -1 : res;
  return res;
}

int coins1(vector<int>& v, int aim) {
  if(v.size() == 0 || aim < 0) {
	return 0;
  }
  vector<vector<int>> map_v;
  map_v.resize(v.size() + 1);
  for(int i = 0; i < v.size() + 1; i++) { //map全初始化为0  如果方法是0种填-1
	map_v[i].resize(aim + 1);
    for(int j = 0; j < aim + 1; j++) 
	  map_v[i][j] = 0;
  }
  
  return process1(v, 0, aim, map_v);
}


int main() {
  vector<int> v = {5, 10, 25, 1};
  cout<<coins(v, 1000)<<endl;
  cout<<coins1(v, 1000)<<endl;
  return 0;
}
