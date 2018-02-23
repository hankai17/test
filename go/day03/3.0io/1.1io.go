package main

import (
    "fmt"
    "io"
    "strings"
    "bytes"
    "bufio"
    "reflect"
)

func main() {
  // from string.
  var r io.Reader = strings.NewReader(string("hello, world"))
  fmt.Println("type:", reflect.TypeOf(r)) //*strings.Reader
  fmt.Println(r) //&{hello, world 0 -1}

  // from bytes.
  var p io.Reader = bytes.NewReader([]byte("hello, world!")) //不带缓冲区
  fmt.Println("type:", reflect.TypeOf(p)) //*bytes.Reader
  fmt.Println(p) //&{[104 101 108 108 111 44 32 119 111 114 108 100 33] 0 -1}

  // from bytes buffer.
  var q io.Reader = bytes.NewBuffer([]byte("hello, world")) //带缓冲区 
  fmt.Println("type:", reflect.TypeOf(q)) //*bytes.Buffer
  fmt.Println(q) //hello, world

  //
  var m io.Reader = bufio.NewReader(strings.NewReader(string("hello, world")))
  fmt.Println("type:", reflect.TypeOf(m)) //*bufio.Reader
  fmt.Println(m) //&{[0 0 0 0...0] 0xc42000a220 0 0 <nil> -1 -1}
}

//io.Reader可以从很多地方读取内容
