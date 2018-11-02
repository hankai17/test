#include<iostream>
/*
大梵天创造世界的时候做了三根金刚石柱子 在一根柱子上从下往上按照大小顺序摞着64片黄金圆盘 大梵天命令婆罗门把圆盘从下面开始按大小顺序重新摆放在另一根柱子上
并且规定 在小圆盘上不能放大圆盘 在三根柱子之间一次只能移动一个圆盘
*/
int count;

//这个函数的作用 把a中的1~n-1放到b 把n放到c 把b上的1~n-1放到c 思路正确 但是还是写不出代码
void move(char a, char b) {
  std::cout<<"move: " <<a <<"---> "<<b<<std::endl;
  count++;
  return;
}

//函数整体上是整个汉诺塔完成的功能 即将m个塔从one上把所有塔移动到three上
void hanno_tower(int m, char one, char two, char three) {
  if(m == 1) {
	move(one, three);
  } else {
	hanno_tower(m-1,one,three,two);
	move(one, three);
	hanno_tower(m-1,two,one,three);
	return;
  }
}
//写不出来的原因：(int m, char one, char two, char three) 这种结构设计不出来

int main() {
  int m = 64;
  hanno_tower1(m, 'a', 'b', 'c'); 
  std::cout<<"count = "<<count<<std::endl;
  return 0;
}

//http://www.cnblogs.com/xxNote/articles/3965739.html
//要不背会吧

//递归函数 把m个塔从a移到c
void hanno_tower1(int m, char a, char b, char c) {
  if (m == 1) {
    move(a, c);	
  } else {
	hanno_tower1(m-1, a, c, b);
	hanno_tower1(1, a, b, c);
	hanno_tower1(m-1, b, a, c);
  }
}

