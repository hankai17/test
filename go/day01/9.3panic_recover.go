package main

import (
  "fmt"
  "errors"
  //"debug"
)

func funcA() error {
    defer func() {
        if p := recover(); p != nil {
            fmt.Printf("panic recover! p: %v\n", p)
            //debug.PrintStack()
        }
    }()
    return funcB()
}
func funcA1() (err error) {
    defer func() {
        if p := recover(); p != nil {
            fmt.Println("panic recover! p:\n", p)
            str, ok := p.(string)
            if ok {
                err = errors.New(str)
            } else {
                err = errors.New("panic")
            }
            //debug.PrintStack()
        }
    }()
    return funcB()
}


func funcB() error {
    // simulation
    panic("foo")
    return errors.New("success")
}

func test() {
    err := funcA()
    if err == nil {
        fmt.Printf("err is nil\n")
    } else {
        fmt.Printf("err is %v\n", err)
    }
}

func main() {
	test()
}

/*
panic recover! p: foo
err is nil //panic异常处理机制不会自动将错误信息传递给error
*/
