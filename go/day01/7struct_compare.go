package main

import "fmt"

type data struct {
    num int
    fp float32
    complex complex64
    str string
    char rune
    yes bool
    events <-chan string
    handler interface{}
    ref *byte
    raw [10]byte
}

func main1() {
    v1 := data{}
    v2 := data{}
    fmt.Println("v1 == v2:",v1 == v2) //prints: v1 == v2: true
}

type data struct {
    num int                //ok
    checks [10]func() bool //not comparable
    doit func() bool       //not comparable
    m map[string] string   //not comparable
    bytes []byte           //not comparable
}//注意数组仅在它们的数据元素可比较的情况下才可以比较。

func main() {
    v1 := data{}
    v2 := data{}
    fmt.Println("v1 == v2:",v1 == v2)
}
