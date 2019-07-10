#!/usr/bin/env python

import os
import sys
import time
import errno
import stat
import datetime
import socket
import struct
import logging

#from lru import LRUCacheDict
from logging import handlers
from task_manager import Job, taskManage
from ctypes import *
from urlparse import *

logger = logging.getLogger(__name__)
log_file = "timelog.log"
domain_white_list = {}

fh = handlers.TimedRotatingFileHandler(filename=log_file,when="H",interval=12,backupCount=0)
formatter = logging.Formatter('%(asctime)s: %(message)s')
fh.setFormatter(formatter)
logger.addHandler(fh)

def get_suffix(p):
    if len(p) == 1:
        #return "pure domain"
        return "nil"
    fields = p.split(".")
    if len(fields) == 0 or len(fields) == 1:
        #return "no sfx"
        return "null"
    return fields[len(fields) - 1]

class LogWatcher(object):
    def __init__(self, folder, callback, extensions=["log"], logfile_keyword="squid", tail_lines=0):
        self.files_map = {}
        self.callback = callback
        self.folder = os.path.realpath(folder)
        self.extensions = extensions
        self.logfile_kw = logfile_keyword
        assert os.path.exists(self.folder), "%s does not exists" % self.folder
        assert callable(callback)
        self.update_files()
        for id, file in self.files_map.iteritems():
            file.seek(os.path.getsize(file.name))  # EOF
            if tail_lines:
                lines = self.tail(file.name, tail_lines)
                if lines:
                    self.callback(file.name, lines)

    def __del__(self):
        self.close()

    def loop(self, interval=0.05, async=False):
        while 1:
            try:
                self.update_files()
                for fid, file in list(self.files_map.iteritems()):
                    self.readfile(file)
                if async:
                    return
                time.sleep(interval)
            except KeyboardInterrupt:
                break

    def log(self, line):
        print line

    def listdir(self):
        ls = os.listdir(self.folder)
        if self.extensions: #os.path 常用方法 可以直接获取文件名之类   不知道可不可以分割出来url   可以直接提取sfx
            return [x for x in ls if os.path.splitext(x)[1][1:] in self.extensions and self.logfile_kw in os.path.split(x)[1]  ]
        else:
            return ls

    @staticmethod #跟c++中的静态方法一样 不用实例化 可以直接用 
    def tail(fname, window):
        try:
            f = open(fname, 'r')
        except IOError, err:
            if err.errno == errno.ENOENT:
                return []
            else:
                raise
        else:  #如果try里面的语句成功执行,那么就执行else里面的语句
            BUFSIZ = 1024
            f.seek(0, os.SEEK_END)
            fsize = f.tell()
            block = -1
            data = ""
            exit = False
            while not exit:
                step = (block * BUFSIZ)
                if abs(step) >= fsize:
                    f.seek(0)
                    exit = True
                else:
                    f.seek(step, os.SEEK_END)
                data = f.read().strip()
                if data.count('\n') >= window:
                    break
                else:
                    block -= 1
            return data.splitlines()[-window:] #取后几行

    def update_files(self):
        ls = []
        if os.path.isdir(self.folder):
            for name in self.listdir():
                absname = os.path.realpath(os.path.join(self.folder, name))
                try:
                    st = os.stat(absname)
                except EnvironmentError, err:
                    if err.errno != errno.ENOENT:
                        raise
                else:
                    if not stat.S_ISREG(st.st_mode): #一般文件 
                        continue
                    fid = self.get_file_id(st)
                    ls.append((fid, absname)) #数组里是元组
        elif os.path.isfile(self.folder):
            absname = os.path.realpath(self.folder)
            try:
                st = os.stat(absname)
            except EnvironmentError, err:
                if err.errno != errno.ENOENT:
                    raise
            else:
                fid = self.get_file_id(st)
                ls.append((fid, absname))
        else:
            print 'You submitted an object that was neither a file or folder...exiting now.'
            sys.exit()

        for fid, file in list(self.files_map.iteritems()):
            try:
                st = os.stat(file.name)
            except EnvironmentError, err:
                if err.errno == errno.ENOENT:
                    self.unwatch(file, fid)
                else:
                    raise
            else:
                if fid != self.get_file_id(st):
                    self.unwatch(file, fid)
                    self.watch(file.name)

        for fid, fname in ls:
            if fid not in self.files_map:
                self.watch(fname)

    def readfile(self, file):
        lines = file.readlines()
        if lines:
            self.callback(file.name, lines)

    def watch(self, fname):
        try:
            file = open(fname, "r")
            fid = self.get_file_id(os.stat(fname))
        except EnvironmentError, err:
            if err.errno != errno.ENOENT:
                raise
        else:
            self.log("watching logfile %s" % fname)
            self.files_map[fid] = file

    def unwatch(self, file, fid): #unwatch时 也要读文件?
        lines = self.readfile(file)
        self.log("un-watching logfile %s" % file.name)
        del self.files_map[fid] #map的删除
        if lines:
            self.callback(file.name, lines)

    @staticmethod
    def get_file_id(st):
        return "%xg%x" % (st.st_dev, st.st_ino)

    def close(self):
        for id, file in self.files_map.iteritems():
            file.close()
        self.files_map.clear()

