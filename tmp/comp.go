package main
import (
        "fmt"
        "os"
        "strings"
        "bufio"
       )

func comp_host(m map[string]int) {
    fd, err := os.Open("./1.txt")  //看看哪个数据不在大数据库中
        if err != nil {
            panic(err)
        }

    defer func() {
        if err := fd.Close(); err != nil {
            panic(err)
        }
    }()

    scanner := bufio.NewScanner(fd)
    for scanner.Scan() {
        fields := strings.Fields( strings.Trim( strings.TrimSpace(scanner.Text()) ,"\n"))
        if len(fields) == 0 {
            continue
        }
        domain := fields[0]
        //fmt.Println(domain)
        _, ok := m[domain]
        if (!ok) {
            fmt.Println(domain, " nohost")
        } else {
            fmt.Println(domain, " yeshost")
        }
    }
}

func main() {
    fd, err := os.Open("./alldomain.txt")   //建立最大的那个数据库
        if err != nil {
            panic(err)
        }

    defer func() {
        if err := fd.Close(); err != nil {
            panic(err)
        }
    }()

    var host_map map[string]int
    host_map = make(map[string]int)

    scanner := bufio.NewScanner(fd)
    for scanner.Scan() {
        fields := strings.Fields( strings.Trim( strings.TrimSpace(scanner.Text()) ,"\n"))
        if len(fields) == 0 {
            continue
        }
        domain := fields[0]
        //fmt.Println(domain)

        count, ok := host_map[domain]
        if (ok) {
            host_map[domain] = count + 1
        } else {
            host_map[domain] = 0
        }
    }
    comp_host(host_map)
}
