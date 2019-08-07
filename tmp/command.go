package main

import (
    "fmt"
    "strings"
    "os/exec"
    "bufio"
    "bytes"
    "os"
)

func main() {
    query1()
}

func query1() {
    fd, err := os.Open("./1.txt")
    if err != nil {
        panic(err)
    }
    defer func() {
        if err := fd.Close(); err != nil {
            panic(err)
        }
    }()

    scanner := bufio.NewScanner(fd)
    for scanner.Scan() {
        fields := strings.Fields( strings.Trim( strings.TrimSpace(scanner.Text()) ,"\n"))
        if len(fields) == 0 {
            continue
        }
        key := fields[0]
        _ = key
        //fmt.Println(key)

        /*
        var array = [...] []string {
            []string{key, "*", "-nR", "--color"},
        }
        //cmd1 := exec.Command("grep", array[0]...)
        */

        //cmd1 := exec.Command("ls", "-al")
        //cmd1 := exec.Command("egrep", key, "http_stream.c")
        command := "grep " + key + " " + "* -nR"
        fmt.Println(command)
        
        cmd1 := exec.Command("/bin/sh", "-c", command)
        var outputBuf bytes.Buffer
        cmd1.Stdout = &outputBuf
        if err := cmd1.Start(); err != nil {
            fmt.Printf("Error: The command can not running: %s\n", err)
            return
        }

        if true {
            if err := cmd1.Wait(); err != nil { //等待命令运行完毕后打印缓冲区数据
                fmt.Printf("Error: Can not wait for the command: %s\n", err)
                return
            }
            fmt.Println(string(outputBuf.Bytes()))
        } else {
            output, err := cmd1.CombinedOutput()
            if err != nil {
                fmt.Println(fmt.Sprint(err) + ": " + string(output))
                    return
            }
            fmt.Println(string(output))
        }
    }
}
