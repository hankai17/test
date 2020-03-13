#! /usr/bin/python
# coding=utf8
import os, sys
from urlparse import *

#140.249.28.207                                    1538006        0              1538006        0              1              100              m4a

if len(sys.argv) < 2:
    print "python 1.py path"
    sys.exit()
path = sys.argv[1]
files= os.listdir(path)

logs = {}
All_flow = 0
for filename in files:
    if 'result.txt' not in filename:
        print filename
        continue
    fd = open(filename, 'r')
    for line in fd.readlines():
        fields = line.strip().split()
        domain = fields[0]
        if len(domain) < 3:
            continue
        ddomain = domain
    
        try:
            hit_flow = int(fields[1])
            miss_flow = int(fields[2])
	    sum_flow = int(fields[3])
	    flow_rate  =  int(fields[4])
	    sum_req =  int(fields[5])
	    hit_rate =  int(fields[6])
	    sfx = fields[7]
		
        except BaseException:
            continue

	print "-->", miss_flow, hit_flow, sum_flow, sum_req, hit_rate, sfx
        if not logs.has_key(ddomain):
            logs[ddomain] = {'miss_flow':0, 'hit_flow':0, 'sum_flow':0, 'flow_rate':0, 'sum_req':0, 'hit_rate':0, 'sfx_list':[]}

	logs[ddomain]['miss_flow'] += miss_flow
	logs[ddomain]['hit_flow'] += hit_flow
	logs[ddomain]['sum_flow'] += sum_flow
	logs[ddomain]['sum_req'] += sum_req
        (logs[ddomain]['sfx_list']).append(sfx)
        logs[ddomain]['sfx_list'] = list(set(logs[ddomain]['sfx_list']))
   	All_flow += sum_flow 
    
tmp_f = open("_last.txt", 'w+')
print "%-50s%-15s%-15s%-15s%-15s%-15s%-15s%-15s" %("domain", "hit_flow", "miss_flow", "sum_flow", "flow/ALL", "sum_req", "hit/sum_req", "type")
for k in logs.keys():
    if logs[k]['hit_flow'] + logs[k]['miss_flow'] != 0:
        sfx_str = ''
        for i in logs[k]['sfx_list']:
            sfx_str += (i + ' ')
        tmp_str = "%-50.45s%-15d%-15d%-15d%-15d%-15d%-15d  %s" %(k, logs[k]['hit_flow'], logs[k]['miss_flow'], logs[k]['sum_flow'],
	logs[k]['sum_flow'] * 100 / (All_flow), 
	logs[k]['sum_req'],
	logs[k]['sum_flow'] * 100 / (All_flow), 
	sfx_str)
	tmp_f.write(tmp_str + '\n')
fd.close()
tmp_f.close()

