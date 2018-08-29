package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
	"music_lib"
	"musics"
)

var lib *music_lib.Music_manager  //声明 mlib包的MusicManager对象
var id int = 1
var ctrl, signal chan int

func handle_lib_commands(tokens []string) {
	switch tokens[1] {
	case "list":
		for i := 0; i < lib.Len(); i++ {    //lib list
			e, _ := lib.Get(i)
			fmt.Println(i+1, ":", e.Name, e.Artist, e.Source, e.Type)
		}
	case "add":        //lib add xxxxxx
		{
			if len(tokens) == 6 {
				id++ //全局变量 记录个数
				lib.Add(&music_lib.Music_entry{strconv.Itoa(id), tokens[2], tokens[3], tokens[4], tokens[5]})
			} else {
				fmt.Println("USAGE: lib add <name> <artist> <source> <type>")
			}
		}
	case "remove": //lib remove x
		if len(tokens) == 3 {
			lib.RemoveByName(tokens[2])
		} else {
			fmt.Println("USAGE: lib remove <name>")
		}

	default:
		fmt.Println("Unrecognized lib command:", tokens[1])
	}
}

func handle_play_command(tokens []string) {
	if len(tokens) != 2 {
		fmt.Println("USAGE: play <name>")
		return
	}

	e := lib.Find(tokens[1])  //返回的是音乐节点 
	if e == nil {
		fmt.Println("The music", tokens[1], "does not exist.")
		return
	}

	musics.Play(e.Source, e.Type)//, ctrl, signal)               //这个play名字起的不好                     //mp包中的Play函数里面有 接口的 赋值行为 , 这里是入口
}

func main() {
	fmt.Println(`
Enter following commands to control the player:
lib list -- View the existing music lib
lib add <name> <artist> <source> <type> -- Add a music to the music lib
lib remove <name> -- Remove the specified music from the lib
play <name> -- Play the specified music
`)
	lib = music_lib.New_music_manager()  //定义一个对象
	r := bufio.NewReader(os.Stdin)

	for {
		fmt.Print("Enter command-> ")
		raw_line, _, _ := r.ReadLine()
		line := string(raw_line) //把输入命令 转 为字符串(因为string类型有很多方法)
		if line == "q" || line == "e" {
			break
		}
		tokens := strings.Split(line, " ")  //切片？
		if tokens[0] == "lib" {     //lib xxx             
			handle_lib_commands(tokens)
		} else if tokens[0] == "play" {     //play butterfly
			handle_play_command(tokens)
		} else {
			fmt.Println("Unrecognized command:", tokens[0])
		}
	}
}
