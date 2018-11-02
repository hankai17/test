#include <iostream>

template <typename T>
T* Instance() {
  return new T();
}

template <typename T, typename T0>
T* Instance(T0 arg0) {
  return new T(arg0);
}

template <typename T, typename T0, typename T1>
T* Instance(T0 arg0, T1 arg1) {
  return new T(arg0, arg1);
}

template <typename T, typename T0, typename T1, typename T2>
T* Instance(T0 arg0, T1 arg1, T2 arg2) {
  return new T(arg0, arg1, arg2);
}

template <typename T, typename T0, typename T1, typename T2, typename T3>
T* Instance(T0 arg0, T1 arg1, T2 arg2, T3 arg3) {
  return new T(arg0, arg1, arg2, arg3);
}

template <typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
T* Instance(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
  return new T(arg0, arg1, arg2, arg3, arg4);
}

struct A {
  A(int){}
};

struct B {
  B(int, double){}
};

struct C {
  C(int, double, string){}
};


template<typename T, typename... Args>
//T* Instance1(Args... args) { //值拷贝 有损性能
T* Instance1(Args&&... args) {
	//return new T(args...);
	return new T(std::forward<Args> (args)...);
}

int main1(void) {
  A* pa = Instance<A>(1);
  B* pb = Instance<B>(1, 2);
  C* pc = Instance<C>(1, 2, "");
  return 0;
}

int main(void) {
  A* pa = Instance1<A>(1);
  B* pb = Instance1<B>(1, 2);
  return 0;
}
