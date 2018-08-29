1条件语句
	if a < 5 {
		return 0
	} else {
		return 1
	}
	条件语句无需括号
	{}必须存在
	左{ 必须与if/else同行
	if之后 条件之间 可添加变量初始化语句 使用;间隔
	有返回值的函数中 不允许将最终的return放在if else结构中 即上面的例子会报错

2选择语句
	switch i {
		case 0:
		    fmt.Printf("0")
		default:
		    fmt.Printf("default")
	}
	规则太多不想写了

3循环语句
	只有for关键字
	sum := 0
	for i := 0; i < 10; i++ {
		sum += i
	}
	而 是死循环的意思
	for {
		sum++
		if sum > 100 {
		    break
		}
	}
	多重赋值 ????????????????
	a := []int{1, 2, 3, 4, 5, 6}
	for i, j := 0, len(a) - 1; i < j; i, j = i + 1, j -1 {
		a[i], a[j] = a[j], a[i]
	}
4goto跳转语句
	func myfunc() {
	    i := 0
	    HERE:
	    fmt.Println(i)
	    i++;
            if i < 10 {
		goto HERE
	    }
	}

