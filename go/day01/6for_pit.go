package main

import (
    "fmt"
    "time"
)

func main1() {
    data := []string{"one", "two", "three"}
    for _, v := range data {
		tmp := v
        go func() {
            fmt.Println(tmp)
        }()
    }
    time.Sleep(3 * time.Second)
    //goroutines print: three, three, three
}

func main2() {
    data := []string{"one", "two", "three"}
    for _, v := range data {
        go func(in string) {
            fmt.Println(in)
        }(v)
    }
    time.Sleep(3 * time.Second)
}

type field struct {
    name string
}
func (p *field) print() {
    fmt.Println(p.name)
}

func main() {
    data := []field{ {"one"}, {"two"}, {"three"} }
    for _, v := range data { //v是复杂对象的拷贝
		//tmp := v
        //go tmp.print()
        go v.print()
    }

    time.Sleep(3 * time.Second)
    //goroutines print: three, three, three
}

func main4() {
    data := []*field{{"one"}, {"two"}, {"three"}}
    for _, v := range data { //v是指针
        go v.print()
    }
    time.Sleep(3 * time.Second)
}
