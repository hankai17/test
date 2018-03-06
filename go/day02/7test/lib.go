package lib


type ForTest struct {
    num int ;
}
func (this * ForTest) Loops() {
    for i := 0; i < 10000; i++ {
        this.num++
    }
}

//被测函数Add() go test 命令会自动执行所以符合格式 func TestXXX(t *testing.T) 
func Add(x int, y int) (z int) {
    z = x + y
    return
}
