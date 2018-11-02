#include<iostream>

using namespace std;

int GetValue(int n) {
	int i = 0; //i统计遇到了多少次下跌
	int j = 2; //每次下跌之后上涨的天数，包含已经下跌的那天
	int k = n;
	while (k > j) {
		i += 2;
		k -= j;
		++j;
	}
	return n - i;
}

int sum_money(int day) {
  if (day == 1) return 1;
  if (day <= 0) return 0; 
 
  int i = 0; 
  int sum = 0;
  int factor = 0;
  int tmp;

  while(i < day) {
	tmp = i;
	if(i == 1) {
	  factor = 1;
	} else if (i > 1 && i < tmp + i) {
		factor = -1;
    }
	i++;
	sum += factor;
  }
  return sum;
}

//1 2 3 4 5

//0 1 - 1 -
//1 2 1 2 3


int is_postive(int n) {
  if(n == 1) return 0;
  
}

int main() {
  int day = 10;
  //int ret = sum_money(day);
  int ret = GetValue(day);
  cout<<ret<<endl;
  return 0;
}
