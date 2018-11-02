#include <iostream>
#include <functional>
#include <type_traits>
#include<future>
using namespace std;

template<typename T> class Task; //可变参数模板类
template<typename Ret, typename...Args>
class Task<Ret(Args...)> {
  public:
  typedef Ret return_type;

  Task(std::function<Ret(Args...)>&& f) : m_fn(std::move(f)) {}
  Task(std::function<Ret(Args...)>& f) : m_fn(f) {}
  ~Task() {}
  
  void Wait() { //一直阻塞等待操作 无返回值
    std::async(m_fn).wait();
  }

  Ret Get(Args&&... args) { //获取阻塞等待结果
    return std::async(m_fn, std::forward<Args>(args)...).get();
  }

  std::shared_future<Ret> Run() { //发起异步操作  //注意返回值 std::future是不能复制的 必须用shared_future
    return std::async(m_fn);
  }
  private:
	std::function<Ret(Args...)> m_fn;

};

void test() {
  std::future<int> f = std::async(std::launch::async, []() { //发起一个异步操作 由其内部线程做处理 我不用关心
	return 8;
  });
  std::cout<<f.get()<<std::endl;
  
}

void test1() {
  Task<int(int)> t([](int i){return i + 99;});
  //t.Run();
  auto r = t.Get(1);
  cout<< r <<endl;

  Task<int()> t1([](){return 99;});
  auto r1 = t1.Run().get();
  cout<< r1 <<endl;
}

int main() {
  test1();
  return 0;
}
