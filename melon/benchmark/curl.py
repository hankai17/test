#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import pycurl, StringIO

userAgent = 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 IntcProb/1.0'

def request(url):
    try:
        data = StringIO.StringIO()
        c = pycurl.Curl()
        c.setopt(pycurl.URL, url)
        c.setopt(pycurl.USERAGENT, userAgent)
        c.setopt(pycurl.FOLLOWLOCATION, 1)
        c.setopt(pycurl.WRITEFUNCTION, data.write)
        c.setopt(pycurl.CONNECTTIMEOUT, 3)
        c.setopt(pycurl.TIMEOUT, 300)
        c.perform()
        primary_ip   = c.getinfo(pycurl.PRIMARY_IP)
        http_code    = c.getinfo(pycurl.HTTP_CODE)
        lookup_time  = c.getinfo(pycurl.NAMELOOKUP_TIME)
        connect_time = c.getinfo(pycurl.CONNECT_TIME)
        first_time   = c.getinfo(pycurl.STARTTRANSFER_TIME)
        total_time   = c.getinfo(pycurl.TOTAL_TIME)
        down_size    = c.getinfo(pycurl.SIZE_DOWNLOAD)
        down_speed   = c.getinfo(pycurl.SPEED_DOWNLOAD)
        print "-------------------------------------------------------------------------------------------"
        print "url : %s" % url
        print "dstip http_code lookup_time connect_time first_time total_time down_size down_speed"
        print "%s %d %.3f %.3f %.3f %.3f %d %d" % (primary_ip, http_code, lookup_time, connect_time, first_time, total_time, down_size, down_speed)
        data.close()
        c.close()
    except Exception as e:
        print e
        return None

if __name__ == "__main__":
    i = 0
    while 1:
        print i
        request("http://127.0.0.1:8001/1.html")
        i = i + 1
