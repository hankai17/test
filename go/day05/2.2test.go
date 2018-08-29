package main

import (
  "net"
  "fmt"
)

var (
	maxRead = 1100
	msgStop   = []byte("cmdStop")
	msgStart  = []byte("cmdContinue")
)

func initServer(hostAndPort string) *net.TCPListener {
  serverAddr, err := net.ResolveTCPAddr("tcp", hostAndPort)
  checkError(err, "Resolving address:port failed: '" + hostAndPort + "'")

  listener, err := net.ListenTCP("tcp", serverAddr)
  checkError(err, "ListenTCP: ")

  println("Listening to: ", listener.Addr().String())
  return listener //怎样看listener的类型？//https://studygolang.com/articles/581
}

func talktoclients(to net.Conn) {
  wrote, err := to.Write(msgStart)
  checkError(err, "Write: wrote " + string(wrote) + " bytes.")
}

func handleMsg(length int, err error, msg []byte) {
  if length > 0 {
	for i := 0; ; i++ {
	  if msg[i] == 0 {
		break
	  }
	}
	fmt.Printf("Received data: %v", string(msg[0:length]))
	fmt.Println("   length:",length)
  }
}

func connectionHandler(conn net.Conn) {
  connFrom := conn.RemoteAddr().String()
  println("Connection from: ", connFrom)

  talktoclients(conn) //server端写入数据

  for {
	var ibuf []byte = make([]byte, maxRead + 1)
	length, err := conn.Read(ibuf[0:maxRead])
	ibuf[maxRead] = 0 // to prevent overflow

	switch err {
	case nil:
	  handleMsg(length, err, ibuf) //处理客户端数据
	default:
	  goto DISCONNECT
	}
  }

  DISCONNECT:
  err := conn.Close()
  println("Closed connection:" , connFrom)
  checkError(err, "Close:" )
}


func checkError(error error, info string) {
  if error != nil {
	panic("ERROR: " + info + " " + error.Error()) // terminate program
  }
}

func main() {
  hostAndPort := "localhost:54321"
  listener := initServer(hostAndPort)

  for {
	conn, err := listener.Accept()
	checkError(err, "Accept: ")

	go connectionHandler(conn)
  }
}
