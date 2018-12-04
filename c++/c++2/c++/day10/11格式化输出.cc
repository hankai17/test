#include<iostream>
#include<iomanip>
using namespace std;

//1使用cout成员函数格式化输出
void test01() {
  int number = 99;
  cout.width(10);
  cout.fill('*');
  cout.setf(ios::left);
  cout.unsetf(ios::dec); //卸载十进制
  cout.setf(ios::hex); //安装16进制
  cout.unsetf(ios::hex);
  cout.setf(ios::oct);
  cout.setf(ios::showbase);

  cout << number << endl;	
}

//2. 控制符  引入头文件<iomanip>
void test02() {
  int number = 99;
  cout << hex
	<< setfill('@')
	<< setw(10)
	<< setiosflags(ios::showbase)
	<< number
	<< endl;
}

int main() {
  test01();
  //test02();
  return 0;
}
