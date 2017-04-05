1字面常量 literal
2常量定义
    const Pi float64 = 3.1415926
    const zero = 0.0 //无类型的浮点常量
    const {
        size int64 = 1024
        eof = -1 //无类型的整形常量
    }
    const u, v, float32 = 0, 3 //常量的多重赋值
    const a, b, c = 3, 4, "foo" //无类型整形和字符串常量

    常量的赋值是一个编译期间行为 所以右值不能出现任何在运行才能得出结果的表达式
    eg const home = os.GetEnv("HOME")
    
3预定义常量
    true false iota
    iota是一个可被编译器修改的常量，在每一个const关键字出现时被重置0 在下一次出现const之前
    每出现一次iota 其所代表的数字自动++
    const {
        c0 = iota  //0
        c1 = iota  //1  //c1后面的内容可省略不写
        c2 = iota  //2
    }
    const x = iota //x为0

4枚举
    const {
        Sunday = iota
        Monday
        Tuesday
        Wednesday
        Thursday
        Friday
        Saturday
        numberOfDays //没有导出
    }
    大写字母开头的常量在包外可见 而numberOfDays私有


