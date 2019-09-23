#!/usr/bin/env python
#coding=utf8

import os, sys
import json
import commands
import datetime
import dns.resolver
import pycurl, StringIO
from collections import namedtuple
from subprocess import check_output

Disk = namedtuple('Disk','major_number minor_number dm_name reads_completed reads_merged reads_sectors time_spent_reading writes_completed writes_merged writes_sectors time_spent_writing io_queue time_spent_doing_io weighted_io_time')
userAgent = 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 IntcProb/1.0'

def get_os_release(key):
    f = open('/proc/version')
    for line in f.readlines():
        fields = line.strip().split()
        f.close()
        return fields[2].split('.')[3]
    f.close()

def get_firewall_status(key):
    release = get_os_release('') 
    if release == "el7":
        results = check_output(["systemctl", "status", "firewalld.service"])
        if "Active: active (running)" in results:
            print '1'
        else:
            print '0'
        return

    elif release == "el6":
        results = check_output(["service", "iptables", "statuse"])
        if "Chain" in results:
            print '1'
        else:
            print '0'
        return


def tcp_drop(key):
    #cat /proc/net/dev | grep $1|sed 's/:/ /g'|awk '{print $4}'
    if len(key.split('.')) < 3:
        return
    dev = key.split('.')[1]
    f = open('/proc/net/dev', 'r')
    for line in f.readlines():
        fields = line.strip().split()
        if fields[0].split(':')[0] == dev:
            print fields[4]
            f.close()
            return
    f.close()    
    print '0'

def request(url, _dns, timeout=2):
    try:
        data = StringIO.StringIO()
        c = pycurl.Curl()
        c.setopt(pycurl.URL, url)
        #if _dns:
        #    c.setopt(pycurl.DNS_SERVERS, _dns)
        c.setopt(pycurl.USERAGENT, userAgent)
        c.setopt(pycurl.FOLLOWLOCATION, 1)
        c.setopt(pycurl.WRITEFUNCTION, data.write)
        c.setopt(pycurl.CONNECTTIMEOUT, 2)
        c.setopt(pycurl.TIMEOUT, int(timeout))
        c.perform()
        primary_ip   = c.getinfo(pycurl.PRIMARY_IP)
        if _dns != '':
            print '1'
            return
        http_code    = c.getinfo(pycurl.HTTP_CODE)
        lookup_time  = c.getinfo(pycurl.NAMELOOKUP_TIME)
        connect_time = c.getinfo(pycurl.CONNECT_TIME)
        first_time   = c.getinfo(pycurl.STARTTRANSFER_TIME)
        total_time   = c.getinfo(pycurl.TOTAL_TIME)
        down_size    = c.getinfo(pycurl.SIZE_DOWNLOAD)
        down_speed   = c.getinfo(pycurl.SPEED_DOWNLOAD)
        data.close()
        c.close()
        if http_code == 200 or http_code == 304:
            return 1
        else:
            return 0
    except Exception as e:
        #print e
        return 0

def service_of_ppc(key):
    for i in range(0, 4):
        url = "127.0.0.1:" + str(80 + i) + "/crossdomain.xml"
        if request(url, '') == 0:
            print '0'
            return
    print '1'

def net_status(key):
    if 'dns' in key:
        print request("www.baidu.com", 'dns')
    if 'net' in key:
        print request("www.baidu.com", '')

def get_resolved_iplist(key):
    ip_list = []
    try:
        A = dns.resolver.query("www.baidu.com", 'A')
    except Exception,e:
        print 0
        return
    for i in A.response.answer:
        for j in i.items:
            ip_list.append(j)
    #return ip_list
    if i > 0:
        print 1
    else:
        print 0

def get_disk_info(device):
    with open('/proc/diskstats') as f:
        for line in f:
            if line.split()[2] == device:
                return Disk(*line.split())
    raise RuntimeError('device ({0}) not found!'.format(device))

