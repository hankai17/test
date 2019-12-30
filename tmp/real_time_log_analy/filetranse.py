#!/usr/bin/env python                 
# -*- coding:utf-8 -*-
import os
import re
import string
import struct
import sys
import getopt
import datetime
from urlparse import *
#from mongoop import MyMongoDB
from config import *

# encoding=utf8 
#import sys 
reload(sys) 
sys.setdefaultencoding('utf8')
#sys.path.append()


# GET uri path and len
#refer

class ProtoParser(object):
    def __init__(self):
        self.name = "protoparser"
        self.switch = {}
    
    def __del__(self):
        pass #TODO

    def http_parse(self, method, req_str, req_len, resp_str, resp_len):
        dic = {}
        header = req_str.split("\r\n\r\n")[0]
        if "HTTP" not in header:
            return None
 
        if "HTTP/1" in header: 
            if "POST" in header or "GET" in header:
                tmp = True
            else:
                return None

        linelist = header.split("\r\n")
        reqhead = linelist[0]
        dic["req_head"] = reqhead.decode('utf-8', errors='ignore').encode('utf-8')
        
        reqheadlist  = reqhead.split(" ")
        if len(reqheadlist)  > 1:
            method = reqheadlist[0]
            uri = reqheadlist[1].decode('utf-8', errors='ignore').encode('utf-8')


            dic["method"] = method
            dic["uri"] = uri
            
            urilist = uri.split('?')                                     
            if len(urilist) > 1:
                dic["uri_path"] = urilist[0].strip().decode('utf-8', errors='ignore').encode('utf-8')
                dic["uri_para"] = urilist[1].decode('utf-8', errors='ignore').encode('utf-8')
        
        for elem in linelist:
            if "User-Agent:" in elem:
                dic["User-Agent"] = elem.split("User-Agent:")[1].strip().decode('utf-8', errors='ignore').encode('utf-8')   
            elif "Referer:" in elem:
                dic["Referer"] = elem.split("Referer:")[1].decode('utf-8', errors='ignore').encode('utf-8')

            elif "Host:" in elem:
                dic["host"] = elem.split("Host:")[1].strip().decode('utf-8', errors='ignore').encode('utf-8')
            elif "NETEAST-TYPE:" in elem:
                dic["neteast_type"] = elem.split("NETEAST-TYPE:")[1].strip().decode('utf-8', errors='ignore').encode('utf-8')

        #parse resp
        header = resp_str.split("\r\n\r\n")[0]
        contentlist = header.split("\r\n")
        if len(contentlist) > 2:
            if len(contentlist[0].split(" ")) > 1:
                head = contentlist[0].split(" ")[1]
            else:
                head = ""
            for elem in contentlist:
                if "Content-Range:" in elem:
                    dic["content-range"] = elem.split("Content-Range:")[1]

                elif "Content-Type:" in elem:
                    dic["content-type"] = elem.split("Content-Type:")[1].strip().decode('utf-8', errors='ignore').encode('utf-8')
                
                elif "Content-Length:" in elem:
                    if elem.split("Content-Length:")[1].strip():
                        if elem.split("Content-Length:")[1].strip().isalnum():
                            dic["content-length"] = elem.split("Content-Length:")[1]
                        else:
                            dic["content-length"] = re.findall("\d+",elem.split("Content-Length:")[1])[0]            
                elif "Last-Modified:" in elem:
                    if elem.split("Last-Modified:")[1].strip():
                        dic["last-modified"] = elem.split("Last-Modified:")[1].strip()
                    else:
                        dic["last-modified"] = ""
            if head.isdigit():     
                dic["status_code"] = head
            
        return dic

    def ssh_parse(self, method, req_str, req_len, resp_str, resp_len):
        pass

    def unknown_parse(self, method, req_str, req_len, resp_str, resp_len):
        pass

    def ssl_parse(self, method, req_str, req_len, resp_str, resp_len):
        dic = {}
        if req_len < 4:
            return None
        b = bytearray(req_str)
        if int(b[0]) != 22:
            return None

        ssl_version = struct.unpack('>H', b[1:3])[0]
        ssl_len = struct.unpack('>H', b[3:5])[0]
        #print "ssl_version: ", ssl_version, "ssl_len: ", ssl_len, "req_len: ", req_len
        if ssl_len + 5 != req_len:
            #print 'ssl packet not complete'
            return  None
        
        handshake_type = int(b[5])
        if handshake_type != 1:
            return  None

        handshake_len = struct.unpack('>B', b[6:7])[0] * 256 + struct.unpack('>H', b[7:9])[0]
        handshake_version = struct.unpack('>H', b[9:11])[0]
        #print "handshake_len: ", handshake_len, "handshake_version: ", handshake_version

        session_id_len = struct.unpack('>B', b[11+4+28:11+4+28+1])[0] #GMT(4) + random bytes(28) + sessionlen(1) + len(session)
        #print "session_id_len: ", session_id_len

        handshake_cs_len = struct.unpack('>H', b[11+4+28+1+session_id_len: 11+4+28+1+session_id_len + 2])[0] 
        #print "handshake_cs_len: ", handshake_cs_len

        extension_len = struct.unpack('>H', b[44+handshake_cs_len+2+2: 44+handshake_cs_len+2+2+2])[0]
        #print "extension_len: ", extension_len
        i = 0 
        es = 44 + handshake_cs_len + 2 + 2 + 2;

        while i < extension_len:
           type = struct.unpack('>H', b[es:es+2])[0]
           len = struct.unpack('>H', b[es+2:es+4])[0]

           if type == 0:
               #print b[es+4+5:es+4+len]
               dic["host"] = b[es+4+5:es+4+len]
               dic["req_head"] = b[es+4+5:es+4+len]

           es += (len + 4)
           i += (2 + 2 + len)

        #dic['req_head'] = 
        #dic['req_']
        #dic["method"] = method
        #dic["uri"] = uri
        if dic.has_key('host'):
            return dic
        else:
            return None
		
    def register_proto(self):
        self.switch = {
           "HTTP": self.http_parse,
           "SSH": self.ssh_parse,
           "SSL": self.ssl_parse,
           "UNKNOWN": self.unknown_parse
        }

