#include "event.hh"

bool RWEvent::operator<(const RWEvent& b) const {
  if(this->what < b.what) return true;
  if(this->what > b.what) return false;
  if(this->who < b.who) return true;
  return false;
}

