package main

import (
	"bytes"
	"fmt"
	"io"
)

func main() {
	b1 := []byte("Hello World!")
	reader := bytes.NewReader(b1)

	// Read 方法 //把byte中的数据读到buff里
	buff := make([]byte, 5)
	count, err := reader.Read(buff)
	if err != nil {
		return
	}
	fmt.Printf("read count = %d,read data = %s\n", count, string(buff))

	// ReadAt 方法 //从byte中偏移6处读到buff里
	buff2 := make([]byte, 5)
	count, err = reader.ReadAt(buff2, 6)
	if err != nil {
		return
	}
	fmt.Printf("read count = %d,read data = %s\n", count, string(buff2))

	for {
		// 依次返回未被读取的字节
		b, err := reader.ReadByte() //ReadAt不算消费
		if err == io.EOF {
			break
		}
		println(string(b))
	}

	println("--------")

	b2 := []byte("hello 世界！")
	reader2 := bytes.NewReader(b2)
	for {
		// 依次返回未被读取的rune
		r, _, err := reader2.ReadRune()
		if err == io.EOF {
			break
		}
		println(string(r))
	}

	b3 := []byte("string builder")
	// Reset resets the Reader to be reading from b.
	reader2.Reset(b3)
	println(reader2.Len())

	println("--------")
	reader3 := bytes.NewReader(b1)
	// Seek 设置下一次 Read 或 Write 的偏移量为 offset，它的解释取决于 whence：
	// 0 表示相对于文件的起始处，1 表示相对于当前的偏移，而 2 表示相对于其结尾处。
	// Seek 返回新的偏移量和一个错误，如果有的话。
	abs, err := reader3.Seek(-2, 2)
	println(abs)
	b, _ := reader3.ReadByte()
	println(string(b))
}