def process_line(str):
    dic = {}
    sp = 0
    ep = 4
    textlenstr = str[sp:ep]
    textlen, = struct.unpack('<i',textlenstr)
    sp = ep
    ep = sp + textlen
    text = str[sp:ep-1]

    #if '1565319276 1565319277 6 10.100.70.33 39.105.252.245 5181 80 HTTP POST 661 197 661 197'in str:
        #aa =1    

    textlist = text.split(' ')
    dic["pass"] = int(textlist[0]) 
    dic["session_start_time"] = int(textlist[1])
    dic["session_end_time"] = int(textlist[2])
    dic["prot_id"] = int(textlist[3])
    dic["srcIP"] = textlist[4]
    dic["dstIP"] = textlist[5].strip()
    dic["srcPort"] = int(textlist[6].strip())
    dic["dstPort"] = int(textlist[7].strip())
    dic["user_prot_name"] = textlist[8]
    dic["usr_prot_method"] = textlist[9]
    dic["upstreambytes"] = int(textlist[10])
    dic["downstreambytes"] = int(textlist[11])

    reqlen = int( textlist[-2])
    reslen = int(textlist[-1])
    sp = ep
    ep = sp + reqlen
    reqdata = str[sp:ep]
    sp = ep
    ep = sp+reslen
    resdata = str[sp:ep]

    reqdic = {}

    try:
        pm = ProtoParser()
        pm.register_proto()
        proto_cb = pm.switch[dic["user_prot_name"]]
        if proto_cb:
            reqdic = proto_cb(dic["usr_prot_method"], reqdata, reqlen, resdata, reslen)
        else:
            print "none this proto ", dic["user_prot_name"], ["usr_prot_method"]
            reqdic = {}
    except Exception as e:
        print "ProtoParser err", e
        reqdic = {}

    if reqdic == None:
        dic["uri"] = ""
        dic["uri_path"]=""
        dic["uri_para"]=""
        dic["Referer"] = ""
        dic["host"] = ""
        #dic["content_length"] = -1
        dic["suffix"] = ""
        dic["req_head"] = ""
        dic["hosturi"] = ""
        dic["suffix"] = ""
        dic["neteast_type"] = ""
        dic["last_modified"] = ""
    else:
        dic["req_head"] = reqdic["req_head"]
        if reqdic.has_key("uri"):
            dic["uri"] = reqdic["uri"]

            if "?" in reqdic["uri"]:
                dic["uri_path"] = reqdic["uri_path"]
                dic["uri_para"] = reqdic["uri_para"]
                if "." in dic["uri_path"]:
                    num = len (dic["uri_path"].split("."))
                    if num == 5:
                        aa =1
                    dic["suffix"]=dic["uri_path"].split(".")[num - 1]
                else:
                    dic["suffix"] = ""


            else:
                dic["uri_path"]= reqdic["uri"]
                dic["uri_para"]=""
                if "." in dic["uri"]:
                    suflist = dic["uri"].split(".")
                    if len(suflist) == 2:
                        dic["suffix"] = suflist[1].split("/")[0]
                    elif len(suflist) > 2:
                        dic["suffix"] = suflist[len(suflist)-1].split("/")[0]
                else:
                    dic["suffix"] = ""

        else:
            dic["uri"] = ""
            dic["uri_path"] = ""
            dic["uri_para"] = ""
            dic["suffix"] = ""



        if reqdic.has_key("Referer"):
            dic["Referer"] = reqdic["Referer"].strip()
        else:
            dic["Referer"] = ""

        if reqdic.has_key("host"):
            dic["host"] = reqdic["host"]
            dic["host"].strip()
        else:
            dic["host"] = ""

        if reqdic.has_key("User-Agent"):
            dic["User-Agent"] = reqdic["User-Agent"]
        else:
            dic["User-Agent"] = ""

        if reqdic.has_key("req_head2"):
            dic["req_head2"] = reqdic["req_head2"]
        else:
            dic["req_head2"] = ""

        if reqdic.has_key("neteast_type"):
            if "1024HASH" in reqdic["neteast_type"]:
                dic["neteast_type"] = 1
            elif "DOWNFILE" in reqdic["neteast_type"]:
                dic["neteast_type"] = 2
            elif "GENERATE_DETECT" in reqdic["neteast_type"]:
                dic["neteast_type"] = 3
            else:
                dic["neteast_type"] = -1
        else:
            dic["neteast_type"] = 0

    dic["hosturi"] = dic["host"] + dic["uri_path"]

 
    if not dic.has_key("status_code"):
        dic["res_code"] = 0
    
    if not dic.has_key("content-range"):
        dic["content_range"] = -1

    if not dic.has_key("content-length"):
        dic["content_length"] = -1

    if not dic.has_key("content-type"):
        dic["content_type"] = ""

    if not dic.has_key("last-modified"):
        dic["last_modified"] = ""

    return dic


