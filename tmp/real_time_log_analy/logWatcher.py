#!/usr/bin/env python

import os
import sys
import time
import errno
import stat
import datetime
import socket
import struct
import atexit
import logging

#from lru import LRUCacheDict
from logging import handlers
from task_manager import Job, taskManage
from ctypes import *
from urlparse import *
from multiprocessing import Process,Lock
from log_obj import CLog
from parse_conf import  cConfParser

log_file = "timelog.log"
log_fmt = '%(asctime)s: %(message)s'
config_file = 'test.config'

domain_white_dict = {}
pps_ip_list = []
pps_port = 0
domain_sfx_err_count = 0
domain_sfx_err_rate = 0
ats_ip = ''

def daemonize(pid_file=None):
    pid = os.fork()
    if pid:
        sys.exit(0)
    os.chdir('/')
    os.umask(0)
    os.setsid()
    _pid = os.fork()
    if _pid:
        sys.exit(0)
    sys.stdout.flush()
    sys.stderr.flush()
    with open('/dev/null') as read_null, open('/dev/null', 'w') as write_null:
        os.dup2(read_null.fileno(), sys.stdin.fileno())
        os.dup2(write_null.fileno(), sys.stdout.fileno())
        os.dup2(write_null.fileno(), sys.stderr.fileno())

    if pid_file:
        with open(pid_file, 'w+') as f:
            f.write(str(os.getpid()))
        atexit.register(os.remove, pid_file)

def get_suffix(p):
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

    def loop(self, interval=0.1, async=False):
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
        if self.extensions:
            return [x for x in ls if os.path.splitext(x)[1][1:] in self.extensions and self.logfile_kw in os.path.split(x)[1]  ]
        else:
            return ls

    @staticmethod
    def tail(fname, window):
        try:
            f = open(fname, 'r')
        except IOError, err:
            if err.errno == errno.ENOENT:
                return []
            else:
                raise
        else:
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
            return data.splitlines()[-window:]

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
                    if not stat.S_ISREG(st.st_mode):
                        continue
                    fid = self.get_file_id(st)
                    ls.append((fid, absname))
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

    def unwatch(self, file, fid):
        lines = self.readfile(file)
        self.log("un-watching logfile %s" % file.name)
        del self.files_map[fid]
        if lines:
            self.callback(file.name, lines)

    @staticmethod
    def get_file_id(st):
        return "%xg%x" % (st.st_dev, st.st_ino)

    def close(self):
        for id, file in self.files_map.iteritems():
            file.close()
        self.files_map.clear()

def udp_send_message(ip_list, port, arr):
    for ip in ip_list:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.sendto(arr, (ip, port))
        s.close()
        

def pull_data(job):
    if not (job.sfx == "nil" or job.sfx == "null"):
	fmt = "=HHHH%dsH%dsH" %(len(job.url),len(job.sfx))
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
	fmt = "=HHHH%dsH" %(len(job.url))
    	data = struct.pack(
	    fmt,
    	    80,      #id
    	    1,       #type
    	    8 + len(job.url) + 1, #length
    	    len(job.url), #domain_len
	    job.url,
	    0
	)
    global pps_ip_list
    global pps_port	
    udp_send_message(pps_ip_list, pps_port, data)
    tmg.done_task_add(job)
    log_message = job.url + ' ' + job.sfx
    loger.write(20, log_message)

def callback_routine(idx):
    print 'callback_routinue'

def get_domain_white(f):
    if len(f) == 0:
	print 'No domain_white_list'
	return
    filename = f 
    fd = open(filename, 'r')
    for line in fd.readlines():
	line = line.strip()
	if not domain_white_dict.has_key(line):
	    domain_white_dict[line] = 1
    print 'parse domain_white_list done'

