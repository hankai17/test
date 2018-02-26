package main
const cl  = 100
var bl    = 123

func main()  {
    println(&bl,bl)
    //println(&cl,cl) 常亮在编译期生成 作为指令使用没有地址
}
