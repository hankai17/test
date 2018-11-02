#include<iostream>
#include<list>

void print_same(std::list<int>& l1, std::list<int>& l2) {
  std::list<int>::iterator it1 = l1.begin();
  std::list<int>::iterator it2 = l2.begin();

  while(it1 != l1.end() && it2 != l2.end()) {
	if(*it1 > *it2) {
	  it2++;
	} else if(*it1 < *it2) {
	  it1++;
	}
	//while(*it1 == *it2 && (it1 != l1.end() && it2 != l2.end()) )
	while((it1 != l1.end() && it2 != l2.end()) && *it1 == *it2) {  //if放到while之上还是有道理的 比如若it1++后为空 则就不会再进入while 结束
	  std::cout<<*it1<<"-->";
	  it1++; 
	  it2++; 
	}
  }
  std::cout<<"end"<<std::endl;
  return;
}

int main() {
  std::list<int> l1;
  std::list<int> l2;

  l1.push_back(2);
  l1.push_back(3);
  l1.push_back(4);
  l1.push_back(5);
  l1.push_back(5);

  l2.push_back(3);
  l2.push_back(4);
  l2.push_back(5);
  l2.push_back(5);
  l2.push_back(5);
  l2.push_back(5);
  l2.push_back(5);
  l2.push_back(5);
  l2.push_back(5);
  l2.push_back(5);

  print_same(l1,l2);
  return 0;
}
