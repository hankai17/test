package main

import (
    "fmt"
    "time"
)

func main() {
    data := []string{"one", "two", "three"}
    for _, v := range data {
		tmp := v
        go func() {
            fmt.Println(tmp)
        }()
        fmt.Println("----") //先for循环完 再起协程
    }
    time.Sleep(3 * time.Second)
    //goroutines print: three, three, three
}

func main2() {
    data := []string{"one", "two", "three"}
    for _, v := range data {
        go func(in string) {
            fmt.Println(in)
        }(v) //当做匿名参数  goroutine开始执行时就会得到那个变量的值
    }

    time.Sleep(3 * time.Second)
    //goroutines print: one, two, three
}

type field struct {
    name string
}
func (p *field) print() {
    fmt.Println(p.name)
}

func main3() {
    data := []field{ {"one"}, {"two"}, {"three"} }

    for _, v := range data {
		tmp := v
        go tmp.print()
        //go v.print()
    }

    time.Sleep(3 * time.Second)
    //goroutines print: three, three, three
}

func main4() {
    data := []*field{{"one"}, {"two"}, {"three"}}
    for _, v := range data {
        go v.print()
    }
    time.Sleep(3 * time.Second)
}
