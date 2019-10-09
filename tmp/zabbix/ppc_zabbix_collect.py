#!/usr/bin/env python
#coding=utf8

import os, sys
import json
import commands
import datetime
#import dns.resolver
#import pycurl, StringIO
import requests, time, re
import urllib
from collections import namedtuple

class ZabbixMonitor(object):
    def __init__(self, log_on=0, ppc_init_port=80, ua=''):
        self.debug_msg = log_on
        self.ppc_init_port = 80
        self.read_only_test_fname = 'read_only_test.txt'
        self.process_start_time_fname = '/tmp/.process_start.txt'
        self.UserAgent = 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 IntcProb/1.0'
        self.Disk = namedtuple('Disk','major_number minor_number dm_name read_ops reads_merged read_sectors read_ms write_ops writes_merged write_sectors write_ms io_queue time_spent_doing_io weighted_io_time')
        self.probe_url = 'http://www.baidu.com/path/to'
        self.probe_domain = 'www.baidu.com'
        self.switch = {}

    def __del__(self):
        pass #TODO

    def get_disk_info(self, device):
        with open('/proc/diskstats') as f:
            for line in f:
                if line.split()[2] == device:
                    return self.Disk(*line.split())
        raise RuntimeError('device ({0}) not found!'.format(device))

    def disk_msg(self, key): #custom.vfs.dev.write.sectors sda
        if 'custom' in key:
            fields = key.split('[')
            if len(sys.argv) <= 2:
                print '0'
                return '0'
            dev = sys.argv[2]
            items = fields[0].split('.')[-2:]
            disk_item = items[0] + '_' + items[1]

            disk_info = self.get_disk_info(dev)
            disk_dict = disk_info._asdict()
            print disk_dict[disk_item]
            return
        if len(key.split('.')) < 3:
            return
        dev = key.split('.')[1]
        disk_item = key.split('.')[2]
    
        disk_info = self.get_disk_info(dev)
        disk_dict = disk_info._asdict()
        #sys.stdout.write(disk_dict[disk_item])
        print disk_dict[disk_item]

    def disk_read_only(self, key):  #read_only home1 #所有函数具有is语义
        if len(sys.argv) <= 2:
            print '1'
        dev = sys.argv[2]
        dev_path_file = '/' + dev + '/' + self.read_only_test_fname
        try:
            f = open(dev_path_file, 'a+') #必须有home*路径
            f.write(datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + '\n')
            f.close()
            #os.remove(dev_path_file) TODO
            print '0'
        except Exception as e:
    	    print '1'

    @staticmethod
    def get_pid(name):
        cmd = "pidof " + name
        result = commands.getoutput(cmd)
        if result != '':
            return map(int, result.split())
        return ''

    @staticmethod
    def get_pid_start_time(pids):
        #if len(pids) < 4:
        #    return
        pid_start_time = []
        for pid in pids:
            result = commands.getoutput("ps -p " + str(pid) + " -o lstart")
    	    if len(result.split('\n')) > 1:
                pid_start_time.append(result.split('\n')[1])
        return pid_start_time

    @staticmethod
    def get_os_rele():
        f = open('/proc/version')
        for line in f.readlines():
            fields = line.strip().split()
            f.close()
            return fields[2].split('.')[3]
        f.close()
        return ''

    ''' pair ---------------->'''
    def get_ppc_pre_start_time(self):
        result = ''
        pid_pre_start_time = ''
        try:
            f = open(self.process_start_time_fname, 'ra+')
            for line in f.readlines():
                result += line
                pid_pre_start_time = result.split("|")[0:len(result.split("|")) - 1]
                break #only one line
            f.close()
            return pid_pre_start_time
        except Exception as e:
            print e
            return []

    def set_ppc_start_time(self, pid_start_time):
        try:
            os.remove(self.process_start_time_fname)
        except Exception as e:
            pass
        try:
            f = open(self.process_start_time_fname, 'a+')
            for i in pid_start_time:
                content = i + "|"
                f.write(content)
            f.close()
        except Exception as e:
            return
    ''' pair <-------------------'''

    def process_status(self, key):  #status函数具有ok含义
        if len(key.split('.')) < 2:
            return '0'
        process = key.split('.')[1]
        pids = list(ZabbixMonitor.get_pid(process))
        if len(pids) == 0 or len(pids) != 4:
            print '0'
            return
        ppc_start_time = ZabbixMonitor.get_pid_start_time(pids) #list
        pre_start_time = self.get_ppc_pre_start_time() #list
        if self.debug_msg:
            print "pre:", (pre_start_time)
            print "cur:", (ppc_start_time) 
    
        if len(pre_start_time) != len(pids) or len(ppc_start_time) != len(pids):
            print '0'
            self.set_ppc_start_time(ppc_start_time)
            return '0'
         
        self.set_ppc_start_time(ppc_start_time)
        #if len( set(pre_start_time) & set(ppc_start_time) ) == len(pids):
        if set(pre_start_time) == set(ppc_start_time):
            print '1'
            return '1'
        else:
            print '0'  #status changed!
            return '0'


    def check_ip(self, ipAddr):
        compile_ip=re.compile('^(1\d{2}|2[0-4]\d|25[0-5]|[1-9]\d|[1-9])\.(1\d{2}|2[0-4]\d|25[0-5]|[1-9]\d|\d)\.(1\d{2}|2[0-4]\d|25[0-5]|[1-9]\d|\d)\.(1\d{2}|2[0-4]\d|25[0-5]|[1-9]\d|\d)$')
        if compile_ip.match(ipAddr):
            return True 
        else:  
            return False

    '''
    def dns_test(self, domain, return_ips):
        try:
            ips = []
            resolver = dns.resolver.Resolver()
            resolver.timeout = 1
            resolver.lifetime = 1
            A = resolver.query(domain, 'A')
            if return_ips == 1:
                for i in A.response.answer:
                    for j in i.items:
                        if self.check_ip(str(j)):
                            ips.append(str(j))
                return ips
            print '1'
        except Exception,e:
            if return_ips == 1:
                return '0'
            print '0'
            return '0'
    '''

    def dns_test(self, domain, return_ips):
    	#dig www.baidu.com A +time=5 +tries=1  +short
        ips = []
        cmd = 'dig ' + domain + ' A +time=2 +tries=1 +short'
        result = commands.getoutput(cmd)
        fields = result.strip().split()
        if len(fields) > 0:
            for i in fields:
                if self.check_ip(str(i)):
                    ips.append(str(i))
        if return_ips:
           return ips
        if len(ips) == 0:
           print '0'
           return '0'
        else:
           print '1'
           return '1'
  

    def net_test(self, url):
        try:
            protocol, req = urllib.splittype(url)
            domain, path =  urllib.splithost(req)
            #print domain, path
            r = self.dns_test(domain, 1)
            if len(r) > 0 and r != '0':
                http_code = requests.head(url, timeout=0.3).status_code
                if http_code == 200 or http_code == 302:
                    print '1'
                    return '1'
                else:
                    print '0'
                    return '0'
            print '0'
            return '0'
        except Exception as e:
            print '0'
            return '0'
    
    def net_status(self, key):
        if 'dns' in key:
            self.dns_test(self.probe_domain, 0)
            return
        if 'net' in key:
            self.net_test(self.probe_url)
            return

    def service_of_ppc(self, key):
        for i in range(0, 4):
            ppcurl = "http://127.0.0.1:" + str(self.ppc_init_port + i) + "/crossdomain.xml"
            try:
                http_code = requests.head(ppcurl, timeout=0.3).status_code
                if http_code != 200:
                    print '0'
                    return
            except Exception as e:
                print '0'
                return '0'
        print '1'

    def tcp_drop(self, key):
        #cat /proc/net/dev | grep $1|sed 's/:/ /g'|awk '{print $5}'
        if len(key.split('.')) < 3:
            return
        dev = key.split('.')[2]
        f = open('/proc/net/dev', 'r')
        for line in f.readlines():
            if self.debug_msg:
                print line
            fields = line.strip().split()
            if fields[0].split(':')[0] == dev:
                print fields[4]
                f.close()
                return
        print '0'
        f.close()

    def get_firewall_status(self, key):
        release = ZabbixMonitor.get_os_rele()
        if release == "el7":
            results = commands.getoutput("systemctl status firewalld.service")
            if self.debug_msg:
                print results
            if "Active: active (running)" in results:
                print '1'
            else:
                print '0'
            return
    
        elif release == "el6":
            results = commands.getoutput("service iptables status")
            if self.debug_msg:
                print results
            if "Chain" in results:
                print '1'
            else:
                print '0'
            return

    def get_eth_status(self, key):
        print '1'
        return '1'

    def register_module(self):
        self.switch = {
            "di": self.disk_msg,       #disk.sda.reads_merged
            "cu": self.disk_msg,       #custom.vfs.dev.write.sectors sda
            "re": self.disk_read_only, #read_only home1                 #timeout TODO
            "pr": self.process_status, #process.http_ppc
            "ne": self.net_status,     #net.status #net.status.dns
            "se": self.service_of_ppc, #serv.http_ppc
            "tc": self.tcp_drop,       #tcp.drop.eth0
            "fi": self.get_firewall_status, #firewall.status
            "et": self.get_eth_status  #eth.status
        }

#UserParameter=ppc_status[*],/bin/sh /etc/zabbix/ppc_status.py $1
def main():
    key = sys.argv[1]
    try:
        zm = ZabbixMonitor()
        zm.register_module()
        zm.switch[key[0:2]](key)
    except Exception as e:
        pass

#1.0.1: first version
#1.0.2: replace pycurl timeout
#1.0.3: replace dns.resolver to `dig +timeout`

version_msg = '1.0.3'

if __name__ == '__main__':
    help_message = 'Usage: python %s [custom.vfs.dev.write.sectors sda] [read_only home1] [process.http_ppc] [net.status] [net.status.dns] [serv.ppc] [tcp.drop.eth0] [firewall.status]' % sys.argv[0]
    if len(sys.argv) < 2:
        print help_message
    elif len(sys.argv) == 2 and (sys.argv[1] in '--version'):
        print version_msg
    elif len(sys.argv) == 2 and (sys.argv[1] in '--help'):
        print help_message
    else:
        main()

