#include <sys/types.h>
#include <sys/param.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <arpa/nameser.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include "ares.h"

static int init_by_options(ares_channel channel, struct ares_options *options,
			   int optmask);
static int init_by_resolv_conf(ares_channel channel);
static int init_by_defaults(ares_channel channel);
static int config_domain(ares_channel channel, char *str);
static int config_nameserver(struct server_state **servers, int *nservers, char *str);
static int set_search(ares_channel channel, const char *str);
static int set_options(ares_channel channel, const char *str);
static char *try_config(char *s, const char *opt);
static const char *try_option(const char *p, const char *q, const char *opt);
static int ip_addr(const char *s, int len, struct in_addr *addr);
static void natural_mask(struct apattern *pat);

int ares_init(ares_channel *channelptr) {
  return ares_init_options(channelptr, NULL, 0);
}

int ares_init_options(ares_channel *channelptr, struct ares_options *options, int optmask) {
  ares_channel channel;
  int i, status;
  struct server_state *server;
  struct timeval tv;

  channel = malloc(sizeof(struct ares_channeldata));
  if (!channel) return ARES_ENOMEM;

  //channel->flags = -1;
  channel->flags = 16;
  channel->timeout = -1;
  channel->tries = -1;
  channel->ndots = -1;
  channel->udp_port = -1;
  channel->tcp_port = -1;
  channel->nservers = -1;
  channel->ndomains = -1;
  channel->nsort = -1;
  channel->lookups = NULL;
  channel->queries = NULL;
  channel->domains = NULL;
  channel->sortlist = NULL;

  /* Initialize configuration by each of the four sources, from highest
   * precedence to lowest.
   */
  status = init_by_options(channel, options, optmask);
  if(status == ARES_SUCCESS) status = init_by_resolv_conf(channel);
  if(status == ARES_SUCCESS) status = init_by_defaults(channel);
  if(status != ARES_SUCCESS) {
      /* Something failed; clean up memory we may have allocated. */
      if (channel->nservers != -1) free(channel->servers);
      if (channel->ndomains != -1) {
	    for (i = 0; i < channel->ndomains; i++)
	      free(channel->domains[i]);
	      free(channel->domains);
	  }
      if (channel->nsort != -1) free(channel->sortlist);
      free(channel->lookups);
      free(channel);
      return status;
  }

  if((channel->flags & ARES_FLAG_PRIMARY) && channel->nservers > 1) //Trim to one server if ARES_FLAG_PRIMARY is set
    channel->nservers = 1;

  for(i = 0; i < channel->nservers; i++) {
    server = &channel->servers[i];
    server->udp_socket = -1;
    server->tcp_socket = -1;
    server->tcp_lenbuf_pos = 0;
    server->tcp_buffer = NULL;
    server->qhead = NULL;
    server->qtail = NULL;
  }

  /* Choose a somewhat random query ID.  The main point is to avoid
   * collisions with stale queries.  An attacker trying to spoof a DNS
   * answer also has to guess the query ID, but it's only a 16-bit
   * field, so there's not much to be done about that.
   */
  gettimeofday(&tv, NULL);
  channel->next_id = (unsigned short) (tv.tv_sec ^ tv.tv_usec ^ getpid()) & 0xffff;
  channel->queries = NULL;

  *channelptr = channel;
  return ARES_SUCCESS;
}