def period_check_task(job):
    global txn_idx
    global once_flag
    if txn_idx == 0 and once_flag == 0:
	once_flag = 1
    	tmg.done_task_add(job)
        job.addtime = time.time()
        tmg.task_add(job)
	return
	
    loger.write(10, '------>')
    mutex.acquire()
    for k in d1.keys():
        if domain_white_dict.has_key(k):
            continue
        for k1 in d1[k].keys():
            err_rate = d1[k][k1]['not_ok'] * 100 / (d1[k][k1]['not_ok'] + d1[k][k1]['20x'])
	    log_message = k +  ' ' + str(err_rate)
    	    loger.write(10, log_message)
	    global domain_sfx_err_count
	    global domain_sfx_err_rate
            if err_rate >= domain_sfx_err_rate and (d1[k][k1]['not_ok'] + d1[k][k1]['20x']) >= domain_sfx_err_count :
                #print "will add to task", k, k1, "ok:", d1[k][k1]['20x'], "not_ok:", d1[k][k1]['not_ok'], "err rate:", err_rate
                txn_idx += 1
                job = Job(txn_idx, pull_data, time.time(), 0, k, '', callback_routine, k1, '')
                tmg.task_add(job)
    loger.write(10, '<------')
    d1.clear()
    mutex.release()

    tmg.done_task_add(job)
    if job.period > 0:
        job.addtime = time.time()
        tmg.task_add(job)

def config_parse():
    global domain_sfx_err_count
    global domain_sfx_err_rate
    global pps_ip_list
    global pps_port
    global ats_ip
    cp = cConfParser(config_file)
    pps_ip = cp.get('common', 'pps_ip')
    fields = pps_ip.strip().split('|')
    if len(fields) > 0:
	for i in fields:
	    pps_ip_list.append(i)
    else:
	pps_ip_list.append(pps_ip)

    pps_port = int(cp.get('common', 'pps_port'))
    domain_sfx_err_count = int(cp.get('common', 'domain_sfx_err_count' ))
    domain_sfx_err_rate = int(cp.get('common', 'domain_sfx_err_rate' ))
    ats_ip = cp.get('common', 'ats_ip')

    print 'ats_ip: ', ats_ip
    print 'pps_ip: ', pps_ip
    print 'pps_port: ', pps_port
    print 'domain_sfx_err_count: ', domain_sfx_err_count
    print 'domain_sfx_err_rate: ', domain_sfx_err_rate
    return cp

once_flag = 0
txn_idx = 0
d1 = {}
mutex = Lock()
version_message = '1.0.1'
#1.0.1: Add conf obj; Add log obj
#1.0.2: More pps. add tool config

if __name__ == '__main__':
    help_message = 'Usage: python %s' % sys.argv[0]
    if len(sys.argv) == 2 and (sys.argv[1] in '--version'):
	print version_message
	exit(1)
    if len(sys.argv) == 2 and (sys.argv[1] in '--help'):
	print help_message
	exit(1)
    if len(sys.argv) != 1:
	print help_message
	exit(1)

    cp = config_parse()
    get_domain_white(cp.get('common', 'domain_white_list'))
    loger = CLog(log_file, log_fmt, 12, 5, cp.get('common', 'debug'))
    print 'Start ok'

    daemonize()

    tmg = taskManage()
    tmg.run()
    pull_pps_job = Job(txn_idx, period_check_task, time.time(), int(cp.get('common', 'interval')), '', '', callback_routine, '', '')
    tmg.task_add(pull_pps_job)
    def callback(filename, lines):
        for line in lines:
            fields = line.strip().split("'")
            http_code = fields[23]
            domain = fields[13]
	    log_message = 'new line ' + domain
    	    #loger.write(10, log_message)
            if len(domain.split(":")) > 0:
                domain = domain.split(":")[0]
	    user_ip = fields[5]
	    result = urlparse(fields[15])
	    sfx = get_suffix(result.path)
	    if sfx == 'nil' or sfx == 'null':
		continue

            if len(domain) <= 3:
            	continue
	    #is watch req
	    global ats_ip
	    if user_ip == ats_ip:
		continue
	    
	    mutex.acquire()
	    sfx_dict = None
	    if not d1.has_key(domain):
		d1[domain] = {}
		sfx_dict = d1[domain]
	    else:
		sfx_dict = d1[domain]

	    if not sfx_dict.has_key(sfx):
		sfx_dict[sfx] = {'20x':0, 'not_ok':0}

            if not(http_code in "200" or http_code in "206" or http_code in "304" or http_code in "204"): 
		sfx_dict[sfx]['not_ok'] += 1
	    else:
		sfx_dict[sfx]['20x'] += 1
	    mutex.release()

    l = LogWatcher("/opt/ats/var/log/trafficserver", callback)
    l.loop()


#https://docs.python.org/2/library/ctypes.html
#https://blog.csdn.net/u012611644/article/details/80529746
