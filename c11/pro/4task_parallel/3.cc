#include <iostream>
#include <functional>
#include <type_traits>
#include<future>
#include<vector>
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

class TaskGroup {
public:
  TaskGroup() {}
  ~TaskGroup() {}
  void Run(Task<void()>&& task) {
	m_voidGroup.push_back(task.Run());
  }
  template<typename F> void Run(F&& f) {
    typedef typename std::result_of<F()>::type ReturnType;
	Run(Task<ReturnType()> (std::forward<F>(f)));
  }
  template<typename F, typename... Funs> void Run(F&& first, Funs&&... rest) {
	Run(std::forward<F>(first));
	Run(std::forward<Funs>(rest)...);
  }
  void Wait() {
	for (auto it = m_voidGroup.begin(); it != m_voidGroup.end(); it++) {
	  it->get();
	}
  }
private:
	vector<std::shared_future<void>> m_voidGroup;
};

void test() {
  TaskGroup g;
  std::function<void()> f = []{cout<<"ok0"<<endl;};
  auto f1 = []{cout<<"ok1"<<endl;};
  g.Run(f); //0
  g.Run(f, f1, []{cout<<"ok2"<<endl;}); //0 1 2
  g.Wait();
}

int main() {
  test();
  return 0;
}

