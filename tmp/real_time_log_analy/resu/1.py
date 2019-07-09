#!/usr/bin/env python

import os
import sys
import socket
import struct

pps_ip = "10.0.110.82"
pps_port = 7111

def udp_send_message(ip, port, arr):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    #for data in arr:
    s.sendto(arr, (ip, port))
    s.close()
        

data = struct.pack(
    '=HHH%ds'%
    80, #id
    1, #type
    200 + 8, #length
    len('www.ifeng.com'), #domain_len
    'www.ifeng.com' #domain
)

'''
data1 = struct.pack(
    '=HHHH%ds%ds'%
    80, #id
    1, #type
    200 + 8, #length
    len('www.ifeng.com'), #domain_len
    'www.ifeng.com', #domain
    len('exe'),
    'exe'
)
'''

data2 = struct.pack(
    'HHHH%ds'%
    0,
    1,
    2,
    len('www.baidu.com'),
    len('www.baidu.com'), 'www.baidu.com'
)

data3 = struct.pack(
    'H13s',
    4,
    'www.baidu.com'
)

s = 'www.baidu.com'
data = struct.pack("H%ds" % 
        (len(s),), 
        len(s), 
        s
)
print data

fmt = "HHHH%dsH%ds" %(len('www.baidu.com'),len('exe'))
data4 = struct.pack(
    fmt,
    0,
    1,
    2,
    4,
    'www.baidu.com',
    5,
    'exe'
)

#print "data send to pps len: ", len(data)
udp_send_message(pps_ip, pps_port, data4)
