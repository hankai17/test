package ipc

//获取server中的channel 用以初始化ipc_client
import (
    "encoding/json"
)

type Ipc_client struct { //客户端比较简单 没有抽象成接口 直接是一个channel
    conn chan string
}

func New_ipc_client(server *Ipc_server) *Ipc_client {
     c := server.Connect()
    return &Ipc_client{c}
}

func (client *Ipc_client)Call(method, params string)(resp *Response, err error) { //把请求参数,方法包装成json 放入共用channel, 阻塞等待响应

    req := &Request{method, params}
    var b []byte
    b, err = json.Marshal(req)
    if err != nil {
        return
    }

    client.conn <- string(b)
    str := <-client.conn //等待返回值
    var resp1 Response
    err = json.Unmarshal([]byte(str), &resp1)
    resp = &resp1
    return
}

func (client *Ipc_client)Close() {
    client.conn <- "CLOSE"
}
