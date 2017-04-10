1
	select {
		case <- chan1: /如果chan1成功读到数据 就进行处理语句
			...
		case chan2 <- 1:
			...
		default:
			...
	}


