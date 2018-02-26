package main
import (
	"fmt"
    "runtime"
)

type People struct{}
func (p *People) ShowA() {
	fmt.Println("showA")
	p.ShowB()
}
func (p *People) ShowB() {
	fmt.Println("showB")
}

type Teacher struct {
	People
}
/*
func (t *Teacher) ShowB() {
	fmt.Println("teacher showB")
}
*/

func main1() {
	t := Teacher{}
	t.ShowA()
	t.ShowB() //会把基类覆盖掉
}

func main() {
    fmt.Println(runtime.Version())
}
