package main

import (
	"bytes"
	//"reflect"
	"fmt"
	"strconv"
	"time"
)


func spilt(r rune) bool {
	if r == 'c' {
		return true
	}
	return false
}

func main() { //bytes的效率
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

func write_uint(buff []byte, data uint16) {
	for i := 0; i < 2; i++ {
		buff[i] = byte(data >> uint(i*8)) //分别取data的后8位 和 前8位
	}
}

func main1() {
	println("hello world")
	buff1 := make([]byte, 2) //创建一个切片
	write_uint(buff1, uint16(12))

	buff2 := make([]byte, 2) //创建一个切片
	write_uint(buff2, uint16(12))
	//比较两个字节数组切片
	res := bytes.Compare(buff1, buff2)
	println(res)

	//字符串转换为字节数组
	buff3 := []byte("hello world hello world")
	seq := []byte("hello")
	// Count counts the number of non-overlapping instances of sep in s
	res = bytes.Count(buff3, seq)
	println(res)
	// Contains reports whether subslice is within b
	contains := bytes.Contains(buff3, seq) //true
	println(contains)

	res = bytes.Index(buff3, seq) //0 第一次出现位置
	println(res)
	res = bytes.LastIndex(buff3, seq) //12 最后一次出现位置
	println(res)

	/*
	Rune literals are just an integer value (as you've written).
	They are "mapped" to their unicode codepoint.
	*/
	a := rune('e') //rune等价于int32
	res = bytes.IndexRune(buff3, a) //1
	println(res)


	buff5 := []byte("abcabcabcabc")
	arr := bytes.SplitN(buff5, []byte("a"), -1)
	//println(reflect.TypeOf(arr)) //看不出来
	for _, v := range arr { //二维数组
		for _, t := range v {
			print(t, " ")
		}
		println("|")
	}

	// Fields 以连续的空白字符为分隔符，将 s 切分成多个子串，结果中不包含空白字符本身
	// 空白字符有：\t, \n, \v, \f, \r, ' ', U+0085 (NEL), U+00A0 (NBSP)
	// 如果 s 中只包含空白字符，则返回一个空列表
	buff6 := []byte("abc abc abc    abc")
	arr = bytes.Fields(buff6)
	for _, v := range arr {
		for _, t := range v {
			print(t, " ")
		}
		println("|")
	}

	// FieldsFunc 以一个或多个连续的满足 f(rune) 的字符为分隔符，
	// 将 s 切分成多个子串，结果中不包含分隔符本身
	// 如果 s 中没有满足 f(rune) 的字符，则返回一个空列表
	print("------------\n")
	buff7 := []byte("abcabcabcabc")
	arr = bytes.FieldsFunc(buff7, spilt)
	for _, v := range arr { //4个
		for _, t := range v {
			print(t, " ")
		}
		println("|")
	}

/*
	buff8 := []byte("我是中国人")
	// 将 s 切分为 Unicode 码点列表
	data := bytes.Runes(buff8)
	for _, elem := range data {
		println(string(elem))
	}

	// Title 将 s 中的所有单词的首字母修改为其 Title 格式
	buff9 := bytes.Title(buff7)
	println(string(buff9))

	// Map 将 s 中满足 mapping(rune) 的字符替换为 mapping(rune) 的返回值
	// 如果 mapping(rune) 返回负数，则相应的字符将被删除
	buff10 := bytes.Map(func(r rune) rune {
		if r == 'c' {
			return 'a'
		}
		return r
	}, buff7)
	println(string(buff10))
	*/
}
