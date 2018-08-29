package main

import (
	"fmt"
)

func main() {
	//s := make([]int, 5) //make必须指定切片长度
	s := make([]int, 0) //make必须指定切片长度
	s = append(s, 1, 2, 3)

    //b := [2]int{0, 0}
    //s = append(s, b)
	fmt.Println(s)
}

func main3() {
	s1 := []int{1, 2, 3}
	s2 := []int{4, 5}
	//s1 = append(s1, s2)
	s1 = append(s1, s2...) //append切片时候必须加... 数组没有append
	fmt.Println(s1)
}