def udp_send_message(ip, port, arr):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.sendto(arr, (ip, port))
    s.close()
        

def pull_data(job):
    if not (job.sfx == "nil" or job.sfx == "null"):
        fmt = "HHHH%dsH%dsH" %(len(job.url),len(job.sfx))
        data = struct.pack(
            fmt,
            80,      #id
            1,       #type
            8 + len(job.url) + 2 + len(job.sfx) + 1, #length
            len(job.url), #domain_len
            job.url,      #domain
            len(job.sfx), #sfx_len
            job.sfx,      #sfx
            0
        )
    else:
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

    udp_send_message(settings['pps_ip'], settings['pps_port'], data)
    tmg.done_task_add(job)
    log_message = job.url + ' ' + job.sfx
    logger.warning(log_message)

def callback_routine(idx):
    print 'callback_routinue'

settings={
    'interval':10,
    'hit':1,
    'domain_sfx_err_rate':0,
    'domain_sfx_err_count':1,
    'local_ip':'10.0.110.21',
    'pps_ip':'10.0.110.82',
    'pps_port':7111
}

cur_time = float(time.time())
txn_idx = 0
if __name__ == '__main__':
    d1 = {}
    tmg = taskManage()
    tmg.run()
    def callback(filename, lines):
        for line in lines:
            fields = line.strip().split("'")
            http_code = fields[23]
            domain = fields[13]
            if len(domain.split(":")) > 0:
                domain = domain.split(":")[0]
            user_ip = fields[5]
            result = urlparse(fields[15])
            sfx = get_suffix(result.path)
            #print fields[15], sfx

            if len(domain) <= 3:
                continue
            #is watch req
            if user_ip == settings['local_ip']:
                continue
            
            sfx_dict = None
            if not d1.has_key(domain):
                d1[domain] = {}
                sfx_dict = d1[domain]
            else:
                sfx_dict = d1[domain]

            if not sfx_dict.has_key(sfx):
                sfx_dict[sfx] = {'20x':0, 'not_ok':0}

            if not(http_code in "200" or http_code in "206" or http_code in "304"): 
                sfx_dict[sfx]['not_ok'] += 1
            else:
                sfx_dict[sfx]['20x'] += 1

            global cur_time
            global txn_idx
            if float(time.time()) - cur_time >= settings['interval']:
                cur_time = float(time.time())
                print "------>"
                for k in d1.keys():
                        #print k, d1[k]
                    for k1 in d1[k].keys():
                        err_rate = d1[k][k1]['not_ok'] * 100 / (d1[k][k1]['not_ok'] + d1[k][k1]['20x'])
                        #print k1, err_rate
                        if err_rate >= settings['domain_sfx_err_rate'] and (d1[k][k1]['not_ok'] + d1[k][k1]['20x']) >= settings['domain_sfx_err_count'] :
                            print "will add to task", k, k1, err_rate
                            txn_idx += 1
                            job = Job(txn_idx, pull_data, time.time(), 0, k, '', callback_routine, k1, '')
                            tmg.task_add(job)
                print "<------"
                d1.clear()

    l = LogWatcher("/opt/ats/var/log/trafficserver", callback)
    l.loop()

#https://docs.python.org/2/library/ctypes.html
#https://blog.csdn.net/u012611644/article/details/80529746
