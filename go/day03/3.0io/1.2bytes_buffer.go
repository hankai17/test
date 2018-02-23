package main

import (
	"bytes"
	"fmt"
	"strconv"
	"time"
)

func main() {
	var buffer bytes.Buffer
	ttime := time.Now().UnixNano()
	for i := 0; i < 10000000; i++ {
		//fmt.Println(strconv.Itoa(i))
		buffer.WriteString(strconv.Itoa(i))
		//time.Sleep(1e9) //ns级别
	}
	ttime1 := time.Now().UnixNano()

	//取内容buffer.Bytes() 或者 buffer.String()
    fmt.Printf("time cal %f %d\n", float64(ttime1-ttime)/float64(1*time.Second), len(buffer.String()))
}
