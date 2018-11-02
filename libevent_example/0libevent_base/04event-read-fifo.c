/*
 * This sample code shows how to use Libevent to read from a named pipe.
 * XXX This code could make better use of the Libevent interfaces.
 *
 * XXX This does not work on Windows; ignore everything inside the _WIN32 block.
 *
 * On UNIX, compile with:
 * cc -I/usr/local/include -o event-read-fifo event-read-fifo.c \
 *     -L/usr/local/lib -levent
 */
#include <event2/event-config.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <event2/event.h>

static void fifo_read(evutil_socket_t fd, short event, void *arg) {
  char buf[255];
  int len;
  struct event* ev = arg;
  fprintf(stderr, "fifo_read called with fd: %d, event: %d, arg: %p\n",
        (int)fd, event, arg);
  len = read(fd, buf, sizeof(buf) - 1);

  if (len <= 0) {
    if (len == -1)
    perror("read");
    else if (len == 0)
    fprintf(stderr, "Connection closed\n");
    event_del(ev);
    event_base_loopbreak(event_get_base(ev));
    return;
  }

  buf[len] = '\0';
  fprintf(stdout, "Read: %s\n", buf);
}

/* On Unix, cleanup event.fifo if SIGINT is received. */
static void signal_cb(evutil_socket_t fd, short event, void *arg) {
  struct event_base* base = arg;
  event_base_loopbreak(base);
}

int main()
{
  struct event* evfifo;
  struct event_base* base;
  struct event* signal_int;
  struct stat st;
  const char* fifo = "event.fifo";
  int socket;

  if (lstat(fifo, &st) == 0) {
    if ((st.st_mode & S_IFMT) == S_IFREG) {
      errno = EEXIST;
      perror("lstat");
      exit(1);
    }
  }

  unlink(fifo);
  if (mkfifo(fifo, 0600) == -1) {
    perror("mkfifo");
    exit(1);
  }
  socket = open(fifo, O_RDONLY | O_NONBLOCK, 0);
  if (socket == -1) {
    perror("open");
    exit(1);
  }

  fprintf(stderr, "Write data to %s\n", fifo);

  base = event_base_new();
  signal_int = evsignal_new(base, SIGINT, signal_cb, base);
  event_add(signal_int, NULL); //未决
  evfifo = event_new(base, socket, EV_READ|EV_PERSIST, fifo_read,
        event_self_cbarg());

  event_add(evfifo, NULL); //未决
  event_base_dispatch(base);
  event_base_free(base);
  close(socket);
  unlink(fifo);
  libevent_global_shutdown();
  return (0);
}

