#include"global.hh"
#include"mtasker.hh"
#include"multiplex.hh"

void func_test(int fd, boost::any args) {
  std::cout<<fd<<" hello world"<<endl;
}
extern MTasker<RWEvent>* MT;
extern FDMultiplexer* t_fdm;

// -1 is error, 0 is timeout, 1 is success
int asendtcp(const string& data, Socket* sock) {
  int fd = sock->get_handle();
  t_fdm->add_write_fd(fd, func_test);
  string packet;
  //int ret = MT->waitEvent(fd, &packet); //in time TODO
  RWEvent e;
  e.what = RWEvent::Write;
  e.who = sock;
  int ret = MT->waitEvent(e); //in time TODO
  if(!ret || ret == -1) { // timeout
    t_fdm->remove_write_fd(fd);
  } else if(packet.size() != data.size()) { // main loop tells us what it sent out, or empty in case of an error
    return -1;
  }
  return ret;
}

int arecvtcp(string& data, size_t len, Socket* sock, bool incomplete_oky) {
  data.clear();
  int fd = sock->get_handle();
  t_fdm->add_read_fd(fd, func_test);
  RWEvent e;
  e.what = RWEvent::Read;
  e.who = sock;
  int ret = MT->waitEvent(e); //in time TODO
  if(!ret || ret == -1) { // timeout
    t_fdm->remove_read_fd(fd);
  } else if(data.empty()) { // error, EOF or other
    return -1;
  }
  return ret;
}
/*
bool operator<(const RWEvent& a, const RWEvent& b) {
  if(a.what < b.what) return true;
  if(a.what > b.what) return false;
  if(a.who < b.who) return true;
  return false;
}
*/
