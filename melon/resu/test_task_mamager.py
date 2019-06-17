#!/usr/bin/env python
#encoding: utf-8

from task_manager import Job, taskManage
import os, time, datetime

def dump(job):
    #print 'job dump [%s] --->' % job.jtype
    callback_routine(job.jtype)
    tmg.done_task_add(job) #job done & add to list
    if job.period > 0:
        job.addtime = time.time()
        #print 'ready to add period task: url[%s], type[%s], start[%s], period[%d]'%(job.url, job.jtype, job.addtime, job.period)
        tmg.task_add(job)

def callback_routine(idx):
    print 'job: %s do something' % idx

if __name__ == '__main__':
    tmg = taskManage(3) #ccq is 3
    for i in xrange(10):
        jtype = str(i)
        if int(i)%2 == 0:
            period = 0
            url = 'http://www.acfun.com/'
        else:
            period = 5
            url = 'http://www.bilibili.com/'
        xml_data = ''
        other = {'report': url}
        job = Job(jtype, dump, time.time(), period, url, xml_data, callback_routine, other)
        tmg.task_add(job)

    tmg.run()

    while True:
        time.sleep(2)
