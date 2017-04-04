package main //属于main包 包是go语言的基本分发单位 是工程管理中依赖关系的体现

import "fmt" // 我们需要 fmt 包中的 Println 函数  //import用于导入所依赖的包

func main() {
    fmt.Println("Hello,你坏!")
}
