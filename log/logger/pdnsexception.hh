#ifndef PDNSEXCEPTION_HH
#define PDNSEXCEPTION_HH
#include<string>

#include "namespaces.hh"

class PDNSException
{
public:
  PDNSException(){reason="Unspecified";};
  PDNSException(string r){reason=r;};
  
  string reason; //! Print this to tell the user what went wrong
};

class TimeoutException : public PDNSException
{
public:
  TimeoutException() : PDNSException() {}
  TimeoutException(string r) : PDNSException(r) {}
};

#endif
