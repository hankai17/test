#include "messagebus.hh"

//topic+回调对象 作为key
void test() {
  MessageBus bus;
  bus.Attach([](int a){cout<<"no reference "<<a<<endl;});
  bus.Attach([](int& a){cout<<"l reference "<<a<<endl;});
  bus.Attach([](int&& a){cout<<"r reference "<<a<<endl;});
  bus.Attach([](const int& a){cout<<"const l reference "<<a<<endl;});
  bus.Attach([](int a){cout<<"no reference "<<a<<endl; return a;}, "a");

  int i = 2;
  bus.SendReq<void, int>(2); //no
  bus.SendReq<int, int>(2, "a"); //no ??
  bus.SendReq<void, int&>(i); //l
  bus.SendReq<void, const int&>(2); //const l
  bus.SendReq<void, int&&>(2); //r
/*
  bus.Remove<void, int>();
  bus.Remove<int, int>("a");
  bus.Remove<void, int&>();
  bus.Remove<void, const int&>();
  bus.Remove<void, int&&>();

  bus.SendReq<void, int>(2); 
  bus.SendReq<int, int>(2, "a"); 
  bus.SendReq<void, int&>(i); 
  bus.SendReq<void, const int&>(2); 
  bus.SendReq<void, int&&>(2); 
*/
}

int main() {
  test();
  return 0;
}

