# .bashrc

# User specific aliases and functions

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
