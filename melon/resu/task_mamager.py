#!/usr/bin/env python
#encoding: utf-8
#说明: 1.利用gevent+queue实现了一个批量有序处理异步任务和定时任务的任务调度机制,对定时任务优先处理
#      3.每批量任务处理完成后,会做一个任务完成情况的统计,便于我们知道任务执行概况,然后再继续执行下一批任务
#      7.在运行过程中,可以任意添加任务到任务队列并得到执行
#      8.每次取出定时任务时,需要立即更新其addtime值为下一个period,避免该定时任务被反复取出
#TODO:
#      1.任务优先级控制
#      2.任务延时控制
#概念:
#    尽可能的fetch n个 task去执行 即只要有即时任务立即执行 只要时间到了也立即执行
#
#一轮: 
#    异步并发n个请求 然后 死等这n个异步任务完成 ------>这里需要注意的是 n个异步任务 这个任务也是耗时间的 比如1ms 1s 100s不等 所以这个任务必须必须必须有超时设置！
#
#疑问:
#    可以实时的add task到队列里吗?
#
#架构:
#    对于那些实时性要求比较高的场景: 比如必须得每隔5s钟实时计算当前负载 然后推送 这种场景下必须必须必须控制每一轮的时间不能超过5s

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
        print 'tasks scheduling...'

    def task_add(self, job):
        if job.period > 0:
            new_one = 1
            for i in xrange(len(self.timedQ)):
                ele = self.timedQ[i]
                if ele.jtype == job.jtype:
                    new_one = 0
                    if ele.addtime < job.addtime:
                        ele.addtime = job.addtime
                        print 'update period task addtime %f' % ele.addtime
                    break
            if new_one:
                self.timedQ.append(job)
                print 'added a period task: %s, type: %s, period: %s' % (job.url, job.jtype, job.period)
            else:
                print 'skip a period task: %s, type: %s, period: %s' % (job.url, job.jtype, job.period)
        elif job.period == 0:
            self.taskQ.put(job)
            print 'added a async task: %s, type: %s, period: %s' % (job.url, job.jtype, job.period)
        else:
            print 'skip a invalid task: %s, type: %s, period: %s' % (job.url, job.jtype, job.period)

    def done_task_add(self, job):
        self.doneQ.put(job)

    def task_fetch(self):
        job = ''
        for i in xrange(len(self.timedQ)): #period task priority
            ele = self.timedQ[i]
            #print 'period task, started[%d], url[%s], type[%s], period[%s], addtime[%f]' %(ele.started, ele.url, ele.jtype, ele.period, ele.addtime)
            #print 'period type is int?', isinstance(ele.period, int)
            if ele.started == 0: #first
                job = ele
                ele.started = 1
                ele.addtime += ele.period
                #print 'fetched a period task (first)'
                break
            elif ele.started and ele.period > 0 and (ele.period < (time.time() - ele.addtime)): #reach to exe point
                job = ele
                ele.addtime += ele.period
                #print 'fetched a period task (non-first)'
                break
        if not job: #no period task
            try:
                job = self.taskQ.get_nowait()
            except gevent.queue.Empty:
                gevent.sleep(5)
        if job:
            print 'fetch one task: %s, type: %s, period: %s' % (job.url, job.jtype, job.period)
        return job

    def task_schedule(self):
        while True:
            num_running = 0
            while num_running > 0 or not self.taskQ.empty() or len(self.timedQ):
                while num_running < self.max_conn:
                    job = self.task_fetch()
                    if not job: continue
                    #print 'get a task from taskQ/timedQ, url [%s], type [%s], period [%s]' % (job.url, job.jtype, job.period)
                    #print 'ready to process a task [%s]' % job.url
                    #print job.other
                    gevent.spawn(job.exec_fn, job)
                    num_running += 1
                print 'ccq all running: %d, max: %d' % (num_running, self.max_conn)
                while not self.doneQ.empty() or num_running > 0: #once job done until all ccq job done
                    #print 'collecting completed task ......'
                    job = self.doneQ.get()
                    print 'job done: %s, type: %s, period: %s' % (job.url, job.jtype, job.period)
                    num_running -= 1
                print 'ccq all completed running: %d, max: %d' % (num_running, self.max_conn)
                gevent.sleep(0) #for next ccq
            gevent.sleep(1)
