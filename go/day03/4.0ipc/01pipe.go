package main

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"os/exec"
)

func main() {
	//demo1()
	//fmt.Println()
	demo2()
}

func demo2() {
	fmt.Println("Run command `ps aux | grep bash`: ")
	cmd1 := exec.Command("ps", "aux")
	cmd2 := exec.Command("grep", "bash")
	stdout1, err := cmd1.StdoutPipe()
	if err != nil {
		fmt.Printf("Error: Can not obtain the stdout pipe for command: %s", err)
		return
	}
	if err := cmd1.Start(); err != nil {
		fmt.Printf("Error: The command can not running: %s\n", err)
		return
	}
	//至此数据已到stdout1中
	outputBuf1 := bufio.NewReader(stdout1)
	//至此数据已到outputBuf1中
	stdin2, err := cmd2.StdinPipe() //是io.WriteCloser类型 ...
	if err != nil {
		fmt.Printf("Error: Can not obtain the stdin pipe for command: %s\n", err)
		return
	}
	outputBuf1.WriteTo(stdin2) // stdout1-->outputBuf1-->stdin2
	//至此数据已到stdin2中

	var outputBuf2 bytes.Buffer //定义一个缓冲区
	cmd2.Stdout = &outputBuf2
	if err := cmd2.Start(); err != nil {
		fmt.Printf("Error: The command can not be startup: %s\n", err)
		return
	}
	err = stdin2.Close()
	if err != nil {
		fmt.Printf("Error: Can not close the stdio pipe: %s\n", err)
		return
	}
	if err := cmd2.Wait(); err != nil { //等待命令运行完毕后打印缓冲区数据
		fmt.Printf("Error: Can not wait for the command: %s\n", err)
		return
	}
	fmt.Printf("%s\n", outputBuf2.Bytes())
}

func demo1() {
	useBufferIo := true
	fmt.Println("Run command `echo -n \"My first command from golang.\"`: ")
	cmd0 := exec.Command("echo", "-n", "My first command from golang.")

	stdout0, err := cmd0.StdoutPipe() //stdout0是io.ReadCloser类型 这是个接口类型并扩展了接口类型io.Reader so就可以调用Read方法获取输出了
	/*
	 src/io/io.go源码包里看这个返回值类型
	 ReadCloser is the interface that groups the basic Read and Close methods.
	 126  type ReadCloser interface {
	 127  	Reader
	 128  	Closer
	 129  }	

	 	 type Reader interface {
     78  	Read(p []byte) (n int, err error)
     79  }

	 也就是说io.ReadCloser实现了Read方法!!!
	*/
	if err != nil {
		fmt.Printf("Error: Can not obtain the stdout pipe for command No.0: %s\n", err)
		return
	}

	if err := cmd0.Start(); err != nil {
		fmt.Printf("Error: The command No.0 can not be startup: %s\n", err)
		return
	}

	if !useBufferIo { //思路是先从管道里依次读到切片里 然后依次写到缓冲区里 
		var outputBuf0 bytes.Buffer //定义一个缓冲区
		for {
			tempOutput := make([]byte, 5)
			n, err := stdout0.Read(tempOutput) //调用Read方法
			if err != nil {
				if err == io.EOF {
					break
				} else {
					fmt.Printf("Error: Can not read data from the pipe: %s\n", err)
					return
				}
			}
			if n > 0 {
				outputBuf0.Write(tempOutput[:n])
			}
		}
		fmt.Printf("%s\n", outputBuf0.String())
	} else { //直接读到一个带缓冲器的容器中(带缓冲器的读取器bufio 默认4096字节长度)
		outputBuf0 := bufio.NewReader(stdout0) //stdout0的值也是io.Reader类型 so可以直接作为bufio的参数
		output0, _, err := outputBuf0.ReadLine() //从读取器中读一行 //第二个参数为false 意为当前行未读完
		if err != nil {
			fmt.Printf("Error: Can not read data from the pipe: %s\n", err)
			return
		}
		fmt.Printf("%s\n", string(output0))
	}
}
