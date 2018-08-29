1失败的原因只有一个时 尽量使用bool别用error 只有在想了解更多信息情况下采用error
		func my_fun() error {
		}
3错误值统一定义 而不是很随意
	var ERR_EOF = errors.New("EOF")
	var ERR_CLOSED_PIPE = errors.New("io: read/write on closed pipe")
	var ERR_NO_PROGRESS = errors.New("multiple Read calls return no data or error")
	var ERR_SHORT_BUFFER = errors.New("short buffer")
	var ERR_SHORT_WRITE = errors.New("short write")
	var ERR_UNEXPECTED_EOF = errors.New("unexpected EOF")
4错误逐层传递时层层都加日志
5错误处理使用defer
     func deferDemo() error { //不用defer时 很丑吧
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

	  func deferDemo() error { //用了defer 代码很漂亮
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
	  }

1在程序开发阶段坚持速错 就是让它挂:调panic中断程序执行以强制发生错误
2在程序部署后 跟上面完全相反 用revocer避免程序终止 即panic + recover机制  就是1+2机制
4对于不应该出现的分支 使用异常处理
		case "Clubs":
		    // ...
		default:
		    panic(fmt.Sprintf("invalid suit %v", s))





1内建函数
2假如函数F中书写了panic语句，会终止其后要执行的代码，在panic所在函数F内如果存在要执行的defer函数列表，按照defer的逆序执行
3返回函数F的调用者G，在G中，调用函数F语句之后的代码不会执行，假如函数G中存在要执行的defer函数列表，按照defer的逆序执行
4直到goroutine整个退出，并报告错误

