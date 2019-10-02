#include <iostream>
#include <functional>
#include<memory>
#include<list>

using namespace std;
using msgCallback = std::function<void()>;

class A : public std::enable_shared_from_this<A> {
  public:
    ~A() { 
      cout << "A::~A()" << endl; 
    }
    void output() {
      cout<< "use_count: "<<shared_from_this().use_count()<<endl;
      cout << "A::output()" << endl;
    }
    void test() {
      cout<< "use_count: "<<shared_from_this().use_count()<<endl;
    }
};

class B {
  public:
    ~B() {
      cout << "B::~B()" << endl;
    }
    void setCallback(const msgCallback& cb) {
      if (m_cb == nullptr)
      m_cb = cb;
    }
    void resetCallback() {
      m_cb = nullptr;
    }
  private:
    msgCallback m_cb;
};

int main() {
  shared_ptr<B> spb = make_shared<B>();
  {
    shared_ptr<A> spa = make_shared<A>();   //use_count:1

    list<shared_ptr<A>> lst;
    lst.push_back(spa);     //use_count:2
    cout << "count1: " << spa.use_count() << endl;
    {
      std::function<void()> f = std::bind(&A::output, spa);   //use_count:3
      spb->setCallback(f);    //use_count:4
      cout << "count2: " << spa.use_count() << endl; //---------------------4
      spa->output();
    }   //use_count:3
    cout << "count3: " << spa.use_count() << endl; //--------------------3  because 'f' is dead.  3 = spa list spb->cb

    if (!lst.empty())
    lst.erase(lst.begin());     //use_count:2
    cout << "count4: " << spa.use_count() << endl; //------------------2 = spa spb->cb
  }   //use_count:1

  return 0;
}


int main1() {
  {
    shared_ptr<A> spa = make_shared<A>();   //use_count:1
    shared_ptr<B> spb = make_shared<B>();

    list<shared_ptr<A>> lst;
    lst.push_back(spa);     //use_count:2
    cout << "count1: " << spa.use_count() << endl;    //--------------------2
    {
      std::function<void()> f = std::bind(&A::output, spa);   //use_count:3
      spb->setCallback(f);    //use_count:4
      cout << "count2: " << spa.use_count() << endl; //---------------------4
    }   //use_count:3  because of 'f' is over
    cout << "count3: " << spa.use_count() << endl;    //--------------------3 = spa list spb->cb

    if (!lst.empty())
    lst.erase(lst.begin());     //use_count:2
    cout << "count4: " << spa.use_count() << endl;      //-----------------2 = spa spb->cb

    spb->resetCallback();   //use_count:1            //----------------1 = spa
    cout << "count5: " << spa.use_count() << endl;
  }   //use_count:0

  return 0;
}
