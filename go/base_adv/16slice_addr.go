package main

import (
  "fmt"
)

type data struct {
    name string
}

func main1() {
    //m := map[string]data {"x": {"one"}}
    //m["x"].name = "two" //error //map中的元素不能取地址

	s := []data {{"one"}}
    s[0].name = "two" //ok //slice可以取地址
    fmt.Println(s)    //prints: [{two}]
}

func main1() {
    m := map[string]data {"x": {"one"}}
    r := m["x"] //解决方案是临时值
    r.name = "two"
    m["x"] = r
    fmt.Printf("%v", m) //prints: map[x:{two}]
}

//解决方案二
func main() {
    m := map[string]*data {"x": {"one"}}
    m["x"].name = "two" //ok
    fmt.Println(m["x"]) //prints: &{two}
}
