package main
import (
  "fmt"
)

func calc(index string, a, b int) int {
	ret := a + b
	fmt.Println(index, a, b, ret)
	return ret
}

func main1() {
	a := 1
	b := 2

	calc("1", a, calc("10", a, b)) //1 2 3; 1 3 4

	a = 0
	calc("2", a, calc("20", a, b)) //0 2 2; 0 2 2
	b = 1
}

func main() {
	a := 1
	b := 2

	defer calc("1", a, calc("10", a, b)) //defer只作用到了最外面的那个函数

	a = 0
	defer calc("2", a, calc("20", a, b)) //
	b = 1
}
//10 1 2
//20 0 2
//2 0 2
//1 0 3 !!!
