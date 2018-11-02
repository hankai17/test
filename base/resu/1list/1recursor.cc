#include<iostream>

int recursor_add(int n) {
  if (n == 1) {
    return 1;
  }
  return recursor_add(n-1) + n;
}

int main()
{
  std::cout<<"sum = "<<recursor_add(100)<<std::endl;
  return 0;
}
