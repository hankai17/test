# -*- encoding=utf-8 -*-
#!/usr/bin/python

import sys
import os
import struct
import socket
import time
import re
from Config import *

HOST='10.0.2.209'
PORT='8000'

def Checkandgetargv():
        if(3 == len(sys.argv)):
                reIP = re.compile('^((25[0-5]|2[0-4]\d|[01]?\d\d?)\.){3}(25[0-5]|2[0-4]\d|[01]?\d\d?)$')
                isPort = re.compile('^(6553[0-5]|655[0-2][0-9]|65[0-4][0-9]{2}|6[0-4][0-9]{3}|[1-5][0-9]{4}|[1-9][0-9]{1,3}|[0-9])$')
                try:
                        argvIP = sys.argv[1]
                        argvPort = sys.argv[2]
                except:
                        print 'sys argv ip or port error!'
                        os._exit(1)
                if not reIP.match(sys.argv[1]):
                        print 'sys argv ip error!'
                        os._exit(1)
                if not isPort.match(sys.argv[2]):
                        print 'sys argv port error!'
                        os._exit(2)
        if(2 == len(sys.argv)):
                if '-V' in sys.argv:
                        print 'Version 1.0, date[2017-08-02]'
                        os._exit(0)
                if '-v' in sys.argv:
                        print 'Version 1.0, date[2017-08-02]'
                        os._exit(0)

        #return argvIP, int(argvPort)

def Getaddblackorwhitedata(fmt,recvdata, rcmd):
        #fmt='=cI40sI' 
        #print fmt
        upkrecvdata = struct.unpack(fmt, recvdata)
        if(rcmd == 143):
                cmd = 0x8b
        else:
                cmd = ord(upkrecvdata[0]) + 1
        msid = socket.ntohl(upkrecvdata[1])
        rspblackdata = struct.pack('=cI', chr(cmd), socket.htonl(msid))
        return rspblackdata

def CreatServ():
        fd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        iport = int(port)
        fd.bind((host,iport))
        return fd

def main():
        Checkandgetargv()
        try:
                fd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                #fd.bind((HOST,int(PORT)))
                fd.bind((LBE['IP'],LBE['PORT']))
                fd.settimeout(60)
                while True:
                        #recvdata = fd.recv(1024)
                        recvdata,address = fd.recvfrom(1024)
                        recvcmd = struct.unpack('=c', recvdata[0])
                        #print ord(recvcmd[0])
                        if(ord(recvcmd[0])==140): #8c
                                print 'add to black'
                                if len(recvdata)==57:
                                    fmtb='=cI40sIII'
                                if len(recvdata)==53:
                                    fmtb='=cI40sII'
                                if len(recvdata)==49:
                                    fmtb='=cI40sI'
                                rspblackdata = Getaddblackorwhitedata(fmtb,recvdata, ord(recvcmd[0]))
                                fd.sendto(rspblackdata, address)
                        if(ord(recvcmd[0])==138 or ord(recvcmd[0])==143): #0x8a  0x8f
                                print 'add to white'
                                if len(recvdata)==90:
                                    fmtw='=cI40s40sIc'
                                else:
                                    fmtw='=cI40s40sI5s'
                                rspwhitedata = Getaddblackorwhitedata(fmtw,recvdata, ord(recvcmd[0]))
                                fd.sendto(rspwhitedata, address)

        except Exception,e:
                print Exception,":",e
                fd.close()
                os._exit(0)

if __name__ == '__main__':
       main()
