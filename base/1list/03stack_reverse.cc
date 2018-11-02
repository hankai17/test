#include<iostream>
#include<stack>

//因为stack容器 只能pop之后才能获取下一个元素 而这样就破坏了原始的stack容器 
//so 直觉 用递归把pop出的元素 暂存到return val 这个val中去
//直觉很简单 思路也正确 但是写起来很难


//此题可以改为 只用一个栈如何逆序其本身 
//so 关键点来了：如何获取最后一个元素 而且是递归的获取最后一个元素

//总结了一下 3要素
//递归 做什么 不断的pop
//停止点 size为0 递归"停止"
//触底点 做什么 触底反弹 把它返出去 把上一个值压入


/*
走一遍 这个函数明显破快了栈结构 只是返回最后一个元素 其它元素依次再塞进去
*/
int get_last_val(std::stack<int>& s) {
  int result = s.top();
  s.pop();
  //std::cout<<"result = "<<result<<std::endl;
  if (s.size() == 0) {
	return result;
  }
  int last_val = get_last_val(s);
  s.push(result);
  return last_val;
}

/*
不要被reverse这个名字给迷惑了 这个函数就是不断的获取最后一个元素 最最后再push到原始stack中
也就是说 只用了一个stack 就可以把stack给逆序
*/
void reverse(std::stack<int>& s) {
  if(s.size() == 0) {
	return;
  }
  int last_val = get_last_val(s);
  std::cout<<"last_val = "<<last_val<<std::endl;
  reverse(s);
  s.push(last_val);
}

int main() {
  std::stack<int> s;
  s.push(5);
  s.push(4);
  s.push(3);
  s.push(2);
  s.push(1);

  reverse(s);

  while (!s.empty()) {
	std::cout<<s.top()<<std::endl;
	s.pop();
  }
  return 0;
}
