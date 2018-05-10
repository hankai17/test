#include<iostream>

using namespace std;

template<class T>  
class AutoPtr {  
  public:  
	AutoPtr(T* ptr = 0) :_ptr(ptr) {
	  ptr = NULL;  
	}  

	AutoPtr(AutoPtr<T>& ap) :_ptr(ap._ptr) {  
	  ap._ptr = NULL;  
	}  

	AutoPtr<T>& operator=(AutoPtr<T>& ap) {  
	  if (this != &ap) {  
		if (_ptr) {  
		  delete _ptr;  
		}  
		_ptr = ap._ptr;  
		ap._ptr = NULL;  
	  }  
	  return *this;  
	}  

	~AutoPtr() {  
	  if (_ptr) {  
		delete _ptr;  
		_ptr = NULL;  
	  }  
	}  

	T& operator*() {  
	  return *_ptr;  
	}  

	T* operator->() {  
	  return _ptr;  
	}  
  private:  
	T* _ptr;  
};  

void Test1() {  
  AutoPtr<int> ap(new int(1));  
  AutoPtr<int> ap1(ap);  
  AutoPtr<int> ap2;  
  ap2 = ap1;  
}

AutoPtr<int> test2() {
  AutoPtr<int> ap(new int(1));
  return ap;
} //返回一个元素值 具有常量性

int main() {
  //Test1();
  //AutoPtr<int>ap4(test2()); //下面也是用右值初始化 因为类中没有这个右值的拷贝构造函数 所以报错
  AutoPtr<int>ap5(AutoPtr<int> (new int(1)));
  return 0;
}

