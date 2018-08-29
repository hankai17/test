package main

import (
	"bytes"
	"fmt"
	"time"
	"os/exec"
)

func main() {
	runCmdWithPipe()
	//time.Sleep(time.Second * 10)
}
func runCmdWithPipe() {
	cmd1 := exec.Command("/root/sleep10s") //au或者x
	var outputBuf1 bytes.Buffer

	done := make(chan error, 1)
	cmd1.Stdout = &outputBuf1

	if err := cmd1.Start(); err != nil {
		fmt.Printf("error: The first command can not be startup %s\n", err)
		return
	}

	go func() {
		 done <-cmd1.Wait() //这个函数会阻塞
	} ()

	timeout1 := time.After(time.Second * 2)
	select {
		case err := <-done:
			if err != nil {
				fmt.Println("command wait err")
				return
			}
		case <-timeout1:
			fmt.Println("time out")
	}

	fmt.Printf("-%s-\n", string(outputBuf1.Bytes()))
}

func runCmdWithPipe1() {
	cmd1 := exec.Command("/root/sleep10s") //au或者x
	var outputBuf1 bytes.Buffer

	done := make(chan error, 1)
	cmd1.Stdout = &outputBuf1

	if err := cmd1.Start(); err != nil {
		fmt.Printf("error: The first command can not be startup %s\n", err)
		return
	}

	go func() {
		timeout1 := time.After(time.Second * 1)
		for {
			select {
				case done <-cmd1.Wait(): //这个函数会阻塞
					fmt.Println("has ret")
				case <-timeout1:
					fmt.Println("1s timeout")
			}
		}
	} ()

	select {
		case err := <-done:
			if err != nil {
				fmt.Println("command wait err")
				return
			}
	}

	fmt.Printf("-%s-\n", string(outputBuf1.Bytes()))
}
