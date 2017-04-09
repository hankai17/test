错误处理
1error接口
	type error interface {
	    Error() string 
	}
	常用用法：
	func Foo(param int) (n int,err error) {
		// ...
	}
	调用时错误处理
	n, err := Foo(0)
	if err != nil {
		//... 错误处理
	} else {
		//...
	}

	自定义一个error类型
	type PathError struct {
	    Op 	  string
	    Path  string
            Err   error
	}	
	func (e *PathError) Error() string {
		return e.Op + " " + e.Path + ": " + e.Err.Error()
	}

2defer
	不懂
3panic()与recover()
	
