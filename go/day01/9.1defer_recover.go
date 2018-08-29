package main

import (
  "fmt"
)

func main1() {
    recover() //doesn't do anything
    panic("not good") //return
    recover() //won't be executed
    fmt.Println("ok")
}

func main2() {
    defer func() { //recover仅在defer函数中被直接调用时才有效
		fmt.Println("222")
        fmt.Println("recovered:",recover())
    }()
	fmt.Println("111")
    panic("not good")
	fmt.Println("333")
}
//111
//222
//recovered: not good

func doRecover() {
    fmt.Println("recovered =>",recover()) //prints: recovered => <nil>
}

func main() {
    defer func() {
        doRecover() //panic is not recovered
    }()
    panic("not good")
}
