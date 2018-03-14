panic：
1、内建函数
2、假如函数F中书写了panic语句，会终止其后要执行的代码，在panic所在函数F内如果存在要执行的defer函数列表，按照defer的逆序执行
3、返回函数F的调用者G，在G中，调用函数F语句之后的代码不会执行，假如函数G中存在要执行的defer函数列表，按照defer的逆序执行
4、直到goroutine整个退出，并报告错误

panic使用原则
	1 失败的原因只有一个时，不使用error, 尽量使用bool 只有在想了解更多信息情况下采用error
		func my_fun() error {
		}
	2 尽量不要使用返回值
	3 错误值统一定义，而不是跟着感觉走
		var ERR_EOF = errors.New("EOF")
		var ERR_CLOSED_PIPE = errors.New("io: read/write on closed pipe")
		var ERR_NO_PROGRESS = errors.New("multiple Read calls return no data or error")
		var ERR_SHORT_BUFFER = errors.New("short buffer")
		var ERR_SHORT_WRITE = errors.New("short write")
		var ERR_UNEXPECTED_EOF = errors.New("unexpected EOF")
	4 错误逐层传递时，层层都加日志

    5 错误处理使用defer
        func deferDemo() error {
		  err := createResource1()
		  if err != nil {
			  return ERR_CREATE_RESOURCE1_FAILED
		  }
		  err = createResource2()
		  if err != nil {
			  destroyResource1()
			  return ERR_CREATE_RESOURCE2_FAILED
		  }

		  err = createResource3()
		  if err != nil {
			  destroyResource1()
			  destroyResource2()
			  return ERR_CREATE_RESOURCE3_FAILED
		  }

		  err = createResource4()
		  if err != nil {
			  destroyResource1()
			  destroyResource2()
			  destroyResource3()
			  return ERR_CREATE_RESOURCE4_FAILED
		  }
		  return nil
	  }

	  func deferDemo() error {
		  err := createResource1()
		  if err != nil {
			  return ERR_CREATE_RESOURCE1_FAILED
		  }
		  defer func() {
			  if err != nil {
				  destroyResource1()
			  }
		  }()
		  err = createResource2()
		  if err != nil {
			  return ERR_CREATE_RESOURCE2_FAILED
		  }
		  defer func() {
			  if err != nil {
				  destroyResource2()
			  }
		  }()

		  err = createResource3()
		  if err != nil {
			  return ERR_CREATE_RESOURCE3_FAILED
		  }
		  defer func() {
			  if err != nil {
				  destroyResource3()
			  }
		  }()

		  err = createResource4()
		  if err != nil {
			  return ERR_CREATE_RESOURCE4_FAILED
		  }
		  return nil
	  } //对于闭包的参数是值传递，而对于外部变量却是引用传递，所以闭包中的外部变量err的值就变成外部函数返回时最新的err值。

	6当尝试几次可以避免失败时，不要立即返回错误
	7当上层函数不关心错误时，建议不返回error


使用时候
	1在程序开发阶段，坚持速错 简单来讲就是“让它挂” 最简单的同时也可能是最好的方法是调用panic函数来中断程序的执行以强制发生错误，使得该错误不会被忽略，因而能够被尽快修复。
	2在程序部署后，应恢复异常避免程序终止 Goroutine如果panic了，并且没有recover，那么整个Golang进程就会异常退出
    3 so一旦Golang程序部署后，在任何情况下发生的异常都不应该导致程序异常退出，我们在上层函数中加一个延迟执行的recover调用来达到这个目的，并且是否进行recover需要根据环境变量或配置文件来定，默认需要recover。
	4 对于不应该出现的分支，使用异常处理
		case "Clubs":
		    // ...
		default:
		    panic(fmt.Sprintf("invalid suit %v", s))
    5 针对入参不应该有问题的函数，使用panic设计
    	

