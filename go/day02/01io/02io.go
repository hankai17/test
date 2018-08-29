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
  fmt.Println("type:", reflect.TypeOf(r)) //*strings.Reader 这个Reader类 确实是实现了Read so可以直接赋值给接口io.Reader
  fmt.Println(r) //&{hello, world 0 -1} //strings包的NewReader方法对 strings的Reader类包装了一层专门支持字符串

  // from bytes.
  var p io.Reader = bytes.NewReader([]byte("hello, world!")) //不带缓冲区
  fmt.Println("type:", reflect.TypeOf(p)) //*bytes.Reader 这个Reader类 确实实现了Read
  fmt.Println(p) //&{[104 101 108 108 111 44 32 119 111 114 108 100 33] 0 -1} //return &Reader{b, 0, -1}

  // from bytes buffer.
  var q io.Reader = bytes.NewBuffer([]byte("hello, world")) //带缓冲区 
  fmt.Println("type:", reflect.TypeOf(q)) //*bytes.Buffer 这个Buffer类 确实实现了Read
  fmt.Println(q) //hello, world //bytes包的NewBuffer方法对 bytes的Buffer类包装 //return &Buffer{buf: buf}

  //
  var m io.Reader = bufio.NewReader(strings.NewReader(string("hello, world")))
  fmt.Println("type:", reflect.TypeOf(m)) //*bufio.Reader 这个Reader类 确实实现了Read
  fmt.Println(m) //&{[0 0 0 0...0] 0xc42000a220 0 0 <nil> -1 -1}
}

/*
type Reader interface {
  	Read(p []byte) (n int, err error)
}
*/

//io.Reader可以从很多地方读取内容 这是因为io.Reader是接口类型扩展性非常强(只要实现了Read) 以上4个类都实现了Read函数
//只要一个类实现了Read方法就可调用Read(bytes.Buffer) 把数据读到bytes.Buffer
//一个结构体只要实现了接口的函数 就可以把此结构体赋值给接口


//https://studygolang.com/articles/9464
