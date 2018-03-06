package lib
import "testing"

type AddArray struct {
    result  int;
    add_1   int;
    add_2   int;
}

//符合格式 func BenchmarkXxx(b *testing.B)被认为是一个性能测试程序， 
//go test -bench=. （ 参数必须有！）来执行*go test命令的时候性能测试程序就会被顺序执行。
func BenchmarkLoops(b *testing.B) {
    var test ForTest
    ptr := &test
    //必须循环b.N次 b.N会在运行中调整，以便最终达到合适的时间消耗。方便计算出合理的数据。 （ 免得数据全部是 0 ） 
    for i:=0 ; i<b.N ; i++ {
        ptr.Loops()
    }
}

// 测试并发效率
func BenchmarkLoopsParallel(b *testing.B) {
    b.RunParallel( func(pb *testing.PB) {
        var test ForTest
        ptr := &test
        for pb.Next() {
            ptr.Loops()
        }
    })
}

func TestAdd(t *testing.T) {
    var test_data = [3]AddArray {
        { 2, 1, 1},
        { 5, 2, 3},
        { 4, 2, 2},
    }

    for _ , v := range test_data {
        if v.result != Add(v.add_1, v.add_2) {
            t.Errorf("Add( %d , %d ) != %d \n", v.add_1 , v.add_2 , v.result);
        }
    }
}
/*
// 表示测试全部通过
>PASS                       
// Benchmark 名字 - CPU            循环次数             平均每次执行时间 
BenchmarkLoops-2                  100000             20628 ns/op     
BenchmarkLoopsParallel-2          100000             10412 ns/op   
//      哪个目录下执行go test         累计耗时
ok      swap/lib                   2.279s
*/
