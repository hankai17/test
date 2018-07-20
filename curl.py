import sys
import pycurl, StringIO

#!/usr/bin/python
# -*- coding: utf-8 -*-
userAgent = 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 IntcProb/1.0'

class Test:
    def __init__(self):
        self.contents = ''
    def body_callback(self,buf):
        #self.contents = ''
        self.contents = self.contents + buf
def header(buf):
    #print buf
    http_header = buf.strip().split('\r\n')
    time_header = http_header[0].split(':')[0]
    if  time_header == 'Age' or time_header == 'Expires' or time_header == 'Last-Modified' or time_header == 'Cache-Control':
    	print http_header[0]


def get_ip(ip):
    url = "http://115.29.160.122:8898/cgi-bin/intcip/search.py?ip=" + ip
    t = Test()
    c = pycurl.Curl()
    c.setopt(pycurl.WRITEFUNCTION,t.body_callback)
    c.setopt(pycurl.FOLLOWLOCATION, 1)
    c.setopt(pycurl.URL,url)
    c.setopt(pycurl.SSL_VERIFYPEER, 0)

    try:
        c.perform()
    except:
        pass
    http_code = c.getinfo(pycurl.HTTP_CODE)
    if http_code != 200:
	return ""
    return t.contents.strip()

def request(url, _dns):
    try:
        data = StringIO.StringIO()
        c = pycurl.Curl()
        c.setopt(pycurl.URL, url)
        if _dns:
            print "set assign dns servers %s" % _dns
            c.setopt(pycurl.DNS_SERVERS, _dns)
        c.setopt(pycurl.USERAGENT, userAgent)
	c.setopt(pycurl.SSL_VERIFYPEER, 0)
        c.setopt(pycurl.FOLLOWLOCATION, 1)
        c.setopt(pycurl.WRITEFUNCTION, data.write)
        c.setopt(pycurl.CONNECTTIMEOUT, 9)
        c.setopt(pycurl.TIMEOUT, 300)
        c.setopt(pycurl.IPRESOLVE, pycurl.IPRESOLVE_V4) #IPRESOLVE_V6, IPRESOLVE_WHATEVER
	c.setopt(pycurl.HEADERFUNCTION, header) #header  print response header
        c.perform()
        primary_ip   = c.getinfo(pycurl.PRIMARY_IP)
        http_code    = c.getinfo(pycurl.HTTP_CODE)
        lookup_time  = c.getinfo(pycurl.NAMELOOKUP_TIME)
        connect_time = c.getinfo(pycurl.CONNECT_TIME)
        connect_time = connect_time - lookup_time
        first_time   = c.getinfo(pycurl.STARTTRANSFER_TIME)
        first_time   = first_time - lookup_time
        total_time   = c.getinfo(pycurl.TOTAL_TIME)
        total_time   = total_time - lookup_time
        down_size    = c.getinfo(pycurl.SIZE_DOWNLOAD)
        down_speed   = c.getinfo(pycurl.SPEED_DOWNLOAD)
        ip_result = get_ip(primary_ip)
        #if ip_result == '111.194.248.195':
        #print "-------------------------------------------------------------------------------------------"
        print "url : %s" % url
        print "http_code lookup_time connect_time first_time total_time down_size down_speed desip result"
        print "%d %.3f %.3f %.3f %.3f %d %d %s %s" % (http_code, lookup_time, connect_time, first_time, total_time, down_size, down_speed, primary_ip, ip_result)
        print "-------------------"
        data.close()
        c.close()
    except Exception as e:
        print e
        return None

def prob(urls, _dns):
    for url in urls:
        request(url, _dns)

def get_prob_url(urllist_file):
    urls = []
    #urls.append(urllist_file)
    #return urls
    try:
        f = open(urllist_file, "r")
        for line in f.readlines():
            line = line.strip('\r\n')
            #print(line)
            urls.append(line)
        f.close()
        return urls
    except Exception as e:
        print e
        sys.exit(1)

def usage():
    print("%s [url list file] [dns servers]" % sys.argv[0])
    print
    print('  url list file       set probe url list. one url on one line.')
    print('  dns server          set request dns server, like "192.168.1.100:53,192.168.1.101".')
    sys.exit(1)

if __name__ == "__main__":
    argv_len = len(sys.argv)
    if argv_len <= 1 :
        usage()
    print "down_prob %s" % sys.argv[1]
    urls = get_prob_url(sys.argv[1])
    _dns = None
    if argv_len == 3:
        _dns = sys.argv[2]
    prob(urls, _dns)
                           





                                                             








