#include "miobuffer.hh"

extern MTasker<RWEvent>* MT;

int MIOBuffer::get_char() {
  if(d_pos == d_available) {
	RWEvent e;
	e.who = d_socket;
	e.what = RWEvent::Read;
	if(!MT->waitEvent(e, 0, 5)) throw NetWorkError("Timeout!"); //这个函数要是调用的话 一般是在schedule函数里 schedule一共有三个队列 一般是第一个队列或者第三个队列 如果没有赶上13队列 而且还没有超时那就等run了
	int res = d_socket->read(d_buffer, 1024);
	if(!res) return -1;
	d_available = res;
	d_pos = 0;
  }
  return d_buffer[d_pos++];
}

void MIOBuffer::read_line(string& line/*out*/) {
  line.clear();
  int c;
  for(;;) {
	c = get_char();
	if(c == -1) break;
	line.append(1, (char)c);
	if(c == '\n') break;
  }
}

void MIOBuffer::write(const char* buf, unsigned int to_write) {   
  int written = 0;
  int res = 0;
  RWEvent e;
  e.who = d_socket;
  e.what = RWEvent::Write;

  do {
	res = d_socket->try_write(buf + written, to_write);
	if(!res) { //写入0个 与 eagain 则都返回0 说明有问题 则开始等待
	  MT->waitEvent(e);
	  continue;
	}
	written += res;
	to_write -= res;
  } while(to_write);
}

void MIOBuffer:: write_line(const string& line) { //写到本对象的成员d_socket中
  write(line.c_str(), line.size());
}