def process_file(asbpath,filename):
    if   filename.startswith('unmatch_') or   filename.endswith('.tmp'):
        return
    else:
        try:
            filehandle = open(asbpath,"rb")
        except Exception,e:
            print e
            return
        filesize = os.path.getsize(asbpath)
        #print "-------------------------"
        #print asbpath
        diclist=[]
        while  filehandle.tell() < filesize:
            textlenstr = filehandle.read(4)
            textlen, = struct.unpack('<i',textlenstr)
            text = filehandle.read(textlen)
            dic = process_line(text)
            if dic == None:
                filehandle.close()
                return

            diclist.append(dic)
            cnt = len(diclist)
            #print "cnt = ",cnt
            #if cnt== 223214:
                #aa=1
        #print "======total: ",cnt
        filehandle.close()

def trans_dir(directory):
    '''
    list = os.listdir(directory)
    for i in range(0,len(list)):
        path = os.path.join(directory,list[i])
        if os.path.isfile(path):
            process_file(path,list[i])
    '''
    #process_file("/home/jz/analysis_stream/tmp/10.0.3.134_0_0_20191212_145228", "10.0.3.134_0_0_20191212_145228")
    process_file("/home/jz/analysis_stream/tmp/session_outtime_log.1576133566", "session_outtime_log.1576133566")

def Version():
    print 'ver1.0  2019_07_29'


def main(argv):
    opts, args = getopt.getopt(sys.argv[1:], 'v', ['version'])
    for key, value in opts:
        if key in['-v', '--version']:
            Version()
            return		
    #mongo = MyMongoDB(settings["ip"],settings["port"],settings["db_name"],settings["set_name"],settings["logpath"])
    #res = mongo.connect_mongo()
    #if res == -1:
    #    return
    nowTime1=datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    print nowTime1
    trans_dir(settings["filedir"])
    #mongo.connclose()
    nowTime2=datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    print nowTime2


if __name__ == "__main__":
    main(sys.argv)
