#include <sys/types.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "ares.h"

struct qquery {
  ares_callback callback;
  void *arg;
};

struct search_query {
  /* Arguments passed to ares_search */
  ares_channel channel;
  char *name;			/* copied into an allocated buffer */
  int dnsclass;
  int type;
  ares_callback callback;
  void *arg;

  int status_as_is;		/* error status from trying as-is */
  int next_domain;		/* next search domain to try */
  int trying_as_is;		/* current query is for name as-is */
};



static int name_length(const unsigned char *encoded, const unsigned char *abuf, int alen);
static void qcallback(void *arg, int status, unsigned char *abuf, int alen);
static void search_callback(void *arg, int status, unsigned char *abuf, int alen);
static void end_squery(struct search_query *squery, int status, unsigned char *abuf, int alen);

int ares_fds(ares_channel channel, fd_set *read_fds, fd_set *write_fds) {
  struct server_state *server;
  int i, nfds;

  if(!channel->queries) return 0;

  nfds = 0;
  for(i = 0; i < channel->nservers; i++) {
    server = &channel->servers[i];
    if(server->udp_socket != -1) { //TODO
      FD_SET(server->udp_socket, read_fds);
      if (server->udp_socket >= nfds)
        nfds = server->udp_socket + 1;
    }
    if(server->tcp_socket != -1) {
      FD_SET(server->tcp_socket, read_fds);
      if (server->qhead)
        FD_SET(server->tcp_socket, write_fds);
      if (server->tcp_socket >= nfds)
        nfds = server->tcp_socket + 1;
    }
  }
  return nfds;
}

void ares_free_string(void *str) {
  free(str);
}

void ares_destroy(ares_channel channel) {
  int i;
  struct query *query;

  for (i = 0; i < channel->nservers; i++)
    ares__close_sockets(&channel->servers[i]);
  free(channel->servers);
  for (i = 0; i < channel->ndomains; i++)
    free(channel->domains[i]);
  free(channel->domains);
  if(channel->sortlist)
    free(channel->sortlist);
  free(channel->lookups);
  while (channel->queries) {
    query = channel->queries;
    channel->queries = query->next;
    query->callback(query->arg, ARES_EDESTRUCTION, NULL, 0);
    free(query->tcpbuf);
    free(query->skip_server);
    free(query);
  }
  free(channel);
}

/* Return the length of the expansion of an encoded domain name, or
 * -1 if the encoding is invalid.
 */
static int name_length(const unsigned char *encoded, const unsigned char *abuf, int alen) {
  int n = 0, offset, indir = 0;

  /* Allow the caller to pass us abuf + alen and have us check for it. */
  if(encoded == abuf + alen) return -1;

  while(*encoded) {
    if((*encoded & INDIR_MASK) == INDIR_MASK) {
      /* Check the offset and go there. */
      if(encoded + 1 >= abuf + alen) return -1;
      offset = (*encoded & ~INDIR_MASK) << 8 | *(encoded + 1);
      if(offset >= alen) return -1;
      encoded = abuf + offset;

      /* If we've seen more indirects than the message length,
       * then there's a loop.
       */
      if(++indir > alen) return -1;
    } else {
      offset = *encoded;
      if(encoded + offset + 1 >= abuf + alen) return -1;
      encoded++;
      while (offset--) {
        n += (*encoded == '.' || *encoded == '\\') ? 2 : 1;
        encoded++;
      }
      n++;
    }
  }

  /* If there were any labels at all, then the number of dots is one
   * less than the number of labels, so subtract one.
   */
  return (n) ? n - 1 : n;
}

/* Expand an RFC1035-encoded domain name given by encoded.  The
 * containing message is given by abuf and alen.  The result given by
 * *s, which is set to a NUL-terminated allocated buffer.  *enclen is
 * set to the length of the encoded name (not the length of the
 * expanded name; the goal is to tell the caller how many bytes to
 * move forward to get past the encoded name).
 *
 * In the simple case, an encoded name is a series of labels, each
 * composed of a one-byte length (limited to values between 0 and 63
 * inclusive) followed by the label contents.  The name is terminated
 * by a zero-length label.
 *
 * In the more complicated case, a label may be terminated by an
 * indirection pointer, specified by two bytes with the high bits of
 * the first byte (corresponding to INDIR_MASK) set to 11.  With the
 * two high bits of the first byte stripped off, the indirection
 * pointer gives an offset from the beginning of the containing
 * message with more labels to decode.  Indirection can happen an
 * arbitrary number of times, so we have to detect loops.
 *
 * Since the expanded name uses '.' as a label separator, we use
 * backslashes to escape periods or backslashes in the expanded name.
 */

