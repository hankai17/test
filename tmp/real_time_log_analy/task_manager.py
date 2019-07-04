#!/usr/bin/env python
#encoding: utf-8

import gevent
from gevent import monkey
monkey.patch_all()
from gevent import queue
import os, time, datetime

class Job:
    def __init__(self, jtype, exec_fn, addtime, period, url, xml_data, callback, other):
        self.started = 0 #is it first, aimed at period task
        self.jtype = jtype #id or hash
        self.exec_fn = exec_fn #jb cb
        self.addtime = float(addtime) #task_add time, period task reset this value periodly
        self.period = int(period)
        self.url = url
        self.xml_data = xml_data #some date
        self.cbk_fn = callback #job done cb
        self.other = other

class taskManage: #schedule obj
    def __init__(self, concurrent = 30):
        self.taskQ = gevent.queue.Queue()
        self.doneQ = gevent.queue.Queue()
        self.timedQ = [] #add period task to timedQ
        self.max_conn = concurrent

    def run(self):
        gevent.spawn(self.task_schedule).start()
        ##print 'tasks scheduling...'

    def task_add(self, job):
        if job.period > 0:
            new_one = 1
            for i in xrange(len(self.timedQ)):
                ele = self.timedQ[i]
                if ele.jtype == job.jtype:
                    new_one = 0
                    if ele.addtime < job.addtime:
                        ele.addtime = job.addtime
                        ##print 'update period task addtime %f' % ele.addtime
                    break
            if new_one:
                self.timedQ.append(job)
                #print 'added a period task: %s, type: %s, period: %s' % (job.url, job.jtype, job.period)
            else:
                #print 'skip a period task: %s, type: %s, period: %s' % (job.url, job.jtype, job.period)
		pass
        elif job.period == 0:
            self.taskQ.put(job)
            #print 'added a async task: %s, type: %s, period: %s' % (job.url, job.jtype, job.period)
        else:
            #print 'skip a invalid task: %s, type: %s, period: %s' % (job.url, job.jtype, job.period)
	    pass

    def done_task_add(self, job):
        self.doneQ.put(job)

    def task_fetch(self):
        job = ''
        for i in xrange(len(self.timedQ)): #period task priority
            ele = self.timedQ[i]
            ##print 'period task, started[%d], url[%s], type[%s], period[%s], addtime[%f]' %(ele.started, ele.url, ele.jtype, ele.period, ele.addtime)
            ##print 'period type is int?', isinstance(ele.period, int)
            if ele.started == 0: #first
                job = ele
                ele.started = 1
                ele.addtime += ele.period
                ##print 'fetched a period task (first)'
                break
            elif ele.started and ele.period > 0 and (ele.period < (time.time() - ele.addtime)): #reach to exe point
                job = ele
                ele.addtime += ele.period
                ##print 'fetched a period task (non-first)'
                break
        if not job: #no period task
            try:
                job = self.taskQ.get_nowait()
            except gevent.queue.Empty:
                gevent.sleep(5)
        if job:
            #print 'fetch one task: %s, type: %s, period: %s' % (job.url, job.jtype, job.period)
	    pass
        return job

    def task_schedule(self):
        while True:
            num_running = 0
            while num_running > 0 or not self.taskQ.empty() or len(self.timedQ):
                while num_running < self.max_conn:
                    job = self.task_fetch()
                    if not job: continue
                    ##print 'get a task from taskQ/timedQ, url [%s], type [%s], period [%s]' % (job.url, job.jtype, job.period)
                    ##print 'ready to process a task [%s]' % job.url
                    ##print job.other
                    gevent.spawn(job.exec_fn, job)
                    num_running += 1
                #print 'ccq all running: %d, max: %d' % (num_running, self.max_conn)
                while not self.doneQ.empty() or num_running > 0: #once job done until all ccq job done
                    ##print 'collecting completed task ......'
                    job = self.doneQ.get()
                    #print 'job done: %s, type: %s, period: %s' % (job.url, job.jtype, job.period)
                    num_running -= 1
                #print 'ccq all completed running: %d, max: %d' % (num_running, self.max_conn)
                gevent.sleep(0) #for next ccq
            gevent.sleep(1)
