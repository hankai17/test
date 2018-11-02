#include<iostream>
#include<list>

void print_list(std::list<int>& l) {
  if(l.size() == 0) return;
  for(auto it = l.begin(); it != l.end(); it++) {
	std::cout<<*it<<" ";
  }
  std::cout<<std::endl;
}

void delete_backwards(std::list<int>& l, int k) {
  if((int)l.size() < k) {
	return;
  } else if((int)l.size() == k) {
	l.pop_front();
	return;
  } else {
	std::list<int>::iterator it = l.begin();
	while(it != l.end()) {
	  k--;
	  it++;
	}
	k++;
	//std::cout<<"fun k="<<k<<std::endl;

	it = l.begin();
	while(k != 0) {
	  k++;
	  it++;
	}
	l.erase(++it);
  }
  return;
}

void delete_backwards1(std::list<int>& l, int k) {
  if((int)l.size() < k || k < 1) return; //k至少为1
  if((int)l.size() == k) l.pop_front();
  auto it = l.begin();
  while(it != l.end()) {
	k--;
	it++;
  }
  
  auto it1 = l.begin();
  while(k != 0) {
	it1++;
	k++;
  }
  l.erase(it1);
  std::cout<<"rm = "<<*it1<<std::endl;
}

int main()
{
  std::list<int> l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);
  l.push_back(4);
  l.push_back(5);
  l.push_back(6);
  print_list(l);
  delete_backwards1(l, 2);
  print_list(l);

  return 0;
}

int main1()
{
  std::list<int> l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);
  l.push_back(4);
  l.push_back(5);
  l.push_back(6);
  std::cout<<"input backwards:";
  char K[2] = {0};
  std::cin>>K;
  int count = atoi(K);
  std::cout<<"K="<<count<<std::endl;
  delete_backwards(l, count);

  return 0;
}

//有什么意义