int ares_expand_name(const unsigned char *encoded, const unsigned char *abuf, int alen, char **s, long *enclen) {
  int len, indir = 0;
  char *q;
  const unsigned char *p;

  len = name_length(encoded, abuf, alen);
  if(len == -1) return ARES_EBADNAME;

  *s = malloc(len + 1);
  if(!*s) return ARES_ENOMEM;
  q = *s;

  /* No error-checking necessary; it was all done by name_length(). */
  p = encoded;
  while(*p) {
    if((*p & INDIR_MASK) == INDIR_MASK) {
      if(!indir) {
        *enclen = p + 2 - encoded;
        indir = 1;
      }
      p = abuf + ((*p & ~INDIR_MASK) << 8 | *(p + 1));
    } else {
      len = *p;
      p++;
      while(len--) {
        if(*p == '.' || *p == '\\')
      *q++ = '\\';
        *q++ = *p;
        p++;
      }
      *q++ = '.';
    }
  }
  if(!indir)
    *enclen = p + 1 - encoded;

  /* Nuke the trailing period if we wrote one. */
  if (q > *s)
    *(q - 1) = 0;

  return ARES_SUCCESS;
}

/* Header format, from RFC 1035:
 *                                  1  1  1  1  1  1
 *    0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                      ID                       |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                    QDCOUNT                    |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                    ANCOUNT                    |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                    NSCOUNT                    |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                    ARCOUNT                    |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *
 * AA, TC, RA, and RCODE are only set in responses.  Brief description
 * of the remaining fields:
 *	ID	Identifier to match responses with queries
 *	QR	Query (0) or response (1)
 *	Opcode	For our purposes, always QUERY
 * 	RD	Recursion desired
 *	Z	Reserved (zero)
 *	QDCOUNT	Number of queries
 *	ANCOUNT	Number of answers
 *	NSCOUNT	Number of name server records
 *	ARCOUNT	Number of additional records
 *
 * Question format, from RFC 1035:
 *                                  1  1  1  1  1  1
 *    0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                                               |
 *  /                     QNAME                     /
 *  /                                               /
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                     QTYPE                     |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                     QCLASS                    |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *
 * The query name is encoded as a series of labels, each represented
 * as a one-byte length (maximum 63) followed by the text of the
 * label.  The list is terminated by a label of length zero (which can
 * be thought of as the root domain).
 */

