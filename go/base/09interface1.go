package main

import (
	"fmt"
)

type People interface {
	//Show()
}

type Student struct{}
func (stu *Student) Show() {
}

func live() People {
	var stu *Student
	return stu
}

func main1() {
	if live() == nil {
		fmt.Println("AAAAAAA")
	} else {
		fmt.Println("BBBBBBB")
	}
}

func Foo(x interface{}) {
	if x == nil {
		fmt.Println("empty interface")
		return
	}
	fmt.Println("non-empty interface")
}
func main() {
	var x *int = nil
	Foo(x)
}
