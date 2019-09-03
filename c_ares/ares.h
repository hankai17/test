#ifndef ARES__H
#define ARES__H
#include <stdio.h>
#include <sys/types.h>
#ifdef _AIX
/* HP-UX systems version 9, 10 and 11 lack sys/select.h and so does oldish
   libc5-based Linux systems. Only include it on system that are known to
   require it! */
#include <sys/select.h>
#endif
#include <netinet/in.h>

#define ARES_SUCCESS		0

/* Server error codes (ARES_ENODATA indicates no relevant answer) */
#define ARES_ENODATA		1
#define ARES_EFORMERR		2
#define ARES_ESERVFAIL		3
#define ARES_ENOTFOUND		4
#define ARES_ENOTIMP		5
#define ARES_EREFUSED		6

/* Locally generated error codes */
#define ARES_EBADQUERY		7
#define ARES_EBADNAME		8
#define ARES_EBADFAMILY		9
#define ARES_EBADRESP		10
#define ARES_ECONNREFUSED	11
#define ARES_ETIMEOUT		12
#define ARES_EOF		13
#define ARES_EFILE		14
#define ARES_ENOMEM		15
#define ARES_EDESTRUCTION	16

/* Flag values */
#define ARES_FLAG_USEVC		(1 << 0) //TCP?
#define ARES_FLAG_PRIMARY	(1 << 1) //Only use one dns server
#define ARES_FLAG_IGNTC		(1 << 2)
#define ARES_FLAG_NORECURSE	(1 << 3)
#define ARES_FLAG_STAYOPEN	(1 << 4)
#define ARES_FLAG_NOSEARCH	(1 << 5)
#define ARES_FLAG_NOALIASES	(1 << 6)
#define ARES_FLAG_NOCHECKRESP	(1 << 7)

/* Option mask values */
#define ARES_OPT_FLAGS		(1 << 0)
#define ARES_OPT_TIMEOUT	(1 << 1)
#define ARES_OPT_TRIES		(1 << 2)
#define ARES_OPT_NDOTS		(1 << 3)
#define ARES_OPT_UDP_PORT	(1 << 4)
#define ARES_OPT_TCP_PORT	(1 << 5)
#define ARES_OPT_SERVERS	(1 << 6)
#define ARES_OPT_DOMAINS	(1 << 7)
#define ARES_OPT_LOOKUPS	(1 << 8)

#define DNS__16BIT(p)			(((p)[0] << 8) | (p)[1])
#define DNS__32BIT(p)			(((p)[0] << 24) | ((p)[1] << 16) | \
					 ((p)[2] << 8) | (p)[3])
#define DNS__SET16BIT(p, v)		(((p)[0] = ((v) >> 8) & 0xff), \
					 ((p)[1] = (v) & 0xff))
#define DNS__SET32BIT(p, v)		(((p)[0] = ((v) >> 24) & 0xff), \
					 ((p)[1] = ((v) >> 16) & 0xff), \
					 ((p)[2] = ((v) >> 8) & 0xff), \
					 ((p)[3] = (v) & 0xff))

/* Macros for parsing a DNS header */
#define DNS_HEADER_QID(h)		DNS__16BIT(h)
#define DNS_HEADER_QR(h)		(((h)[2] >> 7) & 0x1)
#define DNS_HEADER_OPCODE(h)		(((h)[2] >> 3) & 0xf)
#define DNS_HEADER_AA(h)		(((h)[2] >> 2) & 0x1)
#define DNS_HEADER_TC(h)		(((h)[2] >> 1) & 0x1)
#define DNS_HEADER_RD(h)		((h)[2] & 0x1)
#define DNS_HEADER_RA(h)		(((h)[3] >> 7) & 0x1)
#define DNS_HEADER_Z(h)			(((h)[3] >> 4) & 0x7)
#define DNS_HEADER_RCODE(h)		((h)[3] & 0xf)
#define DNS_HEADER_QDCOUNT(h)		DNS__16BIT((h) + 4)
#define DNS_HEADER_ANCOUNT(h)		DNS__16BIT((h) + 6)
#define DNS_HEADER_NSCOUNT(h)		DNS__16BIT((h) + 8)
#define DNS_HEADER_ARCOUNT(h)		DNS__16BIT((h) + 10)

/* Macros for constructing a DNS header */
#define DNS_HEADER_SET_QID(h, v)	DNS__SET16BIT(h, v)
#define DNS_HEADER_SET_QR(h, v)		((h)[2] |= (((v) & 0x1) << 7))
#define DNS_HEADER_SET_OPCODE(h, v)	((h)[2] |= (((v) & 0xf) << 3))
#define DNS_HEADER_SET_AA(h, v)		((h)[2] |= (((v) & 0x1) << 2))
#define DNS_HEADER_SET_TC(h, v)		((h)[2] |= (((v) & 0x1) << 1))
#define DNS_HEADER_SET_RD(h, v)		((h)[2] |= (((v) & 0x1)))
#define DNS_HEADER_SET_RA(h, v)		((h)[3] |= (((v) & 0x1) << 7))
#define DNS_HEADER_SET_Z(h, v)		((h)[3] |= (((v) & 0x7) << 4))
#define DNS_HEADER_SET_RCODE(h, v)	((h)[3] |= (((v) & 0xf)))
#define DNS_HEADER_SET_QDCOUNT(h, v)	DNS__SET16BIT((h) + 4, v)
#define DNS_HEADER_SET_ANCOUNT(h, v)	DNS__SET16BIT((h) + 6, v)
#define DNS_HEADER_SET_NSCOUNT(h, v)	DNS__SET16BIT((h) + 8, v)
#define DNS_HEADER_SET_ARCOUNT(h, v)	DNS__SET16BIT((h) + 10, v)

