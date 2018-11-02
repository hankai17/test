#pragma once
#include <string>
#include <functional>
#include <map>
#include "any.hh"
#include "function_traits.hh"
#include "non_copy.hh"

using namespace std;

class MessageBus: NonCopyable {
  public:
	template<typename F> 
    void Attach(F&& f, const string& str_topic = "") { //注册消息 即添加topic以及回调
	  auto func = to_function(std::forward<F>(f)); //stl函数包装器
	  Add(str_topic, std::move(func));
	}

	template<typename R> 
    void SendReq(const string& str_topic = "") { //根据topic回调
	  using function_type = std::function<R()>;
	  string str_msg_type =str_topic+ typeid(function_type).name();
	  auto range = m_map.equal_range(str_msg_type);
	  for (Iterater it = range.first; it != range.second; ++it) {
		auto f = it->second.any_cast < function_type >();
		f();
	  }
	}

	template<typename R, typename... Args> //根据返回值和参数回调
    void SendReq(Args&&... args, const string& str_topic = "") {
	  using function_type = std::function<R(Args...)>;
	  string str_msg_type = str_topic+ typeid(function_type).name();
	  auto range = m_map.equal_range(str_msg_type);
	  for (Iterater it = range.first; it != range.second; ++it) {
		auto f = it->second.any_cast < function_type >();
		f(std::forward<Args>(args)...);
	  }
	}

	template<typename R, typename... Args> void Remove(const string& str_topic = "") { //移除某个主题 需要主题和消息类型
	  using function_type = std::function<R(Args...)>; //typename function_traits<void(CArgs)>::stl_function_type;
	  string str_msg_type =str_topic +typeid(function_type).name();
	  int count = m_map.count(str_msg_type);
	  auto range = m_map.equal_range(str_msg_type);
	  m_map.erase(range.first, range.second);
	}

  private:
	template<typename F> void Add(const string& str_topic, F&& f) {
	  string str_msg_type = str_topic + typeid(F).name();
	  m_map.emplace(std::move(str_msg_type), std::forward<F>(f));
	}

  private:
	std::multimap<string, Any> m_map;
	typedef std::multimap<string, Any>::iterator Iterater;
};