int ares_mkquery(const char *name, int dnsclass, int type, unsigned short id, int rd, unsigned char **buf, int *buflen) {
  int len;
  unsigned char *q;
  const char *p;

  /* Compute the length of the encoded name so we can check buflen.
   * Start counting at 1 for the zero-length label at the end. */
  len = 1;
  for(p = name; *p; p++) {
    if (*p == '\\' && *(p + 1) != 0) p++;
    len++;
  }
  /* If there are n periods in the name, there are n + 1 labels, and
   * thus n + 1 length fields, unless the name is empty or ends with a
   * period.  So add 1 unless name is empty or ends with a period.
   */
  if(*name && *(p - 1) != '.')
    len++;

  *buflen = len + HFIXEDSZ + QFIXEDSZ;
  *buf = malloc(*buflen);
  if(!*buf) return ARES_ENOMEM;

  /* Set up the header. */
  q = *buf;
  memset(q, 0, HFIXEDSZ);
  DNS_HEADER_SET_QID(q, id);
  DNS_HEADER_SET_OPCODE(q, QUERY);
  DNS_HEADER_SET_RD(q, (rd) ? 1 : 0);
  DNS_HEADER_SET_QDCOUNT(q, 1);

  /* A name of "." is a screw case for the loop below, so adjust it. */
  if(strcmp(name, ".") == 0)
    name++;

  /* Start writing out the name after the header. */
  q += HFIXEDSZ;
  while(*name) {
      if(*name == '.') return ARES_EBADNAME;

      /* Count the number of bytes in this label. */
      len = 0;
      for(p = name; *p && *p != '.'; p++) {
	    if(*p == '\\' && *(p + 1) != 0)
	    p++;
	    len++;
	  }
      if(len > MAXLABEL) return ARES_EBADNAME;

      /* Encode the length and copy the data. */
      *q++ = len;
      for(p = name; *p && *p != '.'; p++) {
	    if (*p == '\\' && *(p + 1) != 0)
	    p++;
	    *q++ = *p;
      }

      /* Go to the next label and repeat, unless we hit the end. */
      if (!*p) break;
      name = p + 1;
  }

  /* Add the zero-length label at the end. */
  *q++ = 0;

  /* Finish off the question with the type and class. */
  DNS_QUESTION_SET_TYPE(q, type);
  DNS_QUESTION_SET_CLASS(q, dnsclass);

  return ARES_SUCCESS;
}
int ares_parse_a_reply(const unsigned char *abuf, int alen, struct hostent **host) {
  unsigned int qdcount, ancount;
  int status, i, rr_type, rr_class, rr_len, naddrs;
  int naliases;
  long len;
  const unsigned char *aptr;
  char *hostname, *rr_name, *rr_data, **aliases;
  struct in_addr *addrs;
  struct hostent *hostent;

  /* Set *host to NULL for all failure cases. */
  *host = NULL;

  /* Give up if abuf doesn't have room for a header. */
  if(alen < HFIXEDSZ) return ARES_EBADRESP;

  /* Fetch the question and answer count from the header. */
  qdcount = DNS_HEADER_QDCOUNT(abuf);
  ancount = DNS_HEADER_ANCOUNT(abuf);
  if(qdcount != 1) return ARES_EBADRESP;

  /* Expand the name from the question, and skip past the question. */
  aptr = abuf + HFIXEDSZ; //In query
  status = ares_expand_name(aptr, abuf, alen, &hostname, &len);
  if(status != ARES_SUCCESS) return status;
  if(aptr + len + QFIXEDSZ > abuf + alen) {
    free(hostname);
    return ARES_EBADRESP;
  }
  aptr += len + QFIXEDSZ; //In ans

  /* Allocate addresses and aliases; ancount gives an upper bound for both. */
  addrs = malloc(ancount * sizeof(struct in_addr));
  if (!addrs) {
    free(hostname);
    return ARES_ENOMEM;
  }
  aliases = malloc((ancount + 1) * sizeof(char *));
  if(!aliases) {
    free(hostname);
    free(addrs);
    return ARES_ENOMEM;
  }
  naddrs = 0;
  naliases = 0;

  /* Examine each answer resource record (RR) in turn. */
  for(i = 0; i < (int)ancount; i++) {
    /* Decode the RR up to the data field. */
    status = ares_expand_name(aptr, abuf, alen, &rr_name, &len);
    if(status != ARES_SUCCESS) break;
    aptr += len;
    if(aptr + RRFIXEDSZ > abuf + alen) {
      status = ARES_EBADRESP;
      break;
    }
    rr_type = DNS_RR_TYPE(aptr);
    rr_class = DNS_RR_CLASS(aptr);
    rr_len = DNS_RR_LEN(aptr);
    aptr += RRFIXEDSZ;

    if(rr_class == C_IN && rr_type == T_A && rr_len == sizeof(struct in_addr) && strcasecmp(rr_name, hostname) == 0) {
      memcpy(&addrs[naddrs], aptr, sizeof(struct in_addr));
      naddrs++;
      status = ARES_SUCCESS;
    } //正是查询域名的a记录 则拷到a记录数组中

    if(rr_class == C_IN && rr_type == T_CNAME) { //cname 则把具体的域名 放到alias数组中
      /* Record the RR name as an alias. */
      aliases[naliases] = rr_name;
      naliases++
      /* Decode the RR data and replace the hostname with it. */
      status = ares_expand_name(aptr, abuf, alen, &rr_data, &len);
      if(status != ARES_SUCCESS) break;
      free(hostname);
      hostname = rr_data;
    } else
      free(rr_name);

    aptr += rr_len;
    if(aptr > abuf + alen) {
      status = ARES_EBADRESP;
      break;
    }
  }

  if(status == ARES_SUCCESS && naddrs == 0) status = ARES_ENODATA;
  if(status == ARES_SUCCESS) { //只要有a记录 就当做是成功
    /* We got our answer.  Allocate memory to build the host entry. */
    aliases[naliases] = NULL;
    hostent = malloc(sizeof(struct hostent));
    if(hostent) {
      hostent->h_addr_list = malloc((naddrs + 1) * sizeof(char *));
      if(hostent->h_addr_list) {
        /* Fill in the hostent and return successfully. */
        hostent->h_name = hostname;
        hostent->h_aliases = aliases;
        hostent->h_addrtype = AF_INET;
        hostent->h_length = sizeof(struct in_addr);
        for (i = 0; i < naddrs; i++)
      	  hostent->h_addr_list[i] = (char *) &addrs[i];
        hostent->h_addr_list[naddrs] = NULL;
        *host = hostent;
        return ARES_SUCCESS;
      }
      free(hostent);
    }
    status = ARES_ENOMEM;
  }
  for(i = 0; i < naliases; i++)
    free(aliases[i]);
  free(aliases);
  free(addrs);
  free(hostname);
  return status;
}

