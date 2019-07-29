#!/usr/bin/env python

import os
import sys
import socket
import struct
import ConfigParser
#import numpy as np

class cConfParser:
    def __init__(self, conf_path, overwrite=0):
        self.fpath = conf_path
        self.overwrite = overwrite
        self.cf = ConfigParser.ConfigParser()
        self.cf.read(self.fpath)

    def __del__(self):
        if self.overwrite == 1:
            with open(self.fpath, 'w') as fh:
                self.cf.write(fh)

    def make_product(self, val):
        if '*' not in val:
            return int(val)
        else:
            seg = val.split('*')
            product = 1
            for i in xrange(len(seg)):
                product = product * int(seg[i])
            return product

    def add_section(self, s):
        if not self.cf.has_section(s):
            self.cf.add_section(s)

    def remove_section(self, s):
        return self.cf.remove_section(s)

    def get(self, s, o):
        return self.cf.get(s, o)

    def getint(self, s, o):
        return self.cf.getint(s, o)

    def set(self, s, o, v):
        if self.cf.has_section(s):
           self.cf.set(s, o, v)

    def remove_option(self, s, o):
        if self.cf.has_section(s):
            return self.cf.remove_option(s, o)
        return False

    def items(self, s):
        return self.cf.items(s)

    def sections(self):
        return self.cf.sections()

    def options(self, s):
        return self.cf.options(s)


pps_ip_list = []
pps_port = 0
domain_file = ""
recover_del_port = 82

def udp_send_message(ip_list, port, arr):
    for ip in ip_list:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.sendto(arr, (ip, port))
        s.close()

def format_data(url, sfx):
    fmt = "=HHHH%dsH%dsH" %(len(url),len(sfx))
    data = struct.pack(
        fmt,
        recover_del_port,      #id
        1,       #type
        8 + len(url) + 2 + len(sfx) + 1, #length
        len(url), #domain_len
        url,      #domain
        len(sfx), #sfx_len
        sfx,      #sfx
        0
    )
    return data

def config_parse():
    global pps_ip
    global pps_port
    global domain_file
    cp = cConfParser('test.config')
    pps_ip = cp.get('common', 'pps_ip')
    fields = pps_ip.strip().split('|')

    if len(fields) > 0:
	for i in fields:
	    pps_ip_list.append(i)
    else:
	pps_ip_list.append(pps_ip)

    pps_port = int(cp.get('common', 'pps_port'))
    domain_file = cp.get('common', 'domain_send_list')
    try:
    	if cp.get('common', 'recover_or_del') != '':
	    global recover_del_port
	    recover_del_port = int(cp.get('common', 'recover_or_del'))
    except Exception as e:
	pass
	
if len(sys.argv)  != 1:
    print 'Usage: python %s' % sys.argv[0]
    print '    eg: python send_recover_pps.py'
    exit(1)
else:
    config_parse()


dic = {}
fd = open(domain_file, 'r')
for line in fd.readlines():
    fields = line.strip().split(' ')
    if len(fields) < 2:
	continue

    domain = fields[0];
    sfx_list = []
    for i, val in enumerate(fields):
	if i == 0:
	    continue
	if val != '':
	    sfx_list.append(val)
    #print sfx_list
    if not dic.has_key(domain):
	dic[domain] = sfx_list
    else:
	for i in sfx_list:
	    dic[domain].append(i)

for k in dic.keys():
    #print k, dic[k]
    #uniq_list = np.unique(dic[k])
    uniq_list = list(set(dic[k]))
    print "uniq_list=", uniq_list
    for i in uniq_list:
	data = format_data(k, i)
	udp_send_message(pps_ip_list, pps_port, data)

print 'send to %s:%d ok' %(pps_ip, pps_port)

'''
fmt = "HHHH%dsH" %(len(job.url))
data = struct.pack(
    fmt,
    80,      #id
    1,       #type
    8 + len(job.url) + 1, #length
    len(job.url), #domain_len
    job.url,
    0
)
'''
