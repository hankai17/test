# .bashrc

# User specific aliases and functions
set encoding=utf-8 
set fileencodings=utf-9,cp936
set ts=4
set expandtab
set autoindent
set smartindent
set shiftwidth=4
set softtabstop=4
#vim tab键4个空格 ggG

alias rm='rm -i'
alias cp='cp -i'
alias mv='mv -i'

# Source global definitions
if [ -f /etc/bashrc ]; then
  . /etc/bashrc
fi

function find_str() {
  key_word=$1
  egrep $1 * -nR --color
}

number=100
for n in $(seq $number); do
  `cp 0.html $n.html`	
done

number=100
for n in $(seq $number); do
  `curl -vx 127.0.0.1:8080 -o /dev/null "http://127.0.0.1:80/$n.html"`
done

function run() {
  number=$1
  shift
  for n in $(seq $number); do
    $@
	#sleep 1
  done
}
PS1='${debian_chroot:+($debian_chroot)}\u@\h:\W\$ '

if [ -e /usr/share/terminfo/x/xterm-256color ]; then
  export TERM='xterm-256color'
else
  export TERM='xterm-color'
fi

export LD_LIBRARY_PATH=/root/file/chilkat-9.5.0-x86_64-linux-gcc/lib:$LD_LIBRARY_PATH

alias example_curl='cd /root/file/curl-7.55.1/docs/examples;pwd'
alias example_libev='cd /root/file/libevent-2.1.8-stable/sample;pwd'

#alias ats_log='cd /opt/ats/var/log/trafficserver;pwd'
#alias ats_db='cd /opt/ats/var/trafficserver;pwd'
#alias ats_bin='cd /opt/ats/bin;pwd'
#alias ats_conf='cd /opt/ats/etc/trafficserver;pwd'
#alias ats_lib='cd /opt/ats/libexec/trafficserver;pwd'
#alias ats_stat='ps auxf | grep traffic | grep -v grep'
#alias ats_ports='netstat -nltp | grep traffic | grep -v grep'
#alias example='cd /root/file/ts/trafficserver-7.1.1/example;pwd'
#alias experience='cd /root/file/ts/trafficserver-7.1.1/plugins/experimental;pwd'
#alias purge='echo "curl -X PURGE -vx 127.0.0.1:80"'
#alias capture_packet='tcpdump -i any -nnvvAs 0 "tcp[((tcp[12:1] & 0xf0) >> 2):4]=0x47455420"'

alias ats_log='cd /opt/ats/var/log/trafficserver;pwd'
alias ats_db='cd /opt/ats/var/trafficserver;pwd'
alias ats_bin='cd /opt/ats/bin;pwd'
alias ats_conf='cd /opt/ats/etc/trafficserver;pwd'
alias ats_lib='cd /opt/ats/libexec/trafficserver;pwd'
alias ats_stat='ps auxf | grep traffic | grep -v grep'
alias example6='cd /root/file/ts/trafficserver-6.2.2/example;pwd'
alias example='cd /root/file/trafficserver-5.3.2/example;pwd'
alias nsq='cd /root/git/my_nsq1/src/github.com/nsqio;ls'
alias spider='cd /root/git/my_nsq1/src/github.com/henrylee2cn/pholcus_lib;ls'
alias ats_jtest='cd /root/file/ts/trafficserver-7.1.1/tools/jtest;pwd'
alias ats_clear_cache='traffic_server -Cclear;'
alias go_src='cd /usr/lib/golang/src;pwd'
alias gortp='cd /root/go_pro/src/github.com/wernerd/GoRTP/src/net;pwd'
alias local_ip='curl http://icanhazip.com'

export GOROOT=/usr/local/go
export GOPATH=/root/go_pro
export PATH=$PATH:$GOPATH:/usr/local/go/bin
#go version