static int init_by_options(ares_channel channel, struct ares_options *options, int optmask) {
  int i;

  //Easy stuff
  if ((optmask & ARES_OPT_FLAGS) && channel->flags == -1)
    channel->flags = options->flags;
  if ((optmask & ARES_OPT_TIMEOUT) && channel->timeout == -1)
    channel->timeout = options->timeout;
  if ((optmask & ARES_OPT_TRIES) && channel->tries == -1)
    channel->tries = options->tries;
  if ((optmask & ARES_OPT_NDOTS) && channel->ndots == -1)
    channel->ndots = options->ndots;
  if ((optmask & ARES_OPT_UDP_PORT) && channel->udp_port == -1)
    channel->udp_port = options->udp_port;
  if ((optmask & ARES_OPT_TCP_PORT) && channel->tcp_port == -1)
    channel->tcp_port = options->tcp_port;

  /* Copy the servers, if given. */
  if ((optmask & ARES_OPT_SERVERS) && channel->nservers == -1)
    {
      channel->servers =
	malloc(options->nservers * sizeof(struct server_state));
      if (!channel->servers && options->nservers != 0)
	return ARES_ENOMEM;
      for (i = 0; i < options->nservers; i++)
	channel->servers[i].addr = options->servers[i];
      channel->nservers = options->nservers;
    }

  /* Copy the domains, if given.  Keep channel->ndomains consistent so
   * we can clean up in case of error.
   */
  if ((optmask & ARES_OPT_DOMAINS) && channel->ndomains == -1)
    {
      channel->domains = malloc(options->ndomains * sizeof(char *));
      if (!channel->domains && options->ndomains != 0)
	return ARES_ENOMEM;
      for (i = 0; i < options->ndomains; i++)
	{
	  channel->ndomains = i;
	  channel->domains[i] = strdup(options->domains[i]);
	  if (!channel->domains[i])
	    return ARES_ENOMEM;
	}
      channel->ndomains = options->ndomains;
    }

  /* Set lookups, if given. */
  if ((optmask & ARES_OPT_LOOKUPS) && !channel->lookups)
    {
      channel->lookups = strdup(options->lookups);
      if (!channel->lookups)
	return ARES_ENOMEM;
    }

  return ARES_SUCCESS;
}

static int init_by_resolv_conf(ares_channel channel)
{
  char *line = NULL;
  int status, nservers = 0, nsort = 0;
  struct server_state *servers = NULL;

  {
    char *p;
    FILE *fp;
    int linesize;
   
    fp = fopen(PATH_RESOLV_CONF, "r");
    if(!fp) return (errno == ENOENT) ? ARES_SUCCESS : ARES_EFILE;
    while ((status = ares__read_line(fp, &line, &linesize)) == ARES_SUCCESS)
    {
      if ((p = try_config(line, "domain")) )
        status = config_domain(channel, p);
      else if ((p = try_config(line, "nameserver")) )
        status = config_nameserver(&servers, &nservers, p);
      else
        status = ARES_SUCCESS;
      if (status != ARES_SUCCESS)
        break;
    }
    free(line);
    fclose(fp);
  }

  if(status != ARES_EOF) {
     if(servers != NULL) free(servers);
     return status;
  }

  if(servers) {
     channel->servers = servers;
     channel->nservers = nservers;
  }

  return ARES_SUCCESS;
}

static int init_by_defaults(ares_channel channel)
{
  char hostname[MAXHOSTNAMELEN + 1];

  if(channel->flags == -1) channel->flags = 0;
  if(channel->timeout == -1) channel->timeout = DEFAULT_TIMEOUT;
  if(channel->tries == -1) channel->tries = DEFAULT_TRIES;
  if(channel->ndots == -1) channel->ndots = 1;
  if(channel->udp_port == -1) channel->udp_port = htons(NAMESERVER_PORT);
  if(channel->tcp_port == -1) channel->tcp_port = htons(NAMESERVER_PORT);

  if(channel->nservers == -1) { //If nobody specified servers, try a local named
     channel->servers = malloc(sizeof(struct server_state));
     if(!channel->servers) return ARES_ENOMEM;
     channel->servers[0].addr.s_addr = htonl(INADDR_LOOPBACK);
     channel->nservers = 1;
  }

  if(channel->ndomains == -1) {
      /* Derive a default domain search list from the kernel hostname, or set it to empty if the hostname isn't helpful. */
     if (gethostname(hostname, sizeof(hostname)) == -1 || !strchr(hostname, '.')) {
       channel->domains = malloc(0);
	   channel->ndomains = 0;
	 } else {
	  channel->domains = malloc(sizeof(char *));
	  if(!channel->domains) return ARES_ENOMEM;
	  channel->ndomains = 0;
	  channel->domains[0] = strdup(strchr(hostname, '.') + 1);
	  if(!channel->domains[0]) return ARES_ENOMEM;
	  channel->ndomains = 1;
	}
  }

  if(channel->nsort == -1) {
    channel->sortlist = NULL;
    channel->nsort = 0;
  }

  if(!channel->lookups) {
    channel->lookups = strdup("bf");
    if (!channel->lookups) return ARES_ENOMEM;
  }

  return ARES_SUCCESS;
}

