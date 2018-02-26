package main

import (
	"bytes"
	"fmt"
	"io"
)

func main() { //bytes.Buffer
    data := "123456789";
    bf := bytes.NewBuffer([]byte(data));
	fmt.Println(bf)

    //Len()返回未读取的数据长度
    fmt.Println("bf len : ", bf.Len());
    //Cap()缓存容量
    fmt.Println("bf cap : ", bf.Cap());

    //Bytes()返回未读取的数据切片
    bys := bf.Bytes();
    for _, v := range bys {
        fmt.Print(string(v) + " ");
    }
    fmt.Println();

    //Next()返回未读取部分前n字节数据的切片
    for i := 0; i < 10; i++ {
        tmp := bf.Next(1);
        fmt.Print(string(tmp) + " ");
    }
    fmt.Println();
    //再次Next，返回[]byte，说明没有未读取的
    fmt.Println(bf.Next(1));

    //重设缓冲，丢弃全部内容
    bf.Reset();

    //通过string创建Buffer
    bf2 := bytes.NewBufferString(data);
    //读取第一个 delim 及其之前的内容，返回遇到的错误
    line, _ := bf2.ReadBytes('3');
    fmt.Println(string(line));
    //效果同上，返回string
    line2, _ := bf2.ReadString('7');
    fmt.Println(line2);

    //创建一个空Buffer
    bf3 := bytes.Buffer{};
    //自动增加缓存容量，保证有n字节剩余空间
    bf3.Grow(16);
    //写入rune编码，返回写入的字节数和错误。
    n, _ := bf3.WriteRune(rune('中'));
    fmt.Println("bf3 write ", n);
    n, _ = bf3.WriteString("国人");
    fmt.Println("bf3 write ", n);
    //返回未读取的字符串
    fmt.Println(bf3.String());
    //将数据长度截断到n字节
    bf3.Truncate(6);
    fmt.Println(bf3.String());
}

func main1() { //bytes.Reader
	b1 := []byte("Hello World!")
	reader := bytes.NewReader(b1)
	//fmt.Println(reader) //&{[72 101 108 108 111 32 87 111 114 108 100 33] 0 -1}

    //bytes.Reader的Read方法
	buff := make([]byte, 5)
	count, err := reader.Read(buff)
	if err != nil {
		return
	}
	fmt.Printf("read count = %d,read data = %s\n", count, string(buff))

	//ReadAt方法从byte中偏移6处读到buff里
	buff2 := make([]byte, 5)
	count, err = reader.ReadAt(buff2, 6)
	if err != nil {
		return
	}
	fmt.Printf("read count = %d,read data = %s\n", count, string(buff2))

	for {
		//返回未被读取的字节
		b, err := reader.ReadByte() //ReadAt不算消费
		if err == io.EOF {
			break
		}
		println(b, string(b))
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
