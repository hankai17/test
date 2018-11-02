#include <iostream>
#include <functional>
#include <type_traits>
#include <future>
#include <vector>
#include <map>
#include "Variant.hh"
#include "any.hh"
#include "non_copy.hh"
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
	template<typename R, typename = typename std::enable_if<!std::is_same<R, void>::value>::type> void Run(Task<R()>& task) {
	  m_group.insert(std::make_pair<RetVariant, Any>(R(), task.Run()));
	}   

	void Run(Task<void()>& task) {
	  m_voidGroup.push_back(task.Run());
	}   

	template<typename R, typename = typename std::enable_if<!std::is_same<R, void>::value>::type> void Run(Task<R()>&& task) {
	  m_group.insert(std::make_pair<RetVariant, Any>(R(), task.Run()));
	}   

	void Run(Task<void()>&& task) {       
	  m_voidGroup.push_back(task.Run());
	}           

	template<typename F> void Run(F&& f) {
      typedef typename std::result_of<F()>::type ReturnType;
	  Run(Task<ReturnType()>(std::forward<F>(f)));
	}   

	template<typename F, typename... Funs> void Run(F&& first, Funs&&... rest) {
	  Run(std::forward<F>(first));
	  Run(std::forward<Funs>(rest)...);
	}

	void Wait() {   
	  for (auto it = m_group.begin(); it != m_group.end(); ++it) {
		auto vrt = it->first;
		vrt.Visit([&](int a){FutureGet<int>(it->second); }, [&](double b){FutureGet<double>(it->second); },
			  [&](string v){FutureGet<string>(it->second); }, [&](short v){FutureGet<short>(it->second); },
			  [&](unsigned int v){FutureGet<unsigned int>(it->second); }
			  );
	  }

	  for (auto it = m_voidGroup.begin(); it != m_voidGroup.end(); ++it) {
		it->get();
	  }
	}

  private:
	template<typename T> void FutureGet(Any& f) {
	  f.AnyCast<shared_future<T>>().get();
	  //boost::any_cast<shared_future<T>>(f).get();
	}

	typedef Variant<int, double, string, short, unsigned int> RetVariant;
	multimap<RetVariant, Any> m_group;
	vector<std::shared_future<void>> m_voidGroup;
};

void test() {
  TaskGroup g;
  std::function<int()> f = []{return 1;};
  g.Run(f);
  g.Run(f, []{cout<<"ok1"<<endl;}, []{cout<<"ok2"<<endl;});
  g.Wait();
}

int main() {
  test();
  return 0;
}

