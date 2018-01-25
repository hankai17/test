package ipc

//server是一个接口 ipc_server是接口的结构体 然后给ipc_server添加connect方法 方法中调用具体的接口
import (
    "encoding/json"
    "fmt"
)

type Request struct {
    Method string `json:"method"`
    Params string `json:"params"`
}

type Response struct {
    Code string `json:"code"`
    Body string `json:"body"`
}

type Server interface { //抽象成接口 
    Name() string
    Handle(method, params string) *Response
}

type Ipc_server struct { //ipc服务器 是服务器的一种服务器都有name handle方法 so是一个接口
    Server
}


func New_ipc_server(server Server) *Ipc_server {
    return &Ipc_server{server}
}

func (server *Ipc_server)Connect() chan string { //connect方法 首先把channel返回给外部，然后获取数据再发出数据
    session := make(chan string, 0)
    go func(c chan string) { //闭包
        for {
            request := <-c
            if request  == "CLOSE" { // 关闭该连接
                break
            }

            var req Request
            err := json.Unmarshal([]byte(request), &req) //把json格式请求 解析成req
            if err != nil {
                fmt.Println("Invalid request format:", request)
            }

            resp := server.Handle(req.Method, req.Params) //处理请求
            b, err := json.Marshal(resp) //封装响应成json
            c <- string(b) // 返回结果
        }
	    fmt.Println("Session closed.")
    }(session) //闭包参数  go关键字与多线程一样 不会阻塞在这
    fmt.Println("A new session has been created successfully.")
    return session
}
