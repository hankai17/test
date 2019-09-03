#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <arpa/nameser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ares.h"

struct host_query {
  /* Arguments passed to ares_gethostbyname() */
  ares_channel channel;
  char *name;
  ares_host_callback callback;
  void *arg;

  const char *remaining_lookups;
};

static void next_lookup(struct host_query *hquery);
static void host_callback(void *arg, int status, unsigned char *abuf,
			  int alen);
static void end_hquery(struct host_query *hquery, int status,
		       struct hostent *host);
static int fake_hostent(const char *name, ares_host_callback callback,
			void *arg);
static void sort_addresses(struct hostent *host, struct apattern *sortlist,
			   int nsort);
static int get_address_index(struct in_addr *addr, struct apattern *sortlist,
			     int nsort);

void ares_gethostbyname(ares_channel channel, const char *name, int family, ares_host_callback callback, void *arg) {
  struct host_query *hquery;

  if(family != AF_INET) {
    callback(arg, ARES_ENOTIMP, NULL);
    return;
  }

  if(fake_hostent(name, callback, arg)) return;

  hquery = malloc(sizeof(struct host_query));
  if(!hquery) {
    callback(arg, ARES_ENOMEM, NULL);
    return;
  }
  hquery->channel = channel;
  hquery->name = strdup(name);
  if(!hquery->name) {
    free(hquery);
    callback(arg, ARES_ENOMEM, NULL);
    return;
  }
  hquery->callback = callback;
  hquery->arg = arg;
  hquery->remaining_lookups = channel->lookups;

  /* Start performing lookups according to channel->lookups. */
  next_lookup(hquery);
}

static void next_lookup(struct host_query *hquery) {
  int status;
  const char *p;
  struct hostent *host;

  for (p = hquery->remaining_lookups; *p; p++) {
    switch (*p) {
	  case 'b': //DNS lookup
	  hquery->remaining_lookups = p + 1;
	  ares_search(hquery->channel, hquery->name, C_IN, T_A, host_callback, hquery);
	  return;

	case 'f': //Host file lookup
	  break;
	}
  }
  end_hquery(hquery, ARES_ENOTFOUND, NULL);
}

static void host_callback(void *arg, int status, unsigned char *abuf, int alen)
{
  struct host_query *hquery = (struct host_query *) arg;
  ares_channel channel = hquery->channel;
  struct hostent *host;

  if (status == ARES_SUCCESS)
    {
      status = ares_parse_a_reply(abuf, alen, &host);
      if (host && channel->nsort)
	sort_addresses(host, channel->sortlist, channel->nsort);
      end_hquery(hquery, status, host);
    }
  else if (status == ARES_EDESTRUCTION)
    end_hquery(hquery, status, NULL);
  else
    next_lookup(hquery);
}

static void end_hquery(struct host_query *hquery, int status,
		       struct hostent *host)
{
  hquery->callback(hquery->arg, status, host);
  //if(host) ares_free_hostent(host);
  free(hquery->name);
  free(hquery);
}

static int fake_hostent(const char *name, ares_host_callback callback, void *arg) {
  struct in_addr addr;
  struct hostent hostent;
  const char *p;
  char *aliases[1] = { NULL };
  char *addrs[2];

  for(p = name; *p; p++) {
    if (!isdigit((unsigned char)*p) && *p != '.')
      return 0;
  }

  /* It also only looks like an IP address if it's non-zero-length and
   * doesn't end with a dot.
   */
  if(p == name || *(p - 1) == '.')
    return 0;

  /* It looks like an IP address.  Figure out what IP address it is. */
  addr.s_addr = inet_addr(name);
  if(addr.s_addr == INADDR_NONE) {
    callback(arg, ARES_EBADNAME, NULL);
    return 1;
  }

  /* Duplicate the name, to avoid a constness violation. */
  hostent.h_name = strdup(name);
  if (!hostent.h_name) {
    callback(arg, ARES_ENOMEM, NULL);
    return 1;
  }

  /* Fill in the rest of the host structure and terminate the query. */
  addrs[0] = (char *) &addr;
  addrs[1] = NULL;
  hostent.h_aliases = aliases;
  hostent.h_addrtype = AF_INET;
  hostent.h_length = sizeof(struct in_addr);
  hostent.h_addr_list = addrs;
  callback(arg, ARES_SUCCESS, &hostent);

  free(hostent.h_name);
  return 1;
}

static void sort_addresses(struct hostent *host, struct apattern *sortlist,
			   int nsort)
{
  struct in_addr a1, a2;
  int i1, i2, ind1, ind2;

  /* This is a simple insertion sort, not optimized at all.  i1 walks
   * through the address list, with the loop invariant that everything
   * to the left of i1 is sorted.  In the loop body, the value at i1 is moved
   * back through the list (via i2) until it is in sorted order.
   */
  for (i1 = 0; host->h_addr_list[i1]; i1++)
    {
      memcpy(&a1, host->h_addr_list[i1], sizeof(struct in_addr));
      ind1 = get_address_index(&a1, sortlist, nsort);
      for (i2 = i1 - 1; i2 >= 0; i2--)
	{
	  memcpy(&a2, host->h_addr_list[i2], sizeof(struct in_addr));
	  ind2 = get_address_index(&a2, sortlist, nsort);
	  if (ind2 <= ind1)
	    break;
	  memcpy(host->h_addr_list[i2 + 1], &a2, sizeof(struct in_addr));
	}
      memcpy(host->h_addr_list[i2 + 1], &a1, sizeof(struct in_addr));
    }
}

/* Find the first entry in sortlist which matches addr.  Return nsort
 * if none of them match.
 */
static int get_address_index(struct in_addr *addr, struct apattern *sortlist, int nsort) {
  int i;
  for(i = 0; i < nsort; i++) {
    if ((addr->s_addr & sortlist[i].mask.s_addr) == sortlist[i].addr.s_addr) break;
  }
  return i;
}
