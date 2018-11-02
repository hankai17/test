#pragma once
#include <thread>
#include <condition_variable>
#include <mutex>
#include <list>
#include <iostream>

template<typename T>
class SimpleSyncQueue {
  public:
	SimpleSyncQueue(){}

	void Put(const T& x) {
	  std::lock_guard<std::mutex> locker(m_mutex);
	  m_queue.push_back(x);
	  m_notEmpty.notify_one();
	}

	void Take(T& x) {
	  std::unique_lock<std::mutex> locker(m_mutex); //用的是unique_lock + condition_variable 语义更明确
	  m_notEmpty.wait(locker, [this]{return !m_queue.empty(); }); //即条件变量的wait有一个方法 可以接受一个条件，若条件为真则向下执行
	  x = m_queue.front();
	  m_queue.pop_front();
	}

	bool Empty() {
	  std::lock_guard<std::mutex> locker(m_mutex);
	  return m_queue.empty();
	}

	size_t Size() {
	  std::lock_guard<std::mutex> locker(m_mutex);
	  return m_queue.size();
	}

  private:
	std::list<T> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_notEmpty;
};
//condition_variable_any和任意带有lock unlock语义的mutex搭配使用 比较灵活 但是效率比condition_variable差一些


