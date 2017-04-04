package mp

import "fmt"

type Player interface {
    Play(source string)
}
	 //名字   类型
func Play(source, mtype string) {  //这只是一个函数接口 不要把重点放到这 
    
    var p Player

    switch mtype {
    case "MP3":
        p = &MP3Player{} //还是那句话 一个结构体实现了接口的函数 就可以把它赋值给接口
    case "WAV":
        p = &WAVPlayer{}
    default:
        fmt.Println("Unsupported music type", mtype)
        return
    }
    
    p.Play(source)
}
