package main

import (
	"fmt"
)

func main() {
	var j int = 5
	a := func()(func()) { //相当于一个对象
		var i int = 10
		return func() {
		    i += 1
		    j += 1
			fmt.Printf("i, j: %d, %d\n", i, j);
		}
	}()

	a() //11 6
	j *= 2
	a() //12 13
}

func adder() func(int) int { //func后面意为参数int返回值int
     sum := 0
     return func(x int) int {
          sum += x
          return sum
     }
}

func main1() {
     pos, neg := adder(), adder()
     for i := 0; i < 10; i++ {
          fmt.Println(
               pos(i),
               neg(-2*i),
          )
     }
}

/*
0 0
1 -2
3 -6
6 -12
10 -20
15 -30
21 -42
28 -56
36 -72
45 -90
*/

func test(x int) (func(),func()) {
    return func() { //必须这种格式！！！
             println(x)
             x += 10
           }, func() {
              println(x)
           }
}

func main()  {
    a, b := test(100)
    a() //100
    b() //110
}
//闭包引用相同变量
//闭包函数使用外部变量，是引用传递，注意：闭包函数运行时使用到该变量时，获取当时该变量的值

