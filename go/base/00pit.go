http://devs.cloudimmunity.com/gotchas-and-common-mistakes-in-go-golang/index.html
https://studygolang.com/articles/9995
http://blog.csdn.net/weiyuefei/article/details/77967619

1“{”不能单独放在一行 //看起来树虎
2未使用已定义的变量 //严谨
3未使用的包 //严谨
4只能在函数内部使用简短的变量声明 //全局区不能这样 myvar := 1 只能var myvar = 1
5无法使用精简的赋值语句对单个变量重新赋值
    one := 0
    one := 1 //error
    one, two := 1, 2 //ok

	data.result, err := work() //error
	var err error
	data.result, err = work() //ok
6隐式变量（作用域）
7除非特别指定，否则无法使用 nil 对变量赋值 
    //nil 可以用作 interface、function、pointer、map、slice 和 channel 的“空值”
    var x = nil //error
    var x interface{} = nil //ok
8Slice 和 Map 的 nil 值
    初始值为 nil 的 Slice 是可以进行“添加”操作的，但是对于 Map 的“添加”操作会导致运行时出错
	x := [3]int{1,2,3}
    func(arr [3]int) {
    //func(arr *[3]int) { //得用数组指针
        arr[0] = 7
        fmt.Println(arr) //prints [7 2 3]
    }(x) //数组指针穿参(&x)
    fmt.Println(x) //prints [1 2 3] (not ok if you need [7 2 3])
	再或者用slice即 x := []int{1,2,3}
9使用 Slice 和 Array 的 range 会导致预料外的结果
	//不是那个02问题这里讲的是range返回两个值
10 04.2array.go
11Accessing Non-Existing Map Keys
