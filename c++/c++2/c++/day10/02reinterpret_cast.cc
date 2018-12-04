#include<iostream>
using namespace std;

class My {};
class You {};

void test() {
  My* m = NULL;
  You* y = NULL;
  You* y1 = reinterpret_cast<You*>(m);
  int a = 10;
  int* ap = reinterpret_cast<int*>(a);
}

int main() {
}

