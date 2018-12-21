package main

import (
	"os"
	"os/exec"
	"fmt"
	"time"
	"net"
	"crypto/tls"
	"net/smtp"
	"strings"
	"flag"
	"bufio"
	"bytes"
)

const (
	mail_user   = "icalert@intcache.com"
	mail_passwd = "i24d_)(*~"

	smtp_server = "smtp.exmail.qq.com:25"
	//smtp_server = "smtp.exmail.qq.com:465"
	//smtp_server = "smtp.exmail.qq.com:587"

	//Alarm_mail  = "xiaoliang.hao@intcache.com;pengfei.shi@intcache.com"
	Alarm_mail  = "icmonitor@intcache.com;xiaoliang.hao@intcache.com"
)

var (
	DefRouteIp = "127.0.0.1"
)

func init() {
	flag.Parse()
}

type Nmail struct {
	smtp_server string
	user        string
	password    string
	to          string
}

func New(smtp_server, user, password, to string) *Nmail {
	return &Nmail{smtp_server: smtp_server, user: user, password: password, to: to}
}

/*
 *  user : example@example.com login smtp server user
 *  password: xxxxx login smtp server password
 *  host: smtp.example.com:port   smtp.163.com:25
 *  to: example@example.com;example1@163.com;example2@sina.com.cn;...
 *  subject:The subject of mail
 *  body: The content of mail
 *  mailtyoe: mail type html or text
 */
func (m *Nmail) SendMail(subject, body, mailtype string) error {
	hp := strings.Split(m.smtp_server, ":")
	auth := smtp.PlainAuth("", m.user, m.password, hp[0])
	var content_type string
	if mailtype == "html" {
		content_type = "Content-Type: text/" + mailtype + "; charset=UTF-8"
	} else {
		content_type = "Content-Type: text/plain" + "; charset=UTF-8"
	}

	user_info := DefRouteIp + "@intcache.com"
	send_to := strings.Split(m.to, ";")
	new_to  := strings.Join(send_to, ", ")
	t       := time.Now()
	Date    := fmt.Sprintf("Date: %v, %v %v %v %d:%d:%d +0800", t.Weekday().String()[:3], t.Day(), t.Month().String()[:3], t.Year(), t.Hour(), t.Minute(), t.Second())
	msg := []byte(Date + "\r\n" + content_type + "\r\nFrom: " + user_info + "<" + user_info + ">\r\nSubject: " + subject + "\r\nTo: " + new_to + "\r\n\r\n" + body)
	//err := smtp.SendMail(m.smtp_server, auth, m.user, send_to, msg)
	err := SendMail(m.smtp_server, auth, m.user, send_to, msg)
	return err
}

func Dial(addr string, host string) (*smtp.Client, error) {
	deadline := time.Now().Add(20 * time.Second)
	d := net.Dialer{Timeout:20 * time.Second, Deadline: deadline}
	conn, err := d.Dial("tcp", addr)
	if err != nil {
		return nil, err
	}
	conn.SetDeadline(deadline)
	return smtp.NewClient(conn, host)
}

func SendMail(addr string, a smtp.Auth, from string, to []string, msg []byte) error {
	host, _, _ := net.SplitHostPort(addr)
	c, err := Dial(addr, host)
	if err != nil {
		return err
	}
	defer c.Close()
	if err = c.Hello("localhost"); err != nil {
		return err
	}
	if ok, _ := c.Extension("STARTTLS"); ok {
		config := &tls.Config{ServerName: host}
		//if testHookStartTLS != nil {
		//	testHookStartTLS(config)
		//}
		if err = c.StartTLS(config); err != nil {
			return err
		}
	}
	if a != nil {
		if ok, _ := c.Extension("AUTH"); ok {
			if err = c.Auth(a); err != nil {
				return err
			}
		}
	}
	if err = c.Mail(from); err != nil {
		return err
	}
	for _, addr := range to {
		if err = c.Rcpt(addr); err != nil {
			return err
		}
	}
	w, err := c.Data()
	if err != nil {
		return err
	}
	_, err = w.Write(msg)
	if err != nil {
		return err
	}
	err = w.Close()
	if err != nil {
		return err
	}
	return c.Quit()
}

