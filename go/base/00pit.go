http://devs.cloudimmunity.com/gotchas-and-common-mistakes-in-go-golang/index.html
https://studygolang.com/articles/9995
http://blog.csdn.net/weiyuefei/article/details/77967619

1"{"不能单独放在一行 //看起来树虎
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
8slice和map的nil
    初始值为nil的slice可以进行“添加”操作的但是对于map“添加”操作会导致运行时出错
    eg: var s []int
        s = append(s,1) //ok
    eg: var m map[string]int
        m["one"] = 1 //error 

9使用slice和Array的range会导致预料外的结果
	//不是那个02问题这里讲的是range返回两个值
10字符串不能为空 eg: var x string = nil //error
11如果返回值中有名称则全都得带名称
    func funcMui(x,y int)(sum int,error){ //改为(sum int, er error)
        return x+y,nil
    }
12用数组指针作为参数
	x := [3]int{1,2,3}
    func(arr [3]int) { //复制了一份
    //func(arr *[3]int) { //得用数组指针
        arr[0] = 7
        fmt.Println(arr) //prints [7 2 3]
    }(x) //数组指针穿参(&x)
    fmt.Println(x) //prints [1 2 3] (not ok if you need [7 2 3])
	再或者用slice即 x := []int{1,2,3}
13slice跟array是一维的04.2array.go
14如何确认map中不存在的key
    x := map[string]string{"1":"a", "2":"", "3":"c"}
    //if v := x["2"]; v == "" { //error
    if _, v := x["2"]; !v { //找到则v为true 否则为false
      //no entry
    }
15strings无法修改
     x := "text"
     x[0] = 'T' //error

     x := "text"
     xbytes := []byte(x)
     xbytes[0] = 'T' //ok
16string和byte slice之间转换 ??????
    Go在[]byte到string和string到[]byte的转换中使用优化来避免额外分配
    第一个优化避免了当 []byte key用于在 map[string]集合中查询时的额外分配: m[string(key)]。
    第二个优化避免了字符串转换为[]byte后在 for range语句中的额外分配for i,v := range []byte(str) {...}。
17string和索引操作
    x := "text"
    fmt.Println(x[0]) //print 116 //字符串索引返回byte值
    fmt.Printf("%T",x[0]) //prints uint8
18字符串不总是utf8格式
    data1 := "ABC"
    fmt.Println(utf8.ValidString(data1)) //prints: true

    data2 := "A\xfeC"
    fmt.Println(utf8.ValidString(data2)) //prints: false
19字符串长度
    data := "♥"
    fmt.Println(len(data)) //prints: 3 //返回byte数量
    fmt.Println(utf8.RuneCountInString(data)) //prints: 1
20在多行的slice、array和map语句中遗漏逗号
    x := []int{
            1,
            2 //error
    }
    y := []int{3, 4} //ok
21log.Fatal和log.Panic不仅仅是Log
    log.Fatalln("Fatal Level: log entry") //app exits here
    log.Println("Normal Level: log entry")
22内建的数据结构操作不是同步的
23string在“range”语句中的迭代值
    range第一个返回值是字节索引  for range语句将会尝试把数据翻译为UTF8文本
24对Map使用“for range”语句迭代
    Go的runtime有心尝试随机化迭代顺序，但并不总会成功
    就是说range遍历结果可能有不同顺序
25"switch"声明中的失效行为
    switch(ch) {
        case ' ': //error
        case '\t':
            return true
        }
	ch为空 当匹配到第一个case时 就break出去了 根本不会走第二个case
	改为
	switch(ch) {
        case ' ', '\t':
        return true
        }
26自增和自减
	不支持前置++ 也不支持在表达式中使用
	data := []int{1,2,3}
    i := 0
    ++i //error
    fmt.Println(data[i++]) //error

	i++ //ok
    fmt.Println(data[i]) //ok
27按位NOT操作
	????
28操作优先级的差异
	????
29未导出的结构体不会被编码
	以小写字母开头的结构体将不会被（json、xml、gob等）编码，因此当你编码这些未导出的结构体时，你将会得到零值。
30有活动的Goroutines下的应用退出
	03案例
31向无缓存的Channel发送消息，只要目标接收者准备好就会立即返回
    ch := make(chan string)
    go func() {
        for m := range ch {
            fmt.Println("processed:",m)
        }
    }()
    ch <- "cmd.1"
    ch <- "cmd.2" //won't be processed
32向已关闭的Channel发送会引起Panic
	16案例
33使用"nil" Channels
34传值方法的接收者无法修改原有的值