static int config_domain(ares_channel channel, char *str) {
  return set_search(channel, str);
}

static int config_nameserver(struct server_state **servers, int *nservers, char *str) {
  struct in_addr addr;
  struct server_state *newserv;

  addr.s_addr = inet_addr(str); //Add a nameserver entry, if this is a valid address
  if (addr.s_addr == INADDR_NONE)
    return ARES_SUCCESS;
  newserv = realloc(*servers, (*nservers + 1) * sizeof(struct server_state));
  if (!newserv)
    return ARES_ENOMEM;
  newserv[*nservers].addr = addr;
  *servers = newserv;
  (*nservers)++;
  return ARES_SUCCESS;
}

static int set_search(ares_channel channel, const char *str)
{
  int n;
  const char *p, *q;

  /* Count the domains given. */
  n = 0;
  p = str;
  while (*p)
    {
      while (*p && !isspace((unsigned char)*p))
	p++;
      while (isspace((unsigned char)*p))
	p++;
      n++;
    }

  channel->domains = malloc(n * sizeof(char *));
  if (!channel->domains && n)
    return ARES_ENOMEM;

  /* Now copy the domains. */
  n = 0;
  p = str;
  while (*p)
    {
      channel->ndomains = n;
      q = p;
      while (*q && !isspace((unsigned char)*q))
	q++;
      channel->domains[n] = malloc(q - p + 1);
      if (!channel->domains[n])
	return ARES_ENOMEM;
      memcpy(channel->domains[n], p, q - p);
      channel->domains[n][q - p] = 0;
      p = q;
      while (isspace((unsigned char)*p))
	p++;
      n++;
    }
  channel->ndomains = n;

  return ARES_SUCCESS;
}

static int set_options(ares_channel channel, const char *str)
{
  const char *p, *q, *val;

  p = str;
  while (*p)
    {
      q = p;
      while (*q && !isspace((unsigned char)*q))
	q++;
      val = try_option(p, q, "ndots:");
      if (val && channel->ndots == -1)
	channel->ndots = atoi(val);
      val = try_option(p, q, "retrans:");
      if (val && channel->timeout == -1)
	channel->timeout = atoi(val);
      val = try_option(p, q, "retry:");
      if (val && channel->tries == -1)
	channel->tries = atoi(val);
      p = q;
      while (isspace((unsigned char)*p))
	p++;
    }

  return ARES_SUCCESS;
}

static char *try_config(char *s, const char *opt)
{
  size_t len;

  len = strlen(opt);
  if (strncmp(s, opt, len) != 0 || !isspace((unsigned char)s[len]))
    return NULL;
  s += len;
  while (isspace((unsigned char)*s))
    s++;
  return s;
}

static const char *try_option(const char *p, const char *q, const char *opt)
{
  size_t len = strlen(opt);
  return ((size_t)(q - p) > len && !strncmp(p, opt, len)) ? &p[len] : NULL;
}

static int ip_addr(const char *s, int len, struct in_addr *addr)
{
  char ipbuf[16];

  /* Four octets and three periods yields at most 15 characters. */
  if (len > 15)
    return -1;
  memcpy(ipbuf, s, len);
  ipbuf[len] = 0;

  addr->s_addr = inet_addr(ipbuf);
  if (addr->s_addr == INADDR_NONE && strcmp(ipbuf, "255.255.255.255") != 0)
    return -1;
  return 0;
}

static void natural_mask(struct apattern *pat)
{
  struct in_addr addr;

  /* Store a host-byte-order copy of pat in a struct in_addr.  Icky,
   * but portable.
   */
  addr.s_addr = ntohl(pat->addr.s_addr);

  /* This is out of date in the CIDR world, but some people might
   * still rely on it.
   */
  if (IN_CLASSA(addr.s_addr))
    pat->mask.s_addr = htonl(IN_CLASSA_NET);
  else if (IN_CLASSB(addr.s_addr))
    pat->mask.s_addr = htonl(IN_CLASSB_NET);
  else
    pat->mask.s_addr = htonl(IN_CLASSC_NET);
}
