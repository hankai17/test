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
from mongoop import MyMongoDB
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

    def http_parse(self, method, req_str, req_len, resp_str, resp_len, down, dstip, dport):
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
        return dic

    def http_resp_parse(self, method, req_str, req_len, resp_str, resp_len, down, dstip):
        dic = {}
        if "HTTP" not in resp_str:
            return dic
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

    def ssh_parse(self, method, req_str, req_len, resp_str, resp_len, down, dstip, dport):
        pass

    def unknown_parse(self, method, req_str, req_len, resp_str, resp_len, down, dstip, dport):
        #print dport
        pass

    def str_to_hex(self, s):
        return ' '.join([hex(ord(c)).replace('0x', '') for c in s])

    def ssl_parse(self, method, req_str, req_len, resp_str, resp_len, down, dstip, dport):
        dic = {}
        dic['host'] = dstip
        dic["req_head"] = dstip

        if req_len < 4:
            return  dic
        b = bytearray(req_str)
        if int(b[0]) != 22:
            #print "not handshake", method, int(b[0])
            return  dic
        try:
            ssl_version = struct.unpack('>H', b[1:3])[0]
            ssl_len = struct.unpack('>H', b[3:5])[0]
        except Exception as e:
            return  dic

        #print "ssl_version: ", ssl_version, "ssl_len: ", ssl_len, "req_len: ", req_len
        if ssl_len + 5 != req_len:
            pass
            #print 'not complete', 'ssl_ori_len:', ssl_len + 5, 'real_len:', req_len
            #return  None
        
        handshake_type = int(b[5])
        if handshake_type != 1:
            #print 'not hello', method
            return  dic

        session_id_len = 0
        handshake_cs_len = 0
        compress_method_len = 0
        extension_len = 0

        try:
            handshake_len = struct.unpack('>B', b[6:7])[0] * 256 + struct.unpack('>H', b[7:9])[0]
            handshake_version = struct.unpack('>H', b[9:11])[0]
            #print "------------>version: ", handshake_version
            #if int(handshake_version) == 771:
                #return None
            #print "handshake_len: ", handshake_len, "handshake_version: ", handshake_version

            session_id_len = struct.unpack('>B', b[11+4+28:11+4+28+1])[0] #GMT(4) + random bytes(28) + sessionlen(1) + len(session)
            #print "session_id_len: ", session_id_len

            handshake_cs_len = struct.unpack('>H', b[11+4+28+1+session_id_len: 11+4+28+1+session_id_len + 2])[0] #GMT(4) + random bytes(28) + sessionlen(1) + len(session) + cslen(2) + len(cs)
            #print "handshake_cs_len: ", handshake_cs_len

            compress_method_len = struct.unpack('>B', b[11+4+28+1+session_id_len+2+handshake_cs_len: 11+4+28+1+session_id_len+2+handshake_cs_len+1])[0] 
            #print "compress_method_len: ", compress_method_len

            #hex_str = self.str_to_hex(req_str)
            #print hex_str

            extension_len = struct.unpack('>H', b[11+4+28+1+session_id_len+2+handshake_cs_len+1+compress_method_len: 11+4+28+1+session_id_len+2+handshake_cs_len+1+compress_method_len+2])[0]
        except Exception as e:
            return  dic

        i = 0 
        es = 11+4+28+1+session_id_len+2+handshake_cs_len+1+compress_method_len+2
        try:
            while i < extension_len:
               type = struct.unpack('>H', b[es:es+2])[0]
               len = struct.unpack('>H', b[es+2:es+4])[0]

               if type == 0:
                   #print b[es+4+5:es+4+len]
                   dic["host"] = str(b[es+4+5:es+4+len]).decode("unicode_escape")
                   dic["req_head"] = str(b[es+4+5:es+4+len]).decode("unicode_escape")
                   #print "dic['host']: ", str(dic['host']), down
                   break

               es += (len + 4)
               i += (2 + 2 + len)
        except Exception as e:
            return dic
        return  dic
		
    def register_proto(self):
        self.switch = {
           "HTTP": self.http_parse,
           "SSH": self.ssh_parse,
           "SSL": self.ssl_parse,
           "UNKNOWN": self.unknown_parse
        }

def mongo_operation(dic,mongo):

    if len(dic) == 0:
        return
    #print "len(dic)=",len(dic)
    #mongo.insert(dic)
    #print "-------------------insert over:",len(dic);

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
            reqdic = proto_cb(dic["usr_prot_method"], reqdata, reqlen, resdata, reslen, dic["downstreambytes"], dic["dstIP"], dic["dstPort"] )
        else:
            print "none this proto ", dic["user_prot_name"], ["usr_prot_method"]
            reqdic = {}
    except Exception as e:
        #print "ProtoParser err", e, dic["user_prot_name"], dic["downstreambytes"]
        reqdic = None

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
        if dic["user_prot_name"] == "SSL":
            dic['host'] = dic["dstIP"]
            dic["req_head"] = dic["dstIP"]

    else:
        '''
        if dic["user_prot_name"] == "SSL":
           if reqdic["host"]:
               print "host", reqdic["host"], dic["downstreambytes"]
           else:
               print "nohst", dic["downstreambytes"]
        '''

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
   
    resdic = {} 
    if "HTTP" in resdata:
        resdic = pm.http_resp_parse(dic["usr_prot_method"], reqdata, reqlen, resdata, reslen, dic["downstreambytes"], dic["dstIP"])

    if resdic.has_key("status_code"):
        dic["res_code"] = int(resdic["status_code"])
        #print dic["res_code"]
    else:
        dic["res_code"] = 0
    
    if resdic.has_key("content-range"):
        dic["content_range"] = resdic["content-range"]
    else:
        dic["content_range"] = -1

    if resdic.has_key("content-length"):
        dic["content_length"] = int(resdic["content-length"])
    else:
        dic["content_length"] = -1

    if resdic.has_key("content-type"):
        dic["content_type"] = resdic["content-type"]
    else:
        dic["content_type"] = ""

    if resdic.has_key("last-modified"):
        dic["last_modified"] = resdic["last-modified"]
    else:
        dic["last_modified"] = ""

    return dic


def process_file(asbpath,filename,mongo):
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
        mongo_operation(diclist,mongo)
        filehandle.close()

def trans_dir(directory,mongo):
    list = os.listdir(directory)
    for i in range(0,len(list)):
        path = os.path.join(directory,list[i])
        if os.path.isfile(path):
            process_file(path,list[i],mongo)

def Version():
    print 'ver1.0  2019_07_29'


def main(argv):
    opts, args = getopt.getopt(sys.argv[1:], 'v', ['version'])
    for key, value in opts:
        if key in['-v', '--version']:
            Version()
            return		
    mongo = MyMongoDB(settings["ip"],settings["port"],settings["db_name"],settings["set_name"],settings["logpath"])
    res = mongo.connect_mongo()
    if res == -1:
        return
    nowTime1=datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    print nowTime1
    trans_dir(settings["filedir"],mongo)
    mongo.connclose()
    nowTime2=datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    print nowTime2


if __name__ == "__main__":
    main(sys.argv)
