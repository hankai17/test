package main

import (
  "fmt"
)

func Any1(v interface{})  {
     v1:=int(v)
     println(v1)
     fmt.Println("")
}

func Any(v interface{})  {
    if v2, ok := v.(string);ok{
        println(v2)
    }else if v3,ok2:=v.(int);ok2{
        println(v3)
    }
}

func main() {
    Any(2)
    Any("666")
}
