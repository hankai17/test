#! /usr/bin/python
# coding=utf8
import os, sys
from urlparse import *

filename = sys.argv[1]

fd = open(filename, 'r')

logs = {}

#test_str = '[Jul 12 12:38:41.109] Server {0x2acb05b35700} ERROR: [22803] Slow Request: client_ip: 10.191.16.99:56501 protocol: -1 url: http://10.129.0.136:82/2Q2W77B14C68BD7C6D599EEBFB7181B73265BE2BD1B4_unknown_553485432D8793ADE62D849D69D98EB0D6A13BF6_5/36.156.4.25/moviets.tc.qq.com/ABP7Peg2ZLD-2t4blW7jEmoo_seJKye4wqM_cfxvwXyw/uwMROfz2r5zBIaQXGdGnC2df644I_EXHvRSGzMhPko-PDo2g/TKio0pAq8eh9 status: 206 unique id:  redirection_tries: 0 bytes: 1023284 fd: 0 client state: 0 server state: 9 ua_begin: 0.000 ua_first_read: 0.000 ua_read_header_done: 0.000 cache_open_read_begin: 0.000 cache_open_read_end: 0.000 dns_lookup_begin: 0.000 dns_lookup_end: 0.000 server_connect: 0.000 server_connect_end: 0.000 server_first_read: 0.009 server_read_header_done: 0.009 server_close: 20.885 ua_write: 0.009 ua_close: 20.885 sm_finish: 20.885 plugin_active: 0.000 plugin_total: 0.000'

def get_suffix(p):
    if len(p) == 1:
        #return "pure domain"
        return "nil"
    fields = p.split(".")
    if len(fields) == 0 or len(fields) == 1:
        #return "no sfx"
        return "null"
    #return fields[len(fields) - 1]
    if fields[len(fields) - 1].find('/') != -1:
        return "null"
    return fields[len(fields) - 1]

def get_suffix1(p):
    if len(p) == 1:
        #return "pure domain"
        return "nil"
    fields = p.split("/")
    if len(fields) == 0 or len(fields) == 1:
        return "null"
    fields1 = fields[len(fields) - 1].split(".")
    if len(fields1) == 0 or len(fields1) == 1:
        return "null"
    else:
        return fields1[len(fields1) - 1]

def get_key_value(line, key):
    fields = line.strip().split(" ")
    for i, str in enumerate(fields):
        #print i, str
        if str == key + ':':
            return fields[i + 1]

for line in fd.readlines():
    fields = line.strip().split(" ")
    if len(fields) != 66:
        continue
    cache_begin = float(get_key_value(line, 'cache_open_read_begin'))
    cache_end = float(get_key_value(line, 'cache_open_read_end'))
    connected_os_end = float(get_key_value(line, 'server_connect_end'))
    file_size = int(get_key_value(line, 'bytes'))

    #print float(cache_begin), float(cache_end)
    if connected_os_end  < 0:
        print cache_end, file_size
