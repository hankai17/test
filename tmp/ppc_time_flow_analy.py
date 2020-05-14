#! /usr/bin/python
# coding=utf8
import os, sys
from urlparse import *

#80 2020/03/12 11:23:13 10.107.179.16 SSD_HIT 60 GET /upic/2020/03/11/18/BMjAyMDAzMTExODE4MzdfMjE1MzA4NTgwXzI0ODA3NTcyMTQ4XzJfMw==_swft_B480c0b28716e266b6f43f062b431afb1.mp4?tag=1-1583983353-h-0-jyzubulvpn-75ac05548588f4b9&type=hot DIRECT NOADDR alimov2.a.yximgs.com 206 OK_1 1583983393 1583983393 0 1049900 7831092 BEDFF14F6DCC88E7D9E40FA513525BD1F0C654EA R_HIT NULL 1 00775C854D90135B4B57A640C6144AA08FD5CED0 |||a/com.smile.gifmaker/7.2.0.12758/r3.8.2.6.1539.7b43c6bac/088706QTEQLIgfJsiTdtrqSDbCtDymVc/091057nKDvit6fyqwzfNzUY9REK5lf2p/cache

if len(sys.argv) < 2:
    print "python 1.py logfile"
    sys.exit()
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

All_flow = 0
for line in fd.readlines():
    fields = line.strip().split(" ")
    domain = fields[10]
    if len(domain) < 3:
        continue

    hit_miss = fields[4]

    result = urlparse(fields[7])
    sfx = get_suffix(result.path)
    if len(sfx) > 7:
        sfx = "null" 
 
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
        logs[ddomain] = {'hit':0, 'miss':0, 'other':0, 'hit_sum':0, 'miss_sum':0, 'sum_flow':0, 'sfx_list':[]}


    if 'HIT' in hit_miss:
        logs[ddomain]['hit'] += 1
        logs[ddomain]['hit_sum'] += cl
    elif 'MISS' in hit_miss:
        logs[ddomain]['miss'] += 1
        logs[ddomain]['miss_sum'] += cl
    else:
        logs[ddomain]['other'] += 1
    All_flow += cl

    logs[ddomain]['sum_flow'] += cl
    (logs[ddomain]['sfx_list']).append(sfx)
    logs[ddomain]['sfx_list'] = list(set(logs[ddomain]['sfx_list']))

#print "%-50s%-10s%-10s%-10s%-10s%-10s%" %("domain", "miss", "hit", "hit/sum", "hit_sum/sum", "sum_flow")
#print "%-50s%-10s%-10s%-10s%-10s%-10s" %("domain", "miss", "hit", "hit/sum", "hit/sum_flow", "sum_flow")

tmp_f = open(filename + "_result.txt", 'w+')
#print "%-50s%-15s%-15s%-15s%-15s%-15s%-15s%-15s" %("domain", "hit_flow", "miss_flow", "sum_flow", "flow/ALL", "sum_req", "hit/sum_req", "type")
for k in logs.keys():
    if logs[k]['hit'] + logs[k]['miss'] == 0 or logs[k]['hit_sum'] + logs[k]['miss_sum'] == 0:
        #print  "%-50.45s%-10d%-10d%-10d%-10d%-10d  " %(k, logs[k]['miss'], logs[k]['hit'], 0, 0, logs[k]['miss'])
        pass
    if logs[k]['hit'] + logs[k]['miss'] != 0 and logs[k]['hit_sum'] + logs[k]['miss_sum'] != 0:
        sfx_str = ''
        for i in logs[k]['sfx_list']:
            sfx_str += (i + ' ')
        tmp_str = "%-50.45s%-15d%-15d%-15d%-15.8f%-15d%-15d  %s" %(k, logs[k]['hit_sum'], logs[k]['miss_sum'], logs[k]['sum_flow'],
        (logs[k]['sum_flow'] * 100.0) / (All_flow), logs[k]['hit'] + logs[k]['miss'] + logs[k]['other'], (logs[k]['hit'] * 100/(logs[k]['hit'] + logs[k]['miss'])), 
                sfx_str)
        #print "(logs[k]['sum_flow'] * 100) / (All_flow)", (logs[k]['sum_flow'] * 100), "/", All_flow
        tmp_f.write(tmp_str + '\n')
fd.close()
tmp_f.close()