#post 0x504f5354
#find . -executable -exec rm -v {} \;
#tcpdump -i any -nn 'host 本机ip'
#tcpdump -i any -nnA '!port 22' 抓非22
#tcpdump -i any -nnvvAs 0 'tcp[((tcp[12:1] & 0xf0) >> 2):4]=0x47455420' 
#tcpdump -i any -nnvvAs 0 'tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x504f5354'
#tcpdump -i any -nnvvAs 0 'tcp[29]=51 and tcp[30]=48 and tcp[31]=50' #302
#tcpdump -i any -nnvvAs 0 'tcp[((tcp[12:1] & 0xf0) >> 2):4]=0x48545450' 抓取http server请求返回信息，开头为HTTP
#tcpdump -i any -nnvvAs 0 'tcp[((tcp[12:1] & 0xf0) >> 2)+6:4]=0x2e312033' 开头为HTTP/1.1 3...
#tcpdump -i any -nnvvAs 0 'tcp[13] & 0x03 != 0' 抓取TCP连接建立及关闭报文
#tcpdump -i any -nnvvAs 0 tcp[13] == 2 syn链接建立的报文
#tcpdump  -XvvennSs 0 -i any tcp[20:2]=0x4745 or tcp[20:2]=0x4854
#4745 get       4854 ht
#tcpdump -i any -nnvvAs 0 'tcp[((tcp[12:1] & 0xf0) >> 2):4]=0x47455420 or tcp[((tcp[12:1] & 0xf0) >> 2):4]=0x48545450'
#tcpdump -nnAs 0 -i eth0 'udp port 53'
#tcpdump -i any -nn 'icmp'
#tcpdump -i any -nnvvAs 0 'tcp[((tcp[12:1] & 0xf0) >> 2):4]=0x47455420 and dst host not 10.10.110.42 and dst host not 182.92.194.123'
#tcpdump -i any tcp port 9090
#-nnvvAs 0 或者 -nnvvvXX
#tcpdump -i eth4 -nnnAs 0 udp port 53 -w wahaha.pcap   先抓包
#tcpdump -i eth4 -nnnAs 0 udp port 53 -r wahaha.pcap  后看包
#tcpdump -i any -n udp             -t不显示时间戳 -n显示ip
#tcpdump -i any -X -n udp dns包研究
#tcpdump -i any -X -n -s 0 udp -w dns.cap  wireshark打开的抓包 必须要有-s 0 -w file
#tcpdump -i bond0 -nnvAs 0  'dst port 80' | grep 'img.ttigame.com' -C5
#tcpdump -i any -w dump.pcap
#for i in {a..m} ;do dig @$i.gtld-servers.net search.yahoo.com;done

#tcpdump udp -i any -w udp.cap
#tcpdump -nn -i p2p1 'tcp port 80 or tcp port 9011' -w 104.pcap

#tcpdump -i eth4 -nnnAs 0 tcp port 9011 and ip src 1.1.1.1 -w 1.pcap  tcp先抓包 再用wireshark看
#tcpdump -i bond0 -nnnAs 0 tcp port 9011 and ip src 10.148.5.231 -w 1.pcap
#tcpdump -i bond0 -XnnnAs 0 tcp port 9011 and ip src 10.148.5.231 -w 1.pcap
#tcpdump -i bond0 -Xnns 0 tcp port 9011 and ip src 10.148.5.231 -r 1.pcap|more
#tcpdump -i bond0 -Xnns 0 tcp port 9011 and host 10.148.5.231 -w 1.pcap
#tcpdump -i bond0 -Xnns 0 tcp port 9011 and host 10.148.5.231 -r 1.pcap
#tcpdump -i bond0 -Xnns 0 tcp port 9011  -w 1.pcap
#tcpdump -i bond0 -Xnns 400 tcp port 9011 and host 10.133.9.178  -w 1.pcap
#tcpdump -i bond0 -Xnns 0 tcp port 9011 and host 10.152.15.96  -w 3.pcap
# tcpdump -Xnns 0 -i any port 80 > 1.txt baosaisai
#curl -d "city=dongguan&appkey=8010132dcf54491a4eaa387f4db61774" "https://way.jd.com/he/freeweather"
#tcpdump -i bond0 -nnnAs 0 ip src 10.71.254.6 or 10.85.2.130 or ip dst 10.71.254.6 -w 4.10.pcap
#tcpdump -i any -nnnAs 0 tcp or udp -w 1.pcap
#tcpdump -Xnns 0 -i any port 5005 赛赛看包很溜
#tcpdump port 80 -i lo -Xnns 0 -r 1.cap
#ll /home*/ppcache_dat/07/A0/07E3D78F8ED1E32E2A318DBA8F909A69B79805A0* 
# hexdump -C /home6/ppcache_dat/07/A0/07E3D78F8ED1E32E2A318DBA8F909A69B79805A0.info  
#date -d @1566760483
#date -d "Thu, 15 Aug 2019 08:28:10" +%s
#seq 1 100 | while read i; do dig +short www.baidu.com; done
# ps -eo pid,lstart,etime,cmd | grep ppc
# https://centos.pkgs.org/7/centos-x86_64/tcl-8.5.13-8.el7.x86_64.rpm.html  http://mirror.centos.org/centos/7/os/x86_64/Packages/tcl-8.5.13-8.el7.x86_64.rpm


#grep -w "read" --include=*.{h,cc} -nR --color --exclude-dir={cmd,proxy,iocore}
