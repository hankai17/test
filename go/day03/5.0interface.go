package main
import (
  //"strings"
  "fmt"
)

type Stringer interface {
    String() string
}
 
type S struct {
     i int
}
func (s *S) String() string {
    return fmt.Sprintf("%d", s.i)
}
 
func Print(s Stringer) {
    println(s.String())
}
 
func DynamicPrint(any interface{}) {
   if s, ok := any.(Stringer); ok {
       Print(s)
   }
}
 
func main() {
   var s S
   s.i = 123456789
   Print(&s)
   DynamicPrint(&s)
}
