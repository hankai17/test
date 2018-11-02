#include "sync_queue.hh"
#include "thread_pool.hh"

void test_thpool() {
  thread_pool pool;
  //pool.Start(2); //调用start就会起两个RunInThread线程
                 //线程死循环 取队列中任务 调用

  this_thread::sleep_for(std::chrono::seconds(2));
  std::thread t1([&pool] {
        //for(auto i : range(0,9))
        for(int i = 0; i < 10; i++ ) { //往pool里添加10个任务/回调
        auto th_id = this_thread::get_id();
        pool.AddTask([th_id] {
              std::cout<<"同步层线程1的线程id="<<th_id<<std::endl;
              }
              );
        }
        });
/*
  std::thread t2([&pool] {
        //for(auto i : range(0,9))
        for(int i = 0; i < 10; i++ ) {
        auto th_id = this_thread::get_id();
        pool.AddTask([th_id] {
              std::cout<<"同步层线程2的线程id="<<th_id<<std::endl;
              }
              );
        }
        });
*/
  this_thread::sleep_for(std::chrono::seconds(2));
  getchar();
  pool.Stop();
  t1.join();
  //t2.join();
  return;
}


int main()
{
  test_thpool();
  return 0;
}

/*
所谓线程池 就是pool里面有几个线程(硬件资源有关个数) 不断的取同步队列中的任务 而调用者只需要把任务塞给队列即可
总结:
线程池底层中必须有同步队列 而上层必须有多个线程组成的池 以及一个添加任务的接口

所谓池就是回调中就是一个死循环 不断的消耗同步队列中的任务

*/

