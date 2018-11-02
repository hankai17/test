#include<iostream>
#include<vector>

void partition(std::vector<int>& v) {
  if(v.empty() || v.size() == 1) {
	return;
  }
  int i = 0;
  int u = 0;
  while(i < (int)v.size()) {
	//if(v[u] != v[i]) 
	if(v[u] != v[i++]) { //理解的核心： u只有在两个值不等的时候才++ 而i是每次都要++的
	  v[++u] = v[i-1];
	}
  }
  return;
}

int main() {
  std::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(2);
  v.push_back(2);
  v.push_back(3);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);
  v.push_back(6);
  v.push_back(6);
  v.push_back(7);
  v.push_back(7);
  v.push_back(8);
  v.push_back(8);
  v.push_back(8);
  v.push_back(8);
  v.push_back(9);
  for(auto it = v.begin(); it != v.end(); it++) {
	std::cout<<*it<<" ";
  }
  std::cout<<std::endl;
  partition(v);

  for(std::vector<int>::iterator it = v.begin(); it != v.end(); it++) {
	std::cout<<*it <<" ";
  }
  return 0;
}