/* Macros for parsing the fixed part of a DNS question */
#define DNS_QUESTION_TYPE(q)		DNS__16BIT(q)
#define DNS_QUESTION_CLASS(q)		DNS__16BIT((q) + 2)

/* Macros for constructing the fixed part of a DNS question */
#define DNS_QUESTION_SET_TYPE(q, v)	DNS__SET16BIT(q, v)
#define DNS_QUESTION_SET_CLASS(q, v)	DNS__SET16BIT((q) + 2, v)

/* Macros for parsing the fixed part of a DNS resource record */
#define DNS_RR_TYPE(r)			DNS__16BIT(r)
#define DNS_RR_CLASS(r)			DNS__16BIT((r) + 2)
#define DNS_RR_TTL(r)			DNS__32BIT((r) + 4)
#define DNS_RR_LEN(r)			DNS__16BIT((r) + 8)

/* Macros for constructing the fixed part of a DNS resource record */
#define DNS_RR_SET_TYPE(r)		DNS__SET16BIT(r, v)
#define DNS_RR_SET_CLASS(r)		DNS__SET16BIT((r) + 2, v)
#define DNS_RR_SET_TTL(r)		DNS__SET32BIT((r) + 4, v)
#define DNS_RR_SET_LEN(r)		DNS__SET16BIT((r) + 8, v)

#define ares_closesocket(x) close(x)

#define	DEFAULT_TIMEOUT		5
#define DEFAULT_TRIES		4
#ifndef INADDR_NONE
#define	INADDR_NONE 0xffffffff
#endif

#define PATH_RESOLV_CONF	"/etc/resolv.conf"
#define PATH_HOSTS		"/etc/hosts"

struct ares_options {
  int flags;
  int timeout;
  int tries;
  int ndots;
  unsigned short udp_port;
  unsigned short tcp_port;
  struct in_addr *servers;
  int nservers;
  char **domains;
  int ndomains;
  char *lookups;
};

struct hostent;
struct timeval;
struct ares_channeldata;
typedef struct ares_channeldata *ares_channel;
typedef void (*ares_callback)(void *arg, int status, unsigned char *abuf, int alen);
typedef void (*ares_host_callback)(void *arg, int status, struct hostent *hostent);

struct send_request {
  const unsigned char *data; //Remaining data to send
  size_t len;
  struct send_request *next; //request queue
};

struct server_state {
  struct in_addr addr;
  int udp_socket;
  int tcp_socket;

  /* Mini-buffer for reading the length word */
  unsigned char tcp_lenbuf[2];
  int tcp_lenbuf_pos;
  int tcp_length;

  /* Buffer for reading actual TCP data */
  unsigned char *tcp_buffer;
  int tcp_buffer_pos;

  /* TCP output queue */
  struct send_request *qhead;
  struct send_request *qtail;
};

struct query {
  unsigned short qid;
  time_t timeout;

  unsigned char *tcpbuf; //Query buf with length at beginning, for TCP transmission
  int tcplen;

  const unsigned char *qbuf; //Arguments passed to ares_send() (qbuf points into tcpbuf)
  int qlen;
  ares_callback callback;
  void *arg;

  int try; //Query status
  int server;
  int *skip_server;
  int using_tcp;
  int error_status;

  struct query *next; //query chain
};

/* An IP address pattern; matches an IP address X if X & mask == addr */
struct apattern {
  struct in_addr addr;
  struct in_addr mask;
};

struct ares_channeldata {
  int flags;
  int timeout;
  int tries;
  int ndots;
  int udp_port;
  int tcp_port;
  char **domains;
  int ndomains;
  struct apattern *sortlist;
  int nsort;
  char *lookups;

  /* Server addresses and communications state */
  struct server_state *servers;
  int nservers;

  unsigned short next_id;

  struct query *queries; //Active queries
};

int ares_init(ares_channel *channelptr);
int ares_init_options(ares_channel *channelptr, struct ares_options *options, int optmask);
void ares_destroy(ares_channel channel);

void ares_send(ares_channel channel, const unsigned char *qbuf, int qlen, ares_callback callback, void *arg);
void ares_query(ares_channel channel, const char *name, int dnsclass, int type, ares_callback callback, void *arg);
void ares_search(ares_channel channel, const char *name, int dnsclass, int type, ares_callback callback, void *arg);
void ares_gethostbyname(ares_channel channel, const char *name, int family, ares_host_callback callback, void *arg);
void ares_gethostbyaddr(ares_channel channel, const void *addr, int addrlen, int family, ares_host_callback callback, void *arg);
int ares_fds(ares_channel channel, fd_set *read_fds, fd_set *write_fds);
struct timeval *ares_timeout(ares_channel channel, struct timeval *maxtv, struct timeval *tvbuf);
void ares_process(ares_channel channel, fd_set *read_fds, fd_set *write_fds);
int ares_mkquery(const char *name, int dnsclass, int type, unsigned short id, int rd, unsigned char **buf, int *buflen);
int ares_expand_name(const unsigned char *encoded, const unsigned char *abuf, int alen, char **s, long *enclen);
int ares_parse_a_reply(const unsigned char *abuf, int alen, struct hostent **host);
int ares_parse_ptr_reply(const unsigned char *abuf, int alen, const void *addr, int addrlen, int family, struct hostent **host);
void ares_free_string(void *str);
const char *ares_strerror(int code);
void ares_free_errmem(char *mem);

void ares__send_query(ares_channel channel, struct query *query, time_t now);
void ares__close_sockets(struct server_state *server);
int ares__read_line(FILE *fp, char **buf, int *bufsize);

#endif /* ARES__H */
