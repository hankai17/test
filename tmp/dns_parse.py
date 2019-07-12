#!/usr/bin/python
#encoding: utf-8

import os
import re
import DNS
import gevent
from gevent import socket
from gevent import Greenlet

DNS.DiscoverNameServers()

def pipe_dig(domain):
        #cmd = "dig +short " + domain
        #result = os.popen(cmd)
        #domains = []
        #for line in result:
        #    domain = line.strip('\r\n.')
        #    if not re.match("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$", domain):
        #        domains.append(domain)

        domains = []
        reqobj = DNS.Request()
        try:
                answerobj = reqobj.req(name = domain, qtype = DNS.Type.A)
        except Exception as e:
                print e
                return False,  domains

        if not answerobj:
                print "no ansoerobj"
                return False, ""

        if not len(answerobj.answers):
                print "Not found"
                return False,  "Not found."

        for item in answerobj.answers:
                if (item["typename"] == "CNAME"):
                        domains.append(item['data'])

        #print domains
        return True, domains

class Task(Greenlet):
        def __init__(self, domain, redis_proc):
                Greenlet.__init__(self)
                self.domain = domain
                self.redis_proc = redis_proc

        def _run(self):
                #self.redis_proc(self.domain, pipe_dig(self.domain))
                i = 3
                while i != 0:
                        ret, domains = pipe_dig(self.domain)
                        if ret:
                                self.redis_proc(self.domain, domains)
                                break
                        gevent.sleep(1)
                        i = i - 1

def addTask(domains, redis_proc):
        tasks = []
        for domain in domains:
                task = Task(domain, redis_proc)
                task.start()
                tasks.append(task)

        #print "tasks start end"
        gevent.joinall(tasks)


def redis_proc(domain, domians):
        a = 1
        #print "domain: " + domain + " cnames:", domians

if __name__ == '__main__':
        tasks = []
        #for i in range(1, 2):
        #       #tasks.append(gevent.spawn(pipe_dig, "www.baidu.com"))
        #       tasks.append("www.aaaaaaaaaaaaaa.bbbbbbbbbbbbb.ccccccccc.shipf")

        for i in range(1, 2):
                #tasks.append(gevent.spawn(pipe_dig, "www.baidu.com"))
                tasks.append("www.baidu.com")

        #gevent.joinall(tasks)
        tasks.append("shouji.u\xd9\x80u\xb2\xcc\x87\x05m")

        print len(tasks)    

        #exit(0)
        addTask(tasks, redis_proc)
        print "tasks end"