static void qcallback(void *arg, int status, unsigned char *abuf, int alen) {
  struct qquery *qquery = (struct qquery *) arg;
  unsigned int ancount;
  int rcode;

  if (status != ARES_SUCCESS)
    qquery->callback(qquery->arg, status, abuf, alen);
  else
    {
      /* Pull the response code and answer count from the packet. */
      rcode = DNS_HEADER_RCODE(abuf);
      ancount = DNS_HEADER_ANCOUNT(abuf);

      /* Convert errors. */
      switch (rcode)
	{
	case NOERROR:
	  status = (ancount > 0) ? ARES_SUCCESS : ARES_ENODATA;
	  break;
	case FORMERR:
	  status = ARES_EFORMERR;
	  break;
	case SERVFAIL:
	  status = ARES_ESERVFAIL;
	  break;
	case NXDOMAIN:
	  status = ARES_ENOTFOUND;
	  break;
	case NOTIMP:
	  status = ARES_ENOTIMP;
	  break;
	case REFUSED:
	  status = ARES_EREFUSED;
	  break;
	}
      qquery->callback(qquery->arg, status, abuf, alen);
    }
  free(qquery);
}

void ares_query(ares_channel channel, const char *name, int dnsclass, int type, ares_callback callback, void *arg) {
  struct qquery *qquery;
  unsigned char *qbuf;
  int qlen, rd, status;

  /* Compose the query. */
  rd = !(channel->flags & ARES_FLAG_NORECURSE);
  status = ares_mkquery(name, dnsclass, type, channel->next_id, rd, &qbuf, &qlen);
  channel->next_id++;
  if(status != ARES_SUCCESS) {
    callback(arg, status, NULL, 0);
    return;
  }

  /* Allocate and fill in the query structure. */
  qquery = malloc(sizeof(struct qquery));
  if(!qquery) {
    ares_free_string(qbuf);
    callback(arg, ARES_ENOMEM, NULL, 0);
    return;
  }
  qquery->callback = callback;
  qquery->arg = arg;

  /* Send it off.  qcallback will be called when we get an answer. */
  ares_send(channel, qbuf, qlen, qcallback, qquery);
  ares_free_string(qbuf);
}

static void end_squery(struct search_query *squery, int status, unsigned char *abuf, int alen) {
  squery->callback(squery->arg, status, abuf, alen);
  free(squery->name);
  free(squery);
}

