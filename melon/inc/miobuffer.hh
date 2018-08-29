#pragma once
#include "socket.hh"
#include "event.hh"
#include "mtasker.hh"

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

