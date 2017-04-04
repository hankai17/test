
1变量声明
    var v1 int
    var v2 string
    var v3 [10]int  //数组
    var v4 []int    //数组切片
    var v5 struct {
        f int
    }
    var v6 *int //指针
    var v7 map[string]int //map key为string value是int
    var v8 fun(a int) int

    简洁的写法：
    var {
        v1 int
        v2 string
    }

2变量初始化
    var v1 int = 10
    var v2 = 10 //编译器自动推到v2为int
    v3 := 10  //同上

    var i int
    i := 10 这样写时错误的

3变量赋值
    var v10 int
    v10 = 123

    多重赋值：
        i, j = j, i  //i j 相互交换元素

4匿名变量
    func get_name() ( first_name, last_name, nick_name string ) {
        return "kai", "han", "goodboy"
    }
    若只想获取nick_name:
        _, _, nick_name := get_name()

