package main

import "fmt"

type data struct {
    name string
}
func (p *data) print() {
    fmt.Println("name:", p.name)
}

type printer interface {
    print()
}

func main() {
    d1 := data{"one"}
    d1.print() //ok

    //var in printer = data{"two"} //error
    var in printer = &data{"two"} //ok
    in.print()

    //n := map[string]data {"x": data{"three"}}
    //fmt.Println(&n["x"]) //cannot take the address of n["x"]
    //n["x"].print() //so this is error //因为有个点 所以肯定是寻址操作

    m := map[string]*data {"x": &data{"three"}}
    m["x"].print() //ok
}
