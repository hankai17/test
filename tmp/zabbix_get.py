#!/usr/bin/env python
#coding=utf8

import os, sys
import json
import commands
import datetime
#import dns.resolver
import pycurl, StringIO
from collections import namedtuple

class ZabbixMonitor(object):
    def __init__(self, log_on=0, ppc_init_port=80, ua=''):
        self.debug_msg = log_on
        self.ppc_init_port = 80
        self.read_only_test_fname = 'read_only_test.txt'
        self.process_start_time_fname = '/tmp/.process_start.txt'
        self.UserAgent = 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 IntcProb/1.0'
        self.Disk = namedtuple('Disk','major_number minor_number dm_name reads_completed reads_merged reads_sectors time_spent_reading writes_completed writes_merged writes_sectors time_spent_writing io_queue time_spent_doing_io weighted_io_time')
        self.url = 'www.baidu.com'
        self.switch = {}

    def __del__(self):
        pass #TODO

    def get_disk_info(self, device):
        with open('/proc/diskstats') as f:
            for line in f:
                if line.split()[2] == device:
                    return self.Disk(*line.split())
        raise RuntimeError('device ({0}) not found!'.format(device))

    def disk_msg(self, key):
        if len(key.split('.')) < 3:
            return
        dev = key.split('.')[1]
        disk_item = key.split('.')[2]
    
        disk_info = self.get_disk_info(dev)
        disk_dict = disk_info._asdict()
        #sys.stdout.write(disk_dict[disk_item])
        print disk_dict[disk_item]

    def disk_read_only(self, key):  #所有函数具有is语义
        if len(key.split('.')) < 2:
            return '1'
        dev = key.split('.')[1]
        dev_path_file = '/' + dev + '/' + self.read_only_test_fname
        try:
            f = open(dev_path_file, 'a+')
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
        if len( set(pre_start_time) & set(ppc_start_time) ) == len(pids):
            print '1'
            return '1'
        else:
            print '0'  #status changed!
            return '0'

    def request(self, url, _dns, timeout = 2):
        try:
            data = StringIO.StringIO()
            c = pycurl.Curl()
            c.setopt(pycurl.URL, url)
            c.setopt(pycurl.USERAGENT, self.UserAgent)
            c.setopt(pycurl.FOLLOWLOCATION, 1)
            c.setopt(pycurl.WRITEFUNCTION, data.write)
            c.setopt(pycurl.CONNECTTIMEOUT, 2)
            c.setopt(pycurl.TIMEOUT, int(timeout))
            c.perform()
            primary_ip   = c.getinfo(pycurl.PRIMARY_IP)
            if _dns != '':
                if self.debug_msg:
                    print primary_ip
                data.close()
                c.close()
                return '1'
            http_code    = c.getinfo(pycurl.HTTP_CODE)
            lookup_time  = c.getinfo(pycurl.NAMELOOKUP_TIME)
            connect_time = c.getinfo(pycurl.CONNECT_TIME)
            first_time   = c.getinfo(pycurl.STARTTRANSFER_TIME)
            total_time   = c.getinfo(pycurl.TOTAL_TIME)
            down_size    = c.getinfo(pycurl.SIZE_DOWNLOAD)
            down_speed   = c.getinfo(pycurl.SPEED_DOWNLOAD)
            data.close()
            c.close()
            if self.debug_msg:
                print primary_ip, http_code
            if http_code == 200 or http_code == 304:
                return '1'
            else:
                return '0'
        except Exception as e:
            print e
            return '0'
    
    def net_status(self, key):
        if 'dns' in key:
            print self.request(self.url, 'dns')
            return
        if 'net' in key:
            print self.request("www.baidu.com", '')
            return

    def service_of_ppc(self, key):
        for i in range(0, 4):
            ppcurl = "127.0.0.1:" + str(self.ppc_init_port + i) + "/crossdomain.xml"
            if self.request(ppcurl, '') == '0':
                print '0'
                return
        print '1'

    def tcp_drop(self, key):
        #cat /proc/net/dev | grep $1|sed 's/:/ /g'|awk '{print $4}'
        if len(key.split('.')) < 3:
            return
        dev = key.split('.')[1]
        f = open('/proc/net/dev', 'r')
        for line in f.readlines():
            if self.debug_msg:
                print line
            fields = line.strip().split()
            if fields[0].split(':')[0] == dev:
                print fields[3]
                f.close()
                return
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

    def register_module(self):
        self.switch = {
            "di": self.disk_msg,       #disk.sda.reads_merged
            "re": self.disk_read_only, #read_only.home1    #timeout TODO
            "pr": self.process_status, #process.http_ppc
            "ne": self.net_status,     #net.status #net.status.dns
            "se": self.service_of_ppc, #serv.ppc
            "tc": self.tcp_drop,       #tcp.eth0.drop
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
    except KeyError as e:
        pass

if __name__ == '__main__':
    if len(sys.argv) > 1:
        main()