'''
netcards = []
step = os.popen("""cat /proc/net/dev  |grep -Ev '(lo|face|Inter)' | awk '{if($2>0) {split($1,a,":");print a[1]}}'""")

for look in step.readlines():
    netcard = os.path.basename(look.strip())
    netcards += [{'{#IFNAME}':netcard}]

print json.dumps({'data':netcards},sort_keys = True,indent = 4,separators=(',',':'))


$1: eth_port  dns_monitor  net_monitor firewall_monitor disk_io disk_status ppc_monitor tcp_drop
'''

def get_pid(name):
    result = check_output(["pidof", name])
    if result != '':
        return map(int, check_output(["pidof", name]).split())
    return ''

def get_ppc_pre_start_time():
    tmp_file = '/tmp/.ppc_start_time.txt'
    result = ''
    try:
        f = open(tmp_file, 'ra+')
        for line in f.readlines():
            result = result + (line + "|")
        f.close()
        return result
    except Exception as e:
        return ''

def get_ppc_start_time(pids): #ps -p 31146 -o lstart
    #if len(pids) < 4:
    #    return
    #result = check_output(["ps -p"], ) TODO order the ppc[0-3]
    ppc_start_time = []
    for pid in pids:
        result = check_output(["ps", "p", str(pid), "o", "lstart"])
        if len(result.split('\n')) == 3:
            #print result.split('\n')[1] #TODO parse time
            ppc_start_time.append(result.split('\n')[1])
    return ppc_start_time

def set_ppc_start_time(ppc_start_time):
    try:
        os.remove('/tmp/.ppc_start_time.txt')
    except Exception as e:
        pass
    #reset pre time & close
    try:
        f = open('/tmp/.ppc_start_time.txt', 'a+')
        for i in list(ppc_start_time):
            f.write(i)
        f.close()
    except Exception as e:
        print 'open err'
        pass

def disk_msg(key):
    if len(key.split('.')) < 3:
        return
    dev = key.split('.')[1]
    disk_item = key.split('.')[2]

    disk_info = get_disk_info(dev)
    disk_dict = disk_info._asdict()

    #sys.stdout.write(disk_dict[disk_item])
    print disk_dict[disk_item]

def disk_read_only(key): 
    if len(key.split('.')) < 2:
        return
    dev = key.split('.')[1]
    dev_path_file = '/' + dev + '/' + 'read_only_test.txt'
    try:
        f = open(dev_path_file, 'a+')
        f.write(datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + '\n')
        f.close()
        #os.remove(dev_path_file) TODO
        print '0'
    except Exception as e:
        print '1'

def ppc_status(key):
    if len(key.split('.')) < 2:
        return
    #process = key.split('.')[1]
    pids = list(get_pid('nginx'))
    ppc_start_time = set(get_ppc_start_time(pids))
    pre_start_time = get_ppc_pre_start_time()
    ppc_pre_start_time = pre_start_time.split("|")[0:len(pre_start_time.split("|")) - 1]
    print "pre:", ppc_pre_start_time, " | cur:", list(ppc_start_time)
     
    if len( set(ppc_pre_start_time) & ppc_start_time ) == 1:
        print '0'
    else:
        print '1'  #status changed!
    set_ppc_start_time(list(ppc_start_time))

switch = {
    "di": disk_msg,       #disk.sda.reads_merged
    "re": disk_read_only, #read_only.home1
    "pp": ppc_status,     #process.ppc1
    "dn": get_resolved_iplist, #dns.status
    "ne": net_status,     #net.status
    "se": service_of_ppc, #serv.ppc
    "tc": tcp_drop,       #tcp.eth0.drop
    "os": get_os_release, #os.release
    "fi": get_firewall_status #firewall.status
}

#UserParameter=ppc_status[*],/bin/sh /etc/zabbix/ppc_status.py $1
def main():
    key = sys.argv[1]
    try:
        switch[key[0:2]](key)
    except KeyError as e:
        pass

if __name__ == '__main__':
    if len(sys.argv) > 1:
        main()
