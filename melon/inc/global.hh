#pragma once
#include"namespaces.hh"
#include"socket.hh"
#include"event.hh"

extern void func_test(int fd, boost::any args);
extern int arecvtcp(string& data, size_t len, Socket* sock, bool incomplete_oky);
extern int asendtcp(const string& data, Socket* sock);
//extern bool operator<(const RWEvent& a, const RWEvent& b);
