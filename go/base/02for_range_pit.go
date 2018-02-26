package main
/*
func test() []func() {
    var funs []func()
    for i := 0; i < 2; i++ {
        funs = append(funs, func() {
            println(&i,i)
        })
    }
    return funs
}
*/

func test() []func()  {
    var funs []func()
    for i := 0; i < 2; i++ {
        x := i
        funs = append(funs, func() {
            println(&x,x)
        })
    }
    return funs
}

func main() {
    funs := test()
    for _, f:=range funs{
        f()
    }
}
