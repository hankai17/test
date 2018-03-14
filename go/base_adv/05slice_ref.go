package main

import (
  "fmt"
)

func get() []byte {
    raw := make([]byte,10000)
   fmt.Println(len(raw),cap(raw),&raw[0]) //prints: 10000 10000 0xc420054000
    return raw[:3]
}

func get1() []byte {
    raw := make([]byte,10000)
    fmt.Println(len(raw),cap(raw),&raw[0]) //prints: 10000 10000 <byte_addr_x>

    res := make([]byte,3)
    copy(res,raw[:3])
    return res // 3 3 地址变化了
}

func main() {
    data := get1()
    fmt.Println(len(data),cap(data),&data[0]) //prints: 3 10000 0xc420054000
}
