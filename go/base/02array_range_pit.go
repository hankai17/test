package main

import (
	"fmt"
)

type student struct {
	Name string
	Age  int
}

func pase_student() {
	m := make(map[string]*student)

	stus := []student{
		{Name: "zhou", Age: 24},
		{Name: "li", Age: 23},
		{Name: "wang", Age: 22},
	}
    /*
    stus := []*student {
      &student{Name: "zhou", Age: 24},
      &student{Name: "li", Age: 23},
      &student{Name: "wang", Age: 22},
    }
    */

	for _, stu := range stus {
		m[stu.Name] = &stu
		//m[stu.Name] = stu
		fmt.Printf("%p\n", &stu)
	}
}

func main1() {
	pase_student()
	fmt.Printf("\n")
}

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

//会重用地址，也就是说，for _, stu := range stus 中的 stu 总是在同一个地址（你可以 fmt.Printf("%p\n", &stu) 试试)这样最终所有的结果都只是最后一个了。
//一定要如下所示用指针才行啊 stus := []*student{ &student{Name: "zhou", Age: 24}, &student{Name: "li", Age: 23}, &student{Name: "wang", Age: 22}, }

//数组中存放元素的地址 而非一个元素 //可以避免重复分配内存
//用for range来遍历数组或者map的时候，被遍历的指针是不变的，每次遍历仅执行struct值的拷贝
