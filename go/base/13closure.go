package main

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
    a()
    b()
}
//闭包引用相同变量
//闭包函数使用外部变量，是引用传递，注意：闭包函数运行时使用到该变量时，获取当时该变量的值
