#include<iostream>
#include<string>
#include<sstream>
#include<stdlib.h>
using namespace std;

//aaabbadddffc  a3b2a1d3f2c1

string statics_chara(string str) {
  if(str.length() == 0) return nullptr;
  int num = 1;
  string ret;
  ret += str[0]; //为什么不能直接赋值?	
  for(int i = 1; i < str.length(); i++) {
	if(str[i] != str[i-1]) {
	  stringstream ss;
	  ss << num;
      //cout<<ret<<endl;
	  ret = ret + ss.str();
	  ret = ret + str[i];
	  num = 1;
	} else {
	  num++;
	}
  }
  stringstream ss;
  ss << num;
  ret = ret + ss.str();
  return ret;
}

int main() {
  string str = "aaabbadddffc";
  string tmp = statics_chara(str);
  cout<<tmp<<endl;
  return 0;
}
