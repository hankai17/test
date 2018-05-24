package main

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"time"
	//"io/ioutil"
	"os/exec"
)

func main() {
	//demo1()
	//fmt.Println()
	//demo2()
	demo3()
	time.Sleep(time.Second * 10)
}

func demo3() {
	fmt.Println("Run command `ps au | grep bash`: ")
	cmd1 := exec.Command("ps", "x") //au或者x
	cmd2 := exec.Command("grep", "bash")
	var outputBuf1 bytes.Buffer
	cmd1.Stdout = &outputBuf1
	cmd1.Start()
	cmd1.Wait()

	cmd2.Stdin = &outputBuf1

	var outputBuf2 bytes.Buffer
	cmd2.Stdout = &outputBuf2
	cmd2.Start()
	cmd2.Wait()
	fmt.Printf("%s\n", outputBuf2.Bytes())
}

func demo2() {
	fmt.Println("Run command `ps aux | grep bash`: ")
	cmd1 := exec.Command("ps", "x")
	cmd2 := exec.Command("grep", "bash")

	stdout1, _ := cmd1.StdoutPipe()
	cmd1.Start()
	cmd1.Wait()
	outputBuf1 := bufio.NewReader(stdout1)
	stdin2, _ := cmd2.StdinPipe()
	outputBuf1.WriteTo(stdin2) // stdout1-->outputBuf1-->stdin2

	//stdout1.Close()
	//cmd1.Process.Kill()

	var outputBuf2 bytes.Buffer //定义一个缓冲区
	cmd2.Stdout = &outputBuf2
	cmd2.Start()
	stdin2.Close()
	cmd2.Wait()

	fmt.Printf("%s\n", outputBuf2.Bytes())
	//stdout1.Close()
	//cmd1.Process.Kill()
	//cmd2.Process.Kill()
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
