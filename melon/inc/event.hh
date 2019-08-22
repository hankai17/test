#pragma once
#include "socket.hh"

class RWEvent {
  public:
	enum {Read, Write} what;
	Socket* who;
	bool operator<(const RWEvent& b) const;

	/*
	   bool operator<(const RWEvent& b) {
	   if(this->what < b.what) return true;
	   if(this->what > b.what) return false;
	   if(this->who < b.who) return true;
	   return false;
	   }
	   bool operator<(const RWEvent& a, const RWEvent& b) {
	   if(a.what < b.what) return true;
	   if(a.what > b.what) return false;
	   if(a.who < b.who) return true;
	   return false;
	   }
	   */
};

