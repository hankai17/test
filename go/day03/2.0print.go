package main

import (
   "fmt"
)

type Human struct {
    Name string
}

func main() {
	var people = Human{ Name:"zhangsan" }
	fmt.Println("%v", people) //字符串组合 

	fmt.Printf("%+v\n", people) //格式化字符串
	//fmt.Printf("#v", people)

	fmt.Printf("%t\n", true) //布尔占位符
	fmt.Printf("%b\n", 5) //二进制
	fmt.Printf("%c\n", 0x4E2D) //unicode所代表的字符
	fmt.Printf("%d\n", 0x12) //10进制
	fmt.Printf("%o\n", 10) //8进制
	fmt.Printf("%q\n", 0x4E2D) //中
	fmt.Printf("%x\n", 13) //16进制小写
	fmt.Printf("%X\n", 13)
	fmt.Printf("%U\n", 0x4E2D) 

    //https://studygolang.com/articles/2644?fr=sidebar
}
