#include<iostream>
using namespace std;

//cin  istream类型的对象

//1cin.get()
void test01() {
  char ch = cin.get();
  cout << "ch:" << ch << endl;

  ch = cin.get();
  cout << "ch:" << ch << endl;

  cin.get(ch);
  cout << "ch:" << ch << endl;

  char c1, c2, c3;
  cin.get(c1).get(c2).get(c3);

  cout << "c1:" << c1 << endl;
  cout << "c2:" << c2 << endl;
  cout << "c3:" << c3 << endl;
}

//2 按行读取
void test02() {
  char buf[1024] = {0};
#if 0
  //没有读取换行
  cin.get(buf, 1024);
  cout << buf;
  char ch = cin.get();
  if (ch == '\n'){
	cout << "换行留在缓冲区了!" << endl;
  }
  cout << ch;
#else
  //读取换行，并且扔掉换行
  cin.getline(buf, 1024);
  cout << buf;
  char ch = cin.get();
  if (ch == '\n') {
	cout << "换行留在缓冲区了!" << endl;
  }
  cout << ch;
#endif
}

//cin.ignore 忽略当前字符
void test03() {
  //当我输入:abc 输出结果?
  //cin.ignore(); //忽略当前一个字符
  cin.ignore(2);
  char ch = cin.get();
  cout << ch << endl;
}


//cin.putback 把字符放回到缓冲区当前位置
void test04(){

  char ch = cin.get();

  //字符ch放回到缓冲区
  cin.putback(ch);

  char buf[1024] = { 0 };
  cin.getline(buf,1024);

  cout << buf;

}

//cin.peek 偷窥
void test05(){

  char ch = cin.peek();
  cout << "ch:" << ch << endl;
  char buf[1024] = { 0 };
  cin.getline(buf, 1024);
  cout << buf;
}

void test06(){

  cout << "请输入一个数字或者字符串:" << endl;

  char ch = cin.peek();

  if (ch >= '0' && ch <= '9'){
	int number;
	cin >> number;

	cout << "输入的是数字:" << number << endl;
  }
  else{

	char buf[1024] = { 0 };
	cin.getline(buf,1024);
	cout << "输入的是字符串:" << buf << endl;
  }

}

//课堂练习: 使用cin.get和cin.putback完成test06相同的功能!


int main(){

  //test01();
  //test02();
  //test03();
  //test04();
  //test05();
  //test06();

  system("pause");
  return EXIT_SUCCESS;
}
