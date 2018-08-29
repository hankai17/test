1函数定义
	package  mymath
	import "errors"
	func Add(a int, b int) (ret int, err error) {
		if a < 0 || b < 0 {
		     err = errors.New("should be non-negative num")
		     return
		}
		return a + b, nil
	}
	若a b类型相同 可写为 func Add(a, b int) (ret int, err error) {
	若返回值只有一个 可写为 func Add(a, b int) int {

2函数调用
	只要导入包 就可以用这个包里的函数
	import "mymath"
	c := mymath.Add(1,2)
	记住 Add必须要大写 大写才对外可见

3不定参数
        首先range有两个返回值：索引和元素
        func myfunc(args ...int) {
                for _, arg := range args {
                        fmt.Println(arg)
                }
        }
        函数myfunc接受不定数量的参数，这些参数类型都是int
        eg myfunc(1,3,4) myfunc(1,4,5,7,8,9)
        ...type 是一个语法糖 其本质是数组切片[]type
        这就是为什么args可以用for循环获取每个传入参数

        若没有这个语法糖 则得这样写：
        func myfunc2(args []int) {
                for _, arg := range args {
                        fmt.Println(arg)
                }
        }
        但是调用的时候 mfunc2([]int{1,3,7,12})  必须得用[]int{}构造数组切片

4多返回值
        多返回值是优美的 简洁的
        func (file *File) Read(b []byte) (n int, err Error)
        若只关心读到的个数 可这样写 n, _ := f.Read(buf)

5匿名函数与闭包
        匿名函数没有名字
        func(a, b int, z float64) bool {
                return a*b < (int)z
        }

        f := func(x, y int) int {  //赋值给一个变量
		return x + y
		}

	    func(ch chan int) {
		    ch <- ACK
	    } (reply_chan)  //{}后面有参数列表 代表函数调用

	闭包
	不太懂

