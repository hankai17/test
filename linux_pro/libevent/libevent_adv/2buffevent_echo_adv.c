#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
void  str_toupper(char *str)
{
    int i;
    for (i = 0; i < strlen(str); i ++)
    {  
        str[i] = toupper(str[i]);
    }
}
void write_cb(struct bufferevent *bev, void *ctx)
{
    struct evbuffer *output = bufferevent_get_output(bev);
    if (evbuffer_get_length(output) ==0)  {
        printf("write done!\n");
    }
}
    static void
echo_read_cb(struct bufferevent *bev, void *ctx)
{
    /* This callback is invoked when there is data to read o
       n bev. */
    //从 bufferevent 得到 输入缓冲区的 首地址
    struct evbuffer *input = bufferevent_get_input(bev);
    //从 bufferevent 得到 输出缓冲区的 首地址
    struct evbuffer *output = bufferevent_get_output(bev);
    /* Copy all the data from the input buffer to the output
       buffer. */
    char *buf = NULL;
    int buf_len = 0;
    //写一些自定义的业务
    //将数据 输入缓冲区  取出来，
    buf_len = evbuffer_get_length(input);
    buf = malloc(buf_len);
    //evbuffer_copyout(input, buf, buf_len);
    evbuffer_remove(input, buf, buf_len);
    //小---》大
    str_toupper(buf);
    //写回去  输出缓冲区 , 底层就会自动将数据发送给对端
    evbuffer_add_printf(output, "%s", buf);
    //evbuffer_add_buffer(output, input);
    free(buf);
}
    static void
echo_event_cb(struct bufferevent *bev, short events, void *ctx)
{
    if (events & BEV_EVENT_ERROR)
        perror("Error from bufferevent");
    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        bufferevent_free(bev);
    }
} 
static void
/* -------------------------------------------*/
/**
 * @brief  accept_conn_cb 
 *
 * @param listener  listenner事件
 * @param fd        cfd 已经accept成功的
 * @param address   客户端地址
 * @param socklen   客户端长度
 * @param ctx       形参
 */
/* -------------------------------------------*/
accept_conn_cb(struct evconnlistener *listener,
        evutil_socket_t fd, struct sockaddr *address, int socklen,
        void *ctx)
{
    /* We got a new connection! Set up a bufferevent for it.
     */
    struct event_base *base = evconnlistener_get_base(listener);
    //创建一个bufferevent
    struct bufferevent *bev = bufferevent_socket_new(
            base, fd, BEV_OPT_CLOSE_ON_FREE);
    //给bufferevent设置回调函数
    bufferevent_setcb(bev, echo_read_cb, write_cb, echo_event_cb, NULL);
    //启动bufferevent的 事件监控
    bufferevent_enable(bev, EV_READ|EV_WRITE);
}
    static void
accept_error_cb(struct evconnlistener *listener, void *ctx)
{
    struct event_base *base = evconnlistener_get_base(listener);
    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr, "Got an error %d (%s) on the listener. "
            "Shutting down.\n", err, evutil_socket_error_to_string(err));
    event_base_loopexit(base, NULL);
} 
int main(int argc, char **argv)
{
    struct event_base *base;
    struct evconnlistener *listener;
    struct sockaddr_in sin;
    int port = 9876;
    if (argc > 1) {
        port = atoi(argv[1]);
    } 
    if (port<=0 || port>65535) {
        puts("Invalid port");
        return 1;
    } 
    //创建一个base
    base = event_base_new();
    if (!base) {
        puts("Couldn't open event base");
        return 1;
    } 
    /* Clear the sockaddr before using it, in case there are
       extra
     * platform-specific fields that can mess us up. */
    memset(&sin, 0, sizeof(sin));
    /* This is an INET address */
    sin.sin_family = AF_INET;
    /* Listen on 0.0.0.0 */
    sin.sin_addr.s_addr = htonl(0);
    /* Listen on the given port. */
    sin.sin_port = htons(port);
    //绑定一个listenner事件，同时指定一个linsterner事件的回调函数
    listener = evconnlistener_new_bind(base, accept_conn_cb,
            NULL,
            LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1,
            (struct sockaddr*)&sin, sizeof(sin));
    if (!listener) {
        perror("Couldn't create listener");
        return 1;
    } 
    evconnlistener_set_error_cb(listener, accept_error_cb);
    //进入循环监控
    event_base_dispatch(base);
    return 0;
}
