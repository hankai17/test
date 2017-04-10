#include "sync_queue.hh"
#include "thread_pool.hh"

void test_thpool()
{
	thread_pool pool;
	pool.Start(2);

	std::thread t1([&pool]
			{
			//for(auto i : range(0,9))
			for(int i = 0; i < 10; i++ )
			{
			auto th_id = this_thread::get_id();
			pool.AddTask([th_id] 
					{
					std::cout<<"同步层线程1的线程id="<<th_id<<std::endl;
					}
				    );
			}
			});

	std::thread t2([&pool]
			{
			//for(auto i : range(0,9))
			for(int i = 0; i < 10; i++ )
			{
			auto th_id = this_thread::get_id();
			pool.AddTask([th_id] 
					{
					std::cout<<"同步层线程2的线程id="<<th_id<<std::endl;
					}
				    );
			}
			});
	this_thread::sleep_for(std::chrono::seconds(2));
	getchar();
	pool.Stop();
	t1.join();
	t2.join();
	return;
}


int main()
{
	test_thpool();
	return 0;
}
