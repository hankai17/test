package musics
import "fmt"

type Player interface {  //接口肯定精简 是最通用的 所以传递名字即可
    Play(source string)
}
	 //名字   类型
func Play(source, mtype string) {  //这只是一个函数只是名字是Play 不要把重点放到这 
    var p Player

    switch mtype { //注意是大写
    case "MP3":
        p = &mp3_player{} //还是那句话 一个结构体实现了接口的函数(Play) 就可以把它(结构体)赋值给接口
    case "WAV":
        p = &wav_player{}
    default:
        fmt.Println("Unsupported music type", mtype)
        return
    }
    p.Play(source)
}
