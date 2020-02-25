#! /usr/bin/python
# coding=utf8
import os, sys
from urlparse import *


#81 2020/02/25 00:00:00 10.20.227.138 DISK_HIT 277 GET /b4a41340ae328520655e0b456b3560b4/5e54012c/video/tos/cn/tos-cn-ve-15/476ff3f40fc4471ba8c9c9be8e4d3be4/?a=1128&br=3832&bt=958&cr=2&cs=2&dr=0&ds=6&er=&l=20200224235959010144061072344A9766&lr=&qs=11&rc=M288eDt3eDtnczMzZGkzM0ApOzg8ZDM6NGQzNzNkOWU3aGczMmUyaV4xYTNfLS0vLS9zc2AyMS42YTYxMC41LzMxXi46Yw%3D%3D&vl=&vr= DIRECT NOADDR v27-dy.ixigua.com 206 INSIDE_CLOSE_2 1582559998 1582560000 0 1900980 3599673 455C2D95F74989A47B3BE602A38ECA6FB93CD39B R_HIT NULL 1132389 |||ttplayer(2.9.5.565)

filename = sys.argv[1]

fd = open(filename, 'r')

logs = {}

def process_domain(domain):
    fields = domain.split(".")
    if len(fields) >= 3:
        tmp = fields[len(fields) - 3] + "." + fields[len(fields) - 2] + "." + fields[len(fields) - 1]
        return tmp

def get_suffix(p):
    if len(p) == 1:
        #return "pure domain"
        return "nil"
    fields = p.split(".")
    if len(fields) == 0 or len(fields) == 1:
        #return "no sfx"
        return "null"
    return fields[len(fields) - 1]

for line in fd.readlines():
    fields = line.strip().split(" ")
    domain = fields[10]
    if len(domain) < 3:
        continue

    hit_miss = fields[4]

    result = urlparse(fields[7])
    sfx = get_suffix(result.path)
    #print sfx 
 
    try:
        cl = int(fields[16])
    except BaseException:
        continue

    ddomain = process_domain(domain)

    ddomain = domain
    if len(ddomain) < 5:
        continue
    if len(ddomain.split(".")) < 2:
        print line
    if not logs.has_key(ddomain):
        logs[ddomain] = {'hit':0, 'miss':0, 'other':0, 'hit_sum':0, 'miss_sum':0, 'sum_flow':0 }


    if 'HIT' in hit_miss:
        logs[ddomain]['hit'] += 1
        logs[ddomain]['hit_sum'] += cl
    elif 'MISS' in hit_miss:
        logs[ddomain]['miss'] += 1
        logs[ddomain]['miss_sum'] += cl
    else:
        logs[ddomain]['other'] += 1

    logs[ddomain]['sum_flow'] += cl

#print "%-50s%-10s%-10s%-10s%-10s%-10s%" %("domain", "miss", "hit", "hit/sum", "hit_sum/sum", "sum_flow")
print "%-50s%-10s%-10s%-10s%-10s%-10s" %("domain", "miss", "hit", "hit/sum", "hit/sum_flow", "sum_flow")
for k in logs.keys():
    if logs[k]['hit'] + logs[k]['miss'] == 0 or logs[k]['hit_sum'] + logs[k]['miss_sum'] == 0:
   	print  "%-50.45s%-10d%-10d%-10d%-10d%-10d  " %(k, logs[k]['miss'], logs[k]['hit'], 0, 0, logs[k]['miss'])
    if logs[k]['hit'] + logs[k]['miss'] != 0 and logs[k]['hit_sum'] + logs[k]['miss_sum'] != 0:
        print "%-50.45s%-10d%-10d%-10d%-10d%-10d  " %(k, logs[k]['miss'], logs[k]['hit'], (logs[k]['hit'] * 100/(logs[k]['hit'] + logs[k]['miss'])), 
		(logs[k]['hit_sum'] * 100 / (logs[k]['hit_sum'] + logs[k]['miss_sum'])), logs[k]['sum_flow'])
fd.close()