func (m *Nmail) MakeBody(content string) (body string){
	body_fmt := `
<html>
	<body>
		<h3>
		%s
		</h3>
	</body>
</html>
    `
	return fmt.Sprintf(body_fmt, content)
}

func NmailTestProc() {
	user := "intcache@163.com"
	password := "wer"
	smtp_server := "smtp.163.com:25"
	to := "xiaoliang.hao@intcache.com"
	nmail := New(smtp_server, user, password, to)

	subject := "链路探测"
	body := nmail.MakeBody("Test send email by golang")

	fmt.Println("send email")
	err := nmail.SendMail(subject, body, "html")
	if err != nil {
		fmt.Println("send mail error!")
		fmt.Println(err)
	} else {
		fmt.Println("send mail success!")
	}

}


func init_mail() *Nmail {
	_nmail := New(smtp_server, mail_user, mail_passwd, Alarm_mail)
	return _nmail
}

func main() {
	if len(flag.Args()) < 1 {
		fmt.Fprintf(os.Stderr, "%s: mail_content is needed\n", os.Args[0])
		os.Exit(-1)
	}
	fmt.Printf("content : %s\n", flag.Args()[0]/*, flag.Args()[2]*/)

	pmail := init_mail()
	ifi   := DefaultRouteDev()
	DefRouteIp = GetIpByDevName(ifi)
	//title := "alarm ICR " + DefRouteIp
	title := "alarm ICR " + getDevName()
	body := fmt.Sprintf("\nHi All:\n\t\n\t%s [%s] %s\n",
						getDevName(), DefRouteIp, flag.Args()[0])
	if err := pmail.SendMail(title, body, "text"); err != nil {
		fmt.Fprintf(os.Stderr, "send mail error - %v\n", err)
	} else {
		fmt.Fprintf(os.Stderr, "send mail success\n")
	}
}

//----------------------------------------------------
func getHostName() string {
	hostname, err := exec.Command("/bin/sh", "-c", "/bin/hostname | /usr/bin/xargs echo -n").CombinedOutput()
	if err != nil || len(hostname) == 0 {
		fmt.Fprintf(os.Stderr, "get hostname failed - %v\n", err)
		os.Exit(-1)
	}
	return string(hostname)

}

func getDevName() string {
	f, e := os.Open("/etc/intc/id")
	if e != nil {
		return getHostName()
	}
	defer f.Close()
	r := bufio.NewReader(f)
	line, err := r.ReadString('\n')
	if err != nil {
		return getHostName()
	}
	line = strings.TrimSuffix(line, "\n")
	line = strings.TrimSuffix(line, "\r")
	return line
}

const (
	ROUTE_FILE        = "/proc/net/route"
	DEFAULT_ROUTE_DEV = "awk '($2 == \"00000000\"){print $1}' /proc/net/route | head -1"
)

func GetIpByDevName(dev string) string {
	//fmt.Printf("[%s]\n", dev)
	iface, err := net.InterfaceByName(dev)
	if err != nil {
		fmt.Fprintf(os.Stderr, "[%s] %v\n", dev, err)
		os.Exit(-1)
	}
	addrs, _ := iface.Addrs()
	if len(addrs) == 0 {
		fmt.Fprintf(os.Stderr, "get %s ip failed\n", dev)
		os.Exit(-1)
	}
	ip, _, e := net.ParseCIDR(addrs[0].String())
	if e != nil {
		fmt.Fprintf(os.Stderr, "%v\n", err)
		os.Exit(-1)
	}
	return fmt.Sprintf("%s", ip)
}

func DefaultRouteDev() string {
	DevName, e := exec.Command("/bin/sh", "-c", DEFAULT_ROUTE_DEV).CombinedOutput()
	DevName = bytes.TrimSuffix(DevName, []byte("\r"))
	DevName = bytes.TrimSuffix(DevName, []byte("\n"))
	if e != nil || len(DevName) == 0 {
		fmt.Fprintf(os.Stderr, "get default route failed - %v\n", e)
		os.Exit(-1)
	}
	fmt.Printf("DefaultRouteDev : %s\n", DevName)
	return string(DevName)
}
