#!/usr/bin/env python

import os
import sys
import socket
import struct

pps_ip = "10.0.110.82"
pps_port = 7545

def udp_send_message(ip, port, arr):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    #for data in arr:
    s.sendto(arr, (ip, port))
    s.close()
        


data = struct.pack(
    '=HHHH200s',
    80, #id
    1, #type
    200 + 8, #length
    len('www.ifeng.com'), #domain_len
    'www.ifeng.com' #domain
)
#print "data send to pps len: ", len(data)
udp_send_message(pps_ip, pps_port, data)
