#include <iostream>
#include <functional>
#include <type_traits>
#include <future>
#include <string>
using namespace std;

template<typename T> class Task; //可变参数模板类
template<typename Ret, typename...Args>
class Task<Ret(Args...)> {
  public:
  typedef Ret return_type;
  Task(std::function<Ret(Args...)>&& f) : m_fn(std::move(f)) {}
  Task(std::function<Ret(Args...)>& f) : m_fn(f) {}
  ~Task() {}

  template<typename F> 
  auto Then(F&& f)->Task<typename std::result_of<F(Ret)>::type(Args...)> { //实现链式调用
	typedef typename std::result_of<F(Ret)>::type ReturnType;
    auto func = std::move(m_fn);
	return Task<ReturnType(Args...)> ([func, &f](Args&&... args) { //func是之前的函数  f是当前的函数
	  std::future<Ret> lastf = std::async(func, std::forward<Args>(args)...); //将之前的函数保存起来
	  return std::async(f, lastf.get()).get(); //返回当前的函数 当前中包含了上次的函数将上次函数运行结果作为参数
    });
  }
  
  void Wait() { //一直阻塞等待操作
    std::async(m_fn).wait();
  }
  Ret Get(Args&&... args) { //阻塞等待结果
    return std::async(m_fn, std::forward<Args>(args)...).get();
  }
  std::shared_future<Ret> Run() {
    return std::async(m_fn);
  }
  private:
	std::function<Ret(Args...)> m_fn;
};

void test() {
/*
  Task<int()> t([]{return 32;});
  auto r1 = t.Then([](int result) { //t的输出为Then的输入
    cout<<result<<endl; return result + 3;
  }).Then([](int result) {
	cout<<result<<endl; return result + 3; //Then将task串起来
  }).Get(); //最后在需要结果的时候get即可 计算过程是链式的
  cout<< r1 <<endl; //38
*/
  Task<int(int)> t1([](int i) {return i;});
  t1.Then([](int result) {
	return std::to_string(result);
  }).Then([](const string& str) {
	cout<< str <<endl;
    return 0;
  }).Get(100);

  Task<string(string)> t2([](string str){return str;});
  //string in = "test";
  auto r2 = t2.Then([](const string& str) {
	cout<< str.c_str() <<endl;
	return str + " ok ";
  }).Get("test");
  cout << r2 << endl;
}

int main() {
  test();
  return 0;
}
