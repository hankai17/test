#pragma once
#include "socket.hh"
#include "event.hh"
#include "mtasker.hh"

#define BUFSIZE 1024

extern MTasker<RWEvent>* MT;

class MIOBuffer {
  public:
    MIOBuffer(Socket* sock): d_socket(sock) {   
      d_buffer = new char[1024];
      d_pos = d_available = 0;
    }
    ~MIOBuffer() {
      delete[] d_buffer;
    }
    int get_char();
    void read_line(string& line);
    void write_line(const string& line);
    void write(const char* buf, unsigned int to_write);

  private:
    char* d_buffer;
    Socket* d_socket;
    size_t d_pos, d_available;
};

class StrBuffer {
  public:
    StrBuffer(Socket* sock): d_socket(sock) {
      d_buffer = std::string("");
      d_pos = d_available = 0;
    }
    ~StrBuffer() {
    }
    

  private:
    std::string d_buffer;
    Socket* d_socket;
    size_t d_pos, d_available;
};

class Buffer {
  public:
    Buffer(Socket* sock): d_socket(sock) {
      d_buffer = new char[BUFSIZE];
      buf_len =  BUFSIZE;
      data_len =  0;
    }
    ~Buffer() {
      delete[] d_buffer;
    }

    size_t buf_free_space() { return buf_len - data_len; }
    size_t buf_data_size() { return data_len; }
    char* buf_end_position() { return d_buffer + data_len; }

    size_t clear_space() {
      d_buffer[0] = '\0';
      data_len = 0;
      return 0;
    }

    int buf_pick(size_t len) {
      assert(data_len >= len);
      data_len -= len;
      if(data_len > 0) {
        memmove(d_buffer, d_buffer + len, data_len);
      }
      return 0;
    }
    
    int buf_realloc(size_t len) {
      char* n = (char*)malloc(buf_len + len);
      memcpy(n, d_buffer, data_len);
      free((void *)d_buffer);
      d_buffer = n;
      buf_len += len;
      return 0;
    }
    
    int buf_put(char* data, size_t len) {
      if(buf_free_space() < len ) {
        /* no recommended to do this, so give some warning here */
        if(buf_realloc(len) < 0) return -1;
      }
      memcpy(d_buffer + data_len, data, len);
      data_len += len;
      return 0;
    }
   
    //The below is upper read-write
    /*
    int read(std::string& str) {
      line.clear();
      int c;
      int r;
      int t = 0;
      //if(sock < 0 ) return -1; //TODO
      while(buf_free_space() > 0) {
	RWEvent e;
	e.who = d_socket;
	e.what = RWEvent::Read;
	if(!MT->waitEvent(e, 0, 5)) throw NetWorkError("Timeout!"); //这个函数要是调用的话 一般是在schedule函数里 schedule一共有三个队列 一般是第一个队列或者第三个队列 如果没有赶上13队列 而且还没有超时那就等run了
        r = read(s->server_sock, buf_end_position(s->out_buf), buf_free_space(s->out_buf));
        if(r < 0) {
                if(errno == EINTR ) continue;
                if(errno == EWOULDBLOCK || errno == EAGAIN) {
                        return t;
                } else {
                        return -1;
                }
        } else if( r == 0) { //peer active closed. Its possiable that there were remain datas in buf !
                s->flags.server_closed = 1;
                return -1;
        } else {
                s->out_buf->data_len += r;
                t += r;
                //return t;
                return t;
        }
      }
      s->data_in += t;
      return t;
    }
    */

    int read(string& line) { //每次read后 传到上层http解析库 上层得有状态机 如果解析不完 还得必须监听
      line.clear();
      int c;
      for(;;) {
	RWEvent e;
	e.who = d_socket;
	e.what = RWEvent::Read;
	if(!MT->waitEvent(e, 0, 5)) throw NetWorkError("Timeout!");
	int res = d_socket->read(d_buffer, BUFSIZE);
	if(!res) return -1;
	data_len = res;
        line.assign(d_buffer, res);
        break;
      }
    }

    void write(const char* buf, unsigned int to_write) {
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

  private:
    char* d_buffer;
    size_t buf_len;
    size_t data_len;
    Socket* d_socket;
};