static void search_callback(void *arg, int status, unsigned char *abuf, int alen) {
  struct search_query *squery = (struct search_query *) arg;
  ares_channel channel = squery->channel;
  char *s;

  /* Stop searching unless we got a non-fatal error. */
  if (status != ARES_ENODATA && status != ARES_ESERVFAIL
      && status != ARES_ENOTFOUND)
    end_squery(squery, status, abuf, alen);
  else
    {
      /* Save the status if we were trying as-is. */
      if (squery->trying_as_is)
	squery->status_as_is = status;
      if (squery->next_domain < channel->ndomains)
	{
	  /* Try the next domain. */
	    end_squery(squery, ARES_ENODATA, NULL, 0);
	}
      else if (squery->status_as_is == -1)
	{
	  /* Try the name as-is at the end. */
	  squery->trying_as_is = 1;
	  ares_query(channel, squery->name, squery->dnsclass, squery->type,
		     search_callback, squery);
	}
      else
	end_squery(squery, squery->status_as_is, NULL, 0);
    }
}

void ares_search(ares_channel channel, const char *name, int dnsclass, int type, ares_callback callback, void *arg) {
  struct search_query *squery;
  char *s;
  const char *p;
  int status, ndots;

  /* Allocate a search_query structure to hold the state necessary for
   * doing multiple lookups.
   */
  squery = malloc(sizeof(struct search_query));
  if(!squery) {
    callback(arg, ARES_ENOMEM, NULL, 0);
    return;
  }
  squery->channel = channel;
  squery->name = strdup(name);
  if(!squery->name) {
    free(squery);
    callback(arg, ARES_ENOMEM, NULL, 0);
    return;
  }
  squery->dnsclass = dnsclass;
  squery->type = type;
  squery->status_as_is = -1;
  squery->callback = callback;
  squery->arg = arg;

  ndots = 0;
  for(p = name; *p; p++) {
    if(*p == '.') ndots++;
  }

  if (ndots >= channel->ndots) {
    /* Try the name as-is first. */
    squery->next_domain = 0;
    squery->trying_as_is = 1;
    ares_query(channel, name, dnsclass, type, search_callback, squery);
  } else {
    free(squery->name);
    free(squery);
	callback(arg, ARES_ENODATA, NULL, 0);
  }
}

void ares_send(ares_channel channel, const unsigned char *qbuf, int qlen, ares_callback callback, void *arg) {
  struct query *query;
  int i;
  time_t now;

  /* Verify that the query is at least long enough to hold the header. */
  if(qlen < HFIXEDSZ || qlen >= (1 << 16)) {
    callback(arg, ARES_EBADQUERY, NULL, 0);
    return;
  }

  /* Allocate space for query and allocated fields. */
  query = malloc(sizeof(struct query));
  if(!query) {
    callback(arg, ARES_ENOMEM, NULL, 0);
    return;
  }
  query->tcpbuf = malloc(qlen + 2);
  if(!query->tcpbuf) {
    free(query);
    callback(arg, ARES_ENOMEM, NULL, 0);
    return;
  }
  query->skip_server = malloc(channel->nservers * sizeof(int));
  if (!query->skip_server) {
    free(query->tcpbuf);
    free(query);
    callback(arg, ARES_ENOMEM, NULL, 0);
    return;
  }

  /* Compute the query ID.  Start with no timeout. */
  query->qid = DNS_HEADER_QID(qbuf);
  query->timeout = 0;

  /* Form the TCP query buffer by prepending qlen (as two
   * network-order bytes) to qbuf.
   */
  query->tcpbuf[0] = (qlen >> 8) & 0xff;
  query->tcpbuf[1] = qlen & 0xff;
  memcpy(query->tcpbuf + 2, qbuf, qlen);
  query->tcplen = qlen + 2;

  /* Fill in query arguments. */
  query->qbuf = query->tcpbuf + 2;
  query->qlen = qlen;
  query->callback = callback;
  query->arg = arg;

  /* Initialize query status. */
  query->try = 0;
  query->server = 0;
  for (i = 0; i < channel->nservers; i++)
    query->skip_server[i] = 0;
  query->using_tcp = (channel->flags & ARES_FLAG_USEVC) || qlen > PACKETSZ;
  query->error_status = ARES_ECONNREFUSED;

  /* Chain the query into this channel's query list. */
  query->next = channel->queries;
  channel->queries = query;

  /* Perform the first query action. */
  time(&now);
  ares__send_query(channel, query, now);
}

