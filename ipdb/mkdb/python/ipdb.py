#!/usr/bin/python
# coding: UTF-8
import os
import re
import sys
import struct
import socket

location_isp_match = re.compile("(.*)\s(.*)")

def usage():
    print "usage as following:"
    print "\tpython ipdb.py ip"
    print "\tfor example:"
    print "\t\tpython ipdb.py 1.1.1.1"

def getLong(b,offset):
    if len( b[offset:offset+4] ) == 4:
        return struct.unpack('I', b[offset:offset+4])[0]

def search(query_ip):
    f = open("ip.db","rb")
    file_size = os.path.getsize("ip.db")

    f.seek(file_size-8)
    first_block = getLong(f.read(4),0)
    #f.seek(file_size-4) #多余
    last_block = getLong(f.read(4),0)
    block_num = (last_block-first_block)/12 + 1;

    l=0
    h=block_num
    while l <= h:
        m = ((l+h) >> 1)
        p = m*12 + first_block 

        f.seek(p)
        sip = getLong(f.read(4),0)
        if query_ip < sip:
            h = m -1
        else:
            f.seek(p+4)
            eip = getLong(f.read(4),0)
            if query_ip > eip:
                l = m + 1
            else:
                data_ptr = getLong(f.read(4),0)
                break
    if l > h:
        print "not found"
        return
    detail_len = ((data_ptr >> 24) & 0xFF)
    detail_pos = (data_ptr & 0x00FFFFFF)
    f.seek(detail_pos)
    result = f.read(detail_len)
    result_match = location_isp_match.match(result)
    location = result_match.group(1)
    isp = result_match.group(2)
    print "location:%s\tisp:%s\t"%(location,isp)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        usage()
    else:
        num_ip=socket.ntohl(struct.unpack("I",socket.inet_aton(str(sys.argv[1])))[0])
        search(num_ip)

