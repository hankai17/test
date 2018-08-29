package ipc

import (
    "testing"
)

type echo_server struct { //实现server接口
}

func (server *echo_server)Handle(method, params string) *Response {
    return &Response{"OK", "ECHO: " + method + " ~ " + params}
}

func (server *echo_server)Name() string {
    return "EchoServer"
}

func Test_ipc(t *testing.T) {
    server := New_ipc_server(&echo_server{})
    client1 := New_ipc_client(server)
    client2 := New_ipc_client(server) //两个客户端分别获取两个channel

    resp1, _ := client1.Call("foo", "From Client1")
    resp2, _ := client1.Call("foo", "From Client2") //channel发送数据 等待数据

    if resp1.Body != "ECHO: foo ~ From Client1" || resp2.Body != "ECHO: foo ~ From Client2" {
        t.Error("IpcClient.Call failed. resp1:", resp1, "resp2:", resp2)
    }

    client1.Close()
    client2.Close()
}