const char *ares_strerror(int code) {
  /* Return a string literal from a table. */
  const char *errtext[] = {
    "Successful completion",
    "DNS server returned answer with no data",
    "DNS server claims query was misformatted",
    "DNS server returned general failure",
    "Domain name not found",
    "DNS server does not implement requested operation",
    "DNS server refused query",
    "Misformatted DNS query",
    "Misformatted domain name",
    "Unsupported address family",
    "Misformatted DNS reply",
    "Could not contact DNS servers",
    "Timeout while contacting DNS servers",
    "End of file",
    "Error reading file",
    "Out of memory"
  };

  assert(code >= 0 && code < (int)(sizeof(errtext) / sizeof(*errtext)));
  return errtext[code];
}

struct timeval *ares_timeout(ares_channel channel, struct timeval *maxtv,
			     struct timeval *tvbuf)
{
  struct query *query;
  time_t now;
  int offset, min_offset;

  /* No queries, no timeout (and no fetch of the current time). */
  if (!channel->queries)
    return maxtv;

  /* Find the minimum timeout for the current set of queries. */
  time(&now);
  min_offset = -1;
  for (query = channel->queries; query; query = query->next)
    {
      if (query->timeout == 0)
	continue;
      offset = query->timeout - now;
      if (offset < 0)
	offset = 0;
      if (min_offset == -1 || offset < min_offset)
	min_offset = offset;
    }

  /* If we found a minimum timeout and it's sooner than the one
   * specified in maxtv (if any), return it.  Otherwise go with
   * maxtv.
   */
  if (min_offset != -1 && (!maxtv || min_offset <= maxtv->tv_sec))
    {
      tvbuf->tv_sec = min_offset;
      tvbuf->tv_usec = 0;
      return tvbuf;
    }
  else
    return maxtv;
}

void ares__close_sockets(struct server_state *server)
{
  struct send_request *sendreq;

  /* Free all pending output buffers. */
  while (server->qhead)
    {
      /* Advance server->qhead; pull out query as we go. */
      sendreq = server->qhead;
      server->qhead = sendreq->next;
      free(sendreq);
    }
  server->qtail = NULL;

  /* Reset any existing input buffer. */
  if (server->tcp_buffer)
    free(server->tcp_buffer);
  server->tcp_buffer = NULL;
  server->tcp_lenbuf_pos = 0;

  /* Close the TCP and UDP sockets. */
  if (server->tcp_socket != -1)
    {
      ares_closesocket(server->tcp_socket);
      server->tcp_socket = -1;
    }
  if (server->udp_socket != -1)
    {
      ares_closesocket(server->udp_socket);
      server->udp_socket = -1;
    }
}

/* This is an internal function.  Its contract is to read a line from
 * a file into a dynamically allocated buffer, zeroing the trailing
 * newline if there is one.  The calling routine may call
 * ares__read_line multiple times with the same buf and bufsize
 * pointers; *buf will be reallocated and *bufsize adjusted as
 * appropriate.  The initial value of *buf should be NULL.  After the
 * calling routine is done reading lines, it should free *buf.
 */
int ares__read_line(FILE *fp, char **buf, int *bufsize)
{
  char *newbuf;
  size_t offset = 0;
  size_t len;

  if (*buf == NULL)
    {
      *buf = malloc(128);
      if (!*buf)
	return ARES_ENOMEM;
      *bufsize = 128;
    }

  while (1)
    {
      if (!fgets(*buf + offset, *bufsize - (int)offset, fp))
	return (offset != 0) ? 0 : (ferror(fp)) ? ARES_EFILE : ARES_EOF;
      len = offset + strlen(*buf + offset);
      if ((*buf)[len - 1] == '\n')
	{
	  (*buf)[len - 1] = 0;
	  return ARES_SUCCESS;
	}
      offset = len;

      /* Allocate more space. */
      newbuf = realloc(*buf, *bufsize * 2);
      if (!newbuf)
	return ARES_ENOMEM;
      *buf = newbuf;
      *bufsize *= 2;
    }
}

