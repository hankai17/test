#include <iostream>
#include <utility>
#include <functional>
#include <atomic>

#include <stdio.h>
#include <stdlib.h>
#include <chrono>    // std::chrono::seconds
#include <thread>    // std::thread, std::this_thread::sleep_for

#if 1

void f1(int n) {
  for (int i = 0; i < 5; ++i) {
	std::cout << "f1 " << n << "\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void f2(int& n) {
  for (int i = 0; i < 5; ++i) {
	std::cout << "f2 "<<n << "\n";
	++n;
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

int main()
{
  int n = 0;
  std::thread t1; // t1 is not a thread
  std::thread t2(f1, n + 1); // pass by value
  std::thread t3(f2, std::ref(n)); // pass by reference
  //t3.detach();
  std::thread t4(std::move(t3)); // t4 is now running f2(). t3 is no longer a thread
  t2.join();
  t4.join();
  std::cout << "Final value of n is " << n << '\n';
}

#else

void thread_task(int n) {
  std::this_thread::sleep_for(std::chrono::seconds(n));
  std::cout << "thread "
	<< std::this_thread::get_id()
	<< " paused " << n << " seconds" << std::endl;
}

int main(int argc, const char *argv[])
{
  std::thread threads[5];
  std::cout << "Spawning 5 threads...\n";
  for (int i = 0; i < 5; i++) {
	threads[i] = std::thread(thread_task, i + 1);
  }
  std::cout << "Done spawning threads! Now wait for them to join\n";
  for (auto& t: threads) {
	t.join();
  }
  std::cout << "All threads joined.\n";
  return 0;
}

#endif
