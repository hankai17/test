#include<iostream>

class A {
  public:
	A() :m_ptr(new int(0)) { 
	  std::cout<<"construct"<<std::endl;
	}

	A(const A& a) :m_ptr(new int(*a.m_ptr)) {
	  std::cout<<"cp construct"<<std::endl;
	}

	A(A&& a) :m_ptr(a.m_ptr) { //为什么move构造 只是一个浅拷贝? 因为他是右值 因为它不需要深拷贝 只需要转移控制权限就可以了
	  std::cout<<"move construct"<<std::endl;
	  a.m_ptr = nullptr;
	}

	~A() {
	  std::cout<<"destruct"<<std::endl;
	  delete m_ptr;
	}
  private:
	int* m_ptr;
};

A Get(bool flag) {
  A a;
  A b;
  return flag ? a : b;
}

int main()
{
  A a = Get(false);
  return 0;
}

/*
construct
construct
move construct
destruct
destruct
destruct
*/

/*
g++ 2.3move_base.cc -fno-elide-constructors
./a.out 
construct
construct
move construct
destruct
destruct
move construct
destruct
destruct
[root@90-20 day02]# 
*/

//move可以省去匿名拷贝 也就是说move代替了所有的拷贝 