int ares_parse_ptr_reply(const unsigned char *abuf, int alen, const void *addr, int addrlen, int family, struct hostent **host) {
  unsigned int qdcount, ancount;
  int status, i, rr_type, rr_class, rr_len;
  long len;
  const unsigned char *aptr;
  char *ptrname, *hostname, *rr_name, *rr_data;
  struct hostent *hostent;

  /* Set *host to NULL for all failure cases. */
  *host = NULL;

  /* Give up if abuf doesn't have room for a header. */
  if (alen < HFIXEDSZ)
    return ARES_EBADRESP;

  /* Fetch the question and answer count from the header. */
  qdcount = DNS_HEADER_QDCOUNT(abuf);
  ancount = DNS_HEADER_ANCOUNT(abuf);
  if (qdcount != 1)
    return ARES_EBADRESP;

  /* Expand the name from the question, and skip past the question. */
  aptr = abuf + HFIXEDSZ;
  status = ares_expand_name(aptr, abuf, alen, &ptrname, &len);
  if (status != ARES_SUCCESS)
    return status;
  if (aptr + len + QFIXEDSZ > abuf + alen)
    {
      free(ptrname);
      return ARES_EBADRESP;
    }
  aptr += len + QFIXEDSZ;

  /* Examine each answer resource record (RR) in turn. */
  hostname = NULL;
  for (i = 0; i < (int)ancount; i++)
    {
      /* Decode the RR up to the data field. */
      status = ares_expand_name(aptr, abuf, alen, &rr_name, &len);
      if (status != ARES_SUCCESS)
	break;
      aptr += len;
      if (aptr + RRFIXEDSZ > abuf + alen)
	{
	  status = ARES_EBADRESP;
	  break;
	}
      rr_type = DNS_RR_TYPE(aptr);
      rr_class = DNS_RR_CLASS(aptr);
      rr_len = DNS_RR_LEN(aptr);
      aptr += RRFIXEDSZ;

      if (rr_class == C_IN && rr_type == T_PTR
	  && strcasecmp(rr_name, ptrname) == 0)
	{
	  /* Decode the RR data and set hostname to it. */
	  status = ares_expand_name(aptr, abuf, alen, &rr_data, &len);
	  if (status != ARES_SUCCESS)
	    break;
	  if (hostname)
	    free(hostname);
	  hostname = rr_data;
	}

      if (rr_class == C_IN && rr_type == T_CNAME)
	{
	  /* Decode the RR data and replace ptrname with it. */
	  status = ares_expand_name(aptr, abuf, alen, &rr_data, &len);
	  if (status != ARES_SUCCESS)
	    break;
	  free(ptrname);
	  ptrname = rr_data;
	}

      free(rr_name);
      aptr += rr_len;
      if (aptr > abuf + alen)
	{
	  status = ARES_EBADRESP;
	  break;
	}
    }

  if (status == ARES_SUCCESS && !hostname)
    status = ARES_ENODATA;
  if (status == ARES_SUCCESS)
    {
      /* We got our answer.  Allocate memory to build the host entry. */
      hostent = malloc(sizeof(struct hostent));
      if (hostent)
	{
	  hostent->h_addr_list = malloc(2 * sizeof(char *));
	  if (hostent->h_addr_list)
	    {
	      hostent->h_addr_list[0] = malloc(addrlen);
	      if (hostent->h_addr_list[0])
		{
		  hostent->h_aliases = malloc(sizeof (char *));
		  if (hostent->h_aliases)
		    {
		      /* Fill in the hostent and return successfully. */
		      hostent->h_name = hostname;
		      hostent->h_aliases[0] = NULL;
		      hostent->h_addrtype = family;
		      hostent->h_length = addrlen;
		      memcpy(hostent->h_addr_list[0], addr, addrlen);
		      hostent->h_addr_list[1] = NULL;
		      *host = hostent;
		      free(ptrname);
		      return ARES_SUCCESS;
		    }
		  free(hostent->h_addr_list[0]);
		}
	      free(hostent->h_addr_list);
	    }
	  free(hostent);
	}
      status = ARES_ENOMEM;
    }
  if (hostname)
    free(hostname);
  free(ptrname);
  return status;
}
