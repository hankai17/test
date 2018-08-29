package main

import (
    "fmt"
    "reflect"
)

func main()  {
    defer func() {
       if err := recover(); err != nil {
           fmt.Println("---")
           fmt.Println(err)
           fmt.Println("---")
       } else {
           fmt.Println("fatal")
       }
        //panic("defer panic2")
    }()

    defer func() {
        panic("defer panic") //打印不出来
    }()
    panic("panic")
}

func main2()  {
    defer func() {
        if err:=recover();err!=nil{
            fmt.Println("++++")
            f:=err.(func()string)
            fmt.Println(err,f(),reflect.TypeOf(err).Kind().String())
        }else {
            fmt.Println("fatal")
        }
    }()

    defer func() {
        panic(func() string {
            return  "defer panic"
        })
    }()
    panic("panic")
}
