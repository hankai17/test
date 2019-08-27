#! /usr/bin/python
# coding=utf8
import os, sys
from urlparse import *

#'2019-06-17/09:30:03' '10.129.0.157' '10.148.29.41' '10.129.0.157' 'GET' 'http/1.1' 'wn.pos.baidu.com' 'http://wn.pos.baidu.com/adx.php?c=d25pZD0zODlmMTUxZmQxZTA5ZDgzAHM9Mzg5ZjE1MWZkMWUwOWQ4MwB0PTE1NjA3MzUwNTcAc2U9MgBidT00AHByaWNlPVhRYnRVUUFLNThCN2pFcGdXNUlBOGx3bkpyaUJjV1VHM2JMd2l3AGNoYXJnZV9wcmljZT03OQBzaGFyaW5nX3ByaWNlPTc5MDAwAHdpbl9kc3A9NABjaG1kPTEAYmRpZD0AY3Byb2lkPQB3ZD0wAHR1PTYxNDEwNzUAYWRjbGFzcz0xMwBzcmN0PTEAcG9zPTAAYmNobWQ9MAB0bT0yNzUxNDU0NTYAYXBwPWI5ZGUzOTkyAHY9MQBpPTZhMGNlNzEy' 'Dalvik/2.1.0 (Linux; U; Android 5.1; OPPO R9tm Build/LMY47I)' '-' 'image/gif' '200' 'MISS' '80' '464' ' 17/Jun/2019:09:30:03 +0800' '1560735003942' '0' '' '' 'wn.pos.baidu.com' '183.232.231.200' '' '' '' '' '' '000' 'FIN' '-' '-' '49' '-' '49' '' '80' '' '1560735003942' '1560735003942' '1560735003983'

#{ 'nil': 0, 'null': 0, 'php' : 0, 'jpg' : 0, 'webp' : 0, 'mp4' : 0, 'js' : 0, 'jpeg' : 0, 'unio' : 0, 'html' : 0, 'gif' : 0, 'imag' : 0, 'png' : 0, 'css' : 0, 'm3u8' : 0, 'json' : 0, 'apk' : 0, 'dat' : 0, 'ini' : 0, 'mp3' : 0}

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
    fields = line.strip().split("'")
    domain = fields[13]
    if len(domain) < 3:
        continue

    hit_miss = fields[25]
    if len(hit_miss) < 2:
        continue
    if len(fields[15]) < 5:
        continue

    result = urlparse(fields[15])

    sfx = get_suffix(result.path)
    #print sfx 
 
    try:
        cl = int(fields[29])
    except BaseException:
        continue

    ddomain = process_domain(domain)
    #print ddomain
    ddomain = domain
    if len(ddomain) < 5:
        continue

    if not logs.has_key(ddomain):
        logs[ddomain] = {'hit':0, 'miss':0, 'other':0, 'sum_flow':0, 'sfx_list':[]}


    if 'HIT' in hit_miss:
        logs[ddomain]['hit'] += 1
    elif 'MISS' in hit_miss:
        logs[ddomain]['miss'] += 1
    else:
        logs[ddomain]['other'] += 1

    logs[ddomain]['sum_flow'] += cl
    (logs[ddomain]['sfx_list']).append(sfx)
    logs[ddomain]['sfx_list'] = list(set(logs[ddomain]['sfx_list']))

print "%-50s%-10s%-10s%-10s%-10s" %("domain", "miss", "hit", "hit/sum", "sum_flow")
for k in logs.keys():
    if logs[k]['hit'] + logs[k]['miss'] != 0:
        sfx_str = ''
        for i in logs[k]['sfx_list']:
            sfx_str += (i + ' ')
        print "%-50.45s%-10d%-10d%-10d%-10d  %s" %(k, logs[k]['miss'], logs[k]['hit'], (logs[k]['hit'] * 100/(logs[k]['hit'] + logs[k]['miss'])), logs[k]['sum_flow'], sfx_str)
fd.close()
