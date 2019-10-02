#include <iostream>  
#include<memory>

using namespace std;  

class D: public std::enable_shared_from_this<D>  {  
  public:  
    D() {
      cout<<"D::D()"<<endl;
    }

    void func() {
      cout<<"D::func()"<<endl;
      shared_ptr<D> p = shared_from_this(); //shared_from_this底层是把weakptr提升到shared_ptr 会+1
      cout<<"p use_count: "<<p.use_count()<<endl; //2

      shared_ptr<D> p1 = shared_from_this();
      cout<<"p use_count: "<<p1.use_count()<<endl; //3

      cout<<"use_cout: "<<shared_from_this().use_count()<<endl; //4 //最好不要用这种方式打印
      cout<<"p use_count3: "<<p1.use_count()<<endl; //3
      cout<<"use_cout: "<<shared_from_this().use_count()<<endl; //4
    }

    void test() {
      cout<<"use_cout: "<<shared_from_this().use_count()<<endl;
    }
};

void print_use_count(shared_ptr<D>& p) {
  cout<<"p.use_count: "<<p.use_count()<<endl;
}

int main() {
  shared_ptr<D> p(new D);
  //1
  p->func();
  print_use_count(p);
  return 0;      
} 
