#! /usr/bin/python
# coding=utf8
import os, sys, commands
from urlparse import *

#'2019-06-17/09:30:03' '10.129.0.157' '10.148.29.41' '10.129.0.157' 'GET' 'http/1.1' 'wn.pos.baidu.com' 'http://wn.pos.baidu.com/adx.php?c=d25pZD0zODlmMTUxZmQxZTA5ZDgzAHM9Mzg5ZjE1MWZkMWUwOWQ4MwB0PTE1NjA3MzUwNTcAc2U9MgBidT00AHByaWNlPVhRYnRVUUFLNThCN2pFcGdXNUlBOGx3bkpyaUJjV1VHM2JMd2l3AGNoYXJnZV9wcmljZT03OQBzaGFyaW5nX3ByaWNlPTc5MDAwAHdpbl9kc3A9NABjaG1kPTEAYmRpZD0AY3Byb2lkPQB3ZD0wAHR1PTYxNDEwNzUAYWRjbGFzcz0xMwBzcmN0PTEAcG9zPTAAYmNobWQ9MAB0bT0yNzUxNDU0NTYAYXBwPWI5ZGUzOTkyAHY9MQBpPTZhMGNlNzEy' 'Dalvik/2.1.0 (Linux; U; Android 5.1; OPPO R9tm Build/LMY47I)' '-' 'image/gif' '200' 'MISS' '80' '464' ' 17/Jun/2019:09:30:03 +0800' '1560735003942' '0' '' '' 'wn.pos.baidu.com' '183.232.231.200' '' '' '' '' '' '000' 'FIN' '-' '-' '49' '-' '49' '' '80' '' '1560735003942' '1560735003942' '1560735003983'

#{ 'nil': 0, 'null': 0, 'php' : 0, 'jpg' : 0, 'webp' : 0, 'mp4' : 0, 'js' : 0, 'jpeg' : 0, 'unio' : 0, 'html' : 0, 'gif' : 0, 'imag' : 0, 'png' : 0, 'css' : 0, 'm3u8' : 0, 'json' : 0, 'apk' : 0, 'dat' : 0, 'ini' : 0, 'mp3' : 0}

if len(sys.argv) < 2:
    print 'python top100_squid.py squid.log'
    sys.exit()

if sys.argv[1] in '--help':
    print 'python top100_squid.py squid.log'
    sys.exit()
    

filename = sys.argv[1]
if len(sys.argv) > 2:
    if int(sys.argv[2]) > 500:
        topn = 500;
    else:
        topn = sys.argv[2]
else:
    topn = 100

def sort_k():
    file_name = "top" + str(topn) + ".txt"
    cmd = 'cat 1.txt | sort -k5nr | head -n ' + str(topn)  + ' > ' +  file_name
    result = commands.getoutput(cmd) 
    os.unlink('1.txt')
    help_str = "%-50s%-10s%-10s%-10s%-10s%s%s%s" %("domain", "miss", "hit", "hit/sum", "sum_flow", "  sfx", " sfx_sum_flow", " hit/sum")
    
    cmd1 = "sed -i '1i" + help_str + "' " + file_name
    commands.getoutput(cmd1)

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
        logs[ddomain] = {'hit':0, 'miss':0, 'other':0, 'sum_flow':0 } # {jpg: {sum_flow, hit/sum}, {png: ...} ...}

    if not logs[ddomain].has_key('sfx_dict'):
        logs[ddomain]['sfx_dict'] = {}

    if not logs[ddomain]['sfx_dict'].has_key(sfx):
        logs[ddomain]['sfx_dict'][sfx] = {'sfx_sum_flow':0, 'sfx_hit':0, 'sfx_miss':0 }

    if 'HIT' in hit_miss:
        logs[ddomain]['hit'] += 1
        logs[ddomain]['sfx_dict'][sfx]['sfx_hit'] += 1
    elif 'MISS' in hit_miss:
        logs[ddomain]['miss'] += 1
        logs[ddomain]['sfx_dict'][sfx]['sfx_miss'] += 1
    else:
        logs[ddomain]['other'] += 1

    logs[ddomain]['sum_flow'] += cl
    logs[ddomain]['sfx_dict'][sfx]['sfx_sum_flow'] += cl

tmp_f = open('1.txt', 'w+')

#print "%-50s%-10s%-10s%-10s%-10s%s%s%s" %("domain", "miss", "hit", "hit/sum", "sum_flow", "  sfx", " sfx_sum_flow", " hit/sum")
for k in logs.keys():
    if logs[k]['hit'] + logs[k]['miss'] != 0:
        pstr = ' '
        for k1 in logs[k]['sfx_dict']:
            if (logs[k]['sfx_dict'][k1]['sfx_hit'] + logs[k]['sfx_dict'][k1]['sfx_miss']) != 0:
                pstr += str(k1 + " " + str(logs[k]['sfx_dict'][k1]['sfx_sum_flow']) + " " + str(logs[k]['sfx_dict'][k1]['sfx_hit'] * 100/(logs[k]['sfx_dict'][k1]['sfx_hit'] + logs[k]['sfx_dict'][k1]['sfx_miss'])) + " ")
        #print "%-50.45s%-10d%-10d%-10d%-10d %s" %(k, logs[k]['miss'], logs[k]['hit'], (logs[k]['hit'] * 100/(logs[k]['hit'] + logs[k]['miss'])), logs[k]['sum_flow'], pstr)
        tmp_str = "%-50.45s%-10d%-10d%-10d%-10d %s" %(k, logs[k]['miss'], logs[k]['hit'], (logs[k]['hit'] * 100/(logs[k]['hit'] + logs[k]['miss'])), logs[k]['sum_flow'], pstr)
        tmp_f.write(tmp_str + '\n')
fd.close()
tmp_f.close()

sort_k()

#domain                                            miss      hit       hit/sum   sum_flow
#ali2.a.yximgs.com                                 489461    290484    37        29390587958  gif 264024 0 webp 8997890808 15 png 2506859 86 jpg 7177970959 70 kpg 13211955308 34
