1bool
    var v1 bool
    v1 = true
    v2 := (1 == 2) //v2被自动推导为bool类型
    布尔类型不支持其他类型的赋值，不支持强制类型转换
    eg :
    var b bool
    b = 1 //err
    b = bool(1) //err

2整形
    int 与 int32是两种不同类型 编译器不会帮你转换
    eg :
    var v2 int32
    v1 := 64 //推导为int
    v2 = v1 //error
    但支持强转
    v2 = int32(v1)

    类型不同不能直接比较 int 和 int32不能直接比较

    ^2 为什么2取反后是-3？

3浮点型
    float64 float32
    var f1 float32
    f1 = 12
    f2 := 12.0 //推导成float64  否则推导成int

    浮点数比较：因为浮点数不精确
    import "math"
    func is_equal(f1, f2, p float64) bool {
        return math.Abs(f1 - f2) < p
    }

4复数类型
    var v1 complex64
    v1 = 3.2 + 12i
    v2 := 3.2 +12i
    v3 := complex(3.2, 12) //3者相同
    real(v3) 获取实部3.2 img(v3)获取虚部12

5字符串
    var str string
    string = "hello world"
    ch := str[0]
    fmt.printf("The length of \"%s\" is %d \n", str, len(str)) //引号内的引号得转义  11
    fmt.printf("The first character of \"%s\" is %c \n", str, ch) //引号内的引号得转义

    str := "mike dog"
    str[0] = 'c' //error

    字符串操作：
        str1 + str2 字符串连接
        len(str1) 字符串长度
    字符串遍历：
        字节数组方式遍历：
        str := "hello,你坏"
        n := len(str)
        for i := 0; i < n; i++ {
            ch := str[i] //每个字符类型为byte
            fmt.Println(i,ch)
        } //输出的是序号 和 相应的utf8 码值  一个汉字三个字节表示utf8值
        unicode字符遍历：
        str := "hello,你坏"
        for i, ch := range str {
            fmt.Println(i, ch)  //每个字符类型为rune
        } //一个汉字一个unicode值
6字符类型：
    go支持两种字符类型：
        byte(uint8的别名) 代表utf8字符的单个字节
        rune 代表unicode字符

7数组:
    也有"元素"的概念
    数组的声明：
       [32]byte
       [2*N] struct { x, y int32 }
       [1000]*float64 //指针数组
       [3][5]int //二维数组 3个元素
       [2][2][2]float64  等价于[2]([2]([2]float64))

       数组常见定义
       array := [5]int{1, 2, 3, 4, 5}

       arr_len := len(arr) //得出总元素个数
       元素访问:
            for i := 0; i< len(array); i++ {
                fmt.Println("element", i, "of array is", array[i])
            }
            可简化为
            for i, v := range array {
                fmt.Println("Array element [", i, "]=",v) //range有两个返回值
            }
       值类型：
            go数组是一个值类型：即在赋值和传参时会发生拷贝 产生一个副本 (没有c语言中指针的味道)
            所以引入了切片的概念

func main() {
    //array [2][3]int 
    array := {{1,1,1},{1,1,1}}
    for i := 0; i< len(array); i++ {
        fmt.Println("element", i, "of array is", array[i])
    }
}

8数组切片：
    因为数组不是vector那样可以动态扩容
    基于数组创建slice 看例子
    直接创建：
    make()函数
    my_slice := make([]int, 5)  //元素个数为5 初始值为0
    my_slice := make([]int, 5, 10)  //元素个数为5 初始值为0 预留10个空间 len(my_slice)是5 cap(my_slice)是10
    my_slice := []int{1, 2, 3, 4, 5}

    动态增长：
        以空间换时间 先预留出一些空间
        看程序
        append(slice, 1, 2, 3) //常见的添加函数
        append(slice, slice2...) //添加一个slice 注意后面的...
    基于数组切片创建数组切片：
        old_slice := []int{1, 2, 3, 4}
        new_slice := old_slice[:3]
    内容复制：
        slice1 := []int{1, 2, 3, 4, 5}
        slice2 := []int{5, 4, 3}
        copy(slice2, slice1) //拷贝1的前3个元素 
        copy(slice1, slice2)  //同上

9 type关键字几个作用
    定义结构体
    类型重命名
        type name string //name与string等同
    结构体 内嵌匿名成员
        type person struct {
          string  //一个string类型的匿名成员变量
          age  int
        }

    定义函数类型
        type handler func(name string) int //handler是一个函数类型/函数对象


