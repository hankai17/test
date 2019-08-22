#include<iostream>
#include<string>
#include<sys/time.h>
#include "mtasker.hh"
#include "socket.hh"
#include "server.hh"
#include "async_server.hh"
#include "multiplex.hh"
#include "select_mplexer.hh"
#include "epoll_mplexer.hh"
#include "server_frame.hh"
#include "event.hh"

struct timeval g_now;
using MT_t = MTasker<RWEvent>;
MT_t* MT; 
FDMultiplexer* t_fdm;

void send_event(RWEvent& e) {
  cout<<"begin send_event cb"<<endl;
  MT->sendEvent(e); 
  cout<<"end send_event cb"<<endl;
}


int main()
{
  MT = new MT_t(20000);
  //t_fdm = new SelectMultiplexer();
  t_fdm = new EpollMultiplexer();
  AsyncServerFrame* s = new AsyncServerFrame(t_fdm, "127.0.0.1", 8001);
  s->go(); //里面只有一句话asny_wait_for_conn 表面上是把fd+收发数据业务处理 挂树上 实际上内部是fd+协程(协程里是收发数据业务处理)
	   //就是为了处理lfd而已 设置专门的回调new_connection里面起协程

  gettimeofday(&g_now, 0);
  vector<RWEvent> wait_events;

  for(;;) {
    while(MT->schedule()); //TODO

	MT->getEvents(wait_events);
	for (vector<RWEvent>::const_iterator i = wait_events.begin(); i != wait_events.end(); i++) {
	  if (i->what == RWEvent::Read && i->who->get_handle() != s->d_fdm->lfd) {
		RWEvent e;
		e.what = RWEvent::Read;
		e.who = i->who;
		t_fdm->add_read_fd(i->who->get_handle(), boost::bind(send_event, e));
	  } else if (i->what == RWEvent::Write) {
		RWEvent e;
		e.what = RWEvent::Write;
		e.who = i->who;
		t_fdm->add_write_fd(i->who->get_handle(), boost::bind(send_event, e));
	  }
	}

    t_fdm->run(&g_now); //一旦有链接读到来 起协程accept读写数据业务处理3剑客一口气 一旦读写数据发生阻塞waitEvent
    //都集成到run()里了
    //所有模型都是只起一个协程  然后读写事件来后只回调一下
    //pdns即是服务器又是客户端 so 在run里有回调  我这里只是一个服务器不需要回调
	
	//run里是回调 起协程
    	

  }
  return 0;
}

/*
   int main1()
   {
//Server* s = new AsyncServer("127.0.0.1", 8001);
MT = new m_MT(20000);
t_fdm = new SelectMultiplexer();
AsyncServerFrame* s = new AsyncServerFrame(t_fdm, "127.0.0.1", 8001);
s->go(); //注册处理链接函数

gettimeofday(&g_now, 0);

for(;;) {
while(MT->schedule()); //&g_now TODO
t_fdm->run(&g_now);
}

return 0;
}
*/
