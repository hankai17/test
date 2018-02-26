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
