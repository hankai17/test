#include "multiplex.hh"
#include <iostream>
#include "namespaces.hh"

class SelectMultiplexer : public FDMultiplexer {
  public:
	SelectMultiplexer() {}
	virtual ~SelectMultiplexer() {}
	virtual int run(struct timeval* tv);
	virtual void add_fd(callbackmap_t& cbmap, int fd, callbackfunc_t todo, const funcparam_t& para);
	virtual void remove_fd(callbackmap_t& cbmap, int fd);
	std::string get_name() { return "select"; }
};

