package musics

import (
    "fmt"
    "time"
)

type mp3_player struct {
    stat int
    progress int
}

func (p *mp3_player)Play(source string) {
    fmt.Println("Playing MP3 music", source)
    p.progress = 0

    for p.progress < 100 {
        time.Sleep(100 * time.Millisecond) // 假装正在播放
        fmt.Print(".")
        p.progress += 10
    }
    fmt.Println("\nFinished playing", source)
}

