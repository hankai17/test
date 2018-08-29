package main

import (
    "fmt"
    "os"
    "bufio"
)
//查找输入行中重复行
func findDup() {
    maps := make(map[string]int)
    scanner:=bufio.NewScanner(os.Stdin)
    for scanner.Scan(){
        maps[scanner.Text()]++
    }
    for line, count := range maps{
        if count >0 {
            fmt.Printf("line: %s, count: %d\n",
                line, count)
        }
    }
}

func main() {
  counts := make(map[string]int)
  input := bufio.NewScanner(os.Stdin)
  for input.Scan() {
    counts[input.Text()]++
  }

  for line, n := range counts {
    if n > 1 {
      fmt.Printf("%d\t%s\n",n,line)
    }
  }
	//findDup();
}
//ctrl + d 即发送一个eof

/*

bufio常用方法
reader:=bufio.NewReader(oldReader) // 默认size为4096
reader:=bufio.NewReaderSize(oldReader, size)

bytes, error = reader.ReadSlice(delim) //结束符
bytes, error = reader.ReadString(delim)
bytes, error = reader.ReadLine() //默认换行符\n
bytes, error = reader.ReadBytes(delim)
n, error = reader.Reader([]byte)
*/
