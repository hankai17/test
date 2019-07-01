#! /usr/bin/python
# coding=utf8
import os, sys
from urlparse import *

sfx_log = { 'nil': 0, 'null': 0, 'php' : 0, 'jpg' : 0, 'webp' : 0, 'mp4' : 0, 'js' : 0, 'jpeg' : 0, 'unio' : 0, 'html' : 0, 'gif' : 0, 'imag' : 0, 'png' : 0, 'css' : 0, 'm3u8' : 0, 'json' : 0, 'apk' : 0, 'dat' : 0, 'ini' : 0, 'mp3' : 0, 'image':0, 'union':0}

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
        print p
        return "null"
    #print fields[len(fields) - 1]
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
    if len(sfx) > 6:
        continue
    if sfx == "v3":
        print line
        break
    if not sfx_log.has_key(sfx):
        print sfx

    #ddomain = process_domain(domain)
    #print ddomain
    ddomain = domain
    if len(ddomain) < 5:
        continue

    if not logs.has_key(ddomain):
        logs[ddomain] = {'nil': 0, 'null': 0, 'php' : 0, 'jpg' : 0, 'webp' : 0, 'mp4' : 0, 'js' : 0, 'jpeg' : 0, 'unio' : 0, 'html' : 0, 'gif' : 0, 'imag' : 0, 'png' : 0, 'css' : 0, 'm3u8' : 0, 'json' : 0, 'apk' : 0, 'dat' : 0, 'ini' : 0, 'mp3' : 0, 'ico' : 0 , 'gif' : 0, 'image':0, 'union':0}
    if 'nil' in sfx:
        logs[ddomain]['nil'] += 1
    if 'null' in sfx:
        logs[ddomain]['null'] += 1
    if 'php' in sfx:
        logs[ddomain]['php'] += 1
    if 'jpg' in sfx:
        logs[ddomain]['jpg'] += 1
    if 'webp' in sfx:
        logs[ddomain]['webp'] += 1
    if 'mp4' in sfx:
        logs[ddomain]['mp4'] += 1
    if 'js' in sfx:
        logs[ddomain]['js'] += 1
    if 'jepg' in sfx:
        logs[ddomain]['jpeg'] += 1
    if 'html' in sfx:
        logs[ddomain]['html'] += 1
    if 'png' in sfx or 'PNG' in sfx:
        logs[ddomain]['png'] += 1
    if 'css' in sfx:
        logs[ddomain]['css'] += 1
    if 'm3u8' in sfx:
        logs[ddomain]['m3u8'] += 1
    if 'json' in sfx:
        logs[ddomain]['json'] += 1
    if 'apk' in sfx:
        logs[ddomain]['apk'] += 1
    if 'dat' in sfx:
        logs[ddomain]['dat'] += 1
    if 'ini' in sfx:
        logs[ddomain]['ini'] += 1
    if 'mp3' in sfx:
        logs[ddomain]['mp3'] += 1
    if 'ico' in sfx:
        logs[ddomain]['ico'] += 1
    if 'gif' in sfx:
        logs[ddomain]['gif'] += 1
    if 'image' in sfx:
        logs[ddomain]['image'] += 1
    if 'union' in sfx:
        logs[ddomain]['union'] += 1
for k in logs.keys():
    #print k, logs[k]['nil'], logs[k]['null'],logs[k]['php'],logs[k]['jpg'],logs[k]['webp'],logs[k]['mp4'],logs[k]['js'],logs[k]['jpeg'],logs[k]['html'],logs[k]['png'],logs[k]['css'],logs[k]['m3u8'],logs[k]['json'],logs[k]['apk'],logs[k]['dat'],logs[k]['ini'],logs[k]['mp3']
    print k,
    for k1,j in logs[k].items():
        if j >= 1:
            print k1,
    print
    

#    if not logs.has_key(ddomain):
#        logs[ddomain] = {'hit':0, 'miss':0, 'other':0 }
#
#
#    if 'HIT' in hit_miss:
#        logs[ddomain]['hit'] += 1
#    elif 'MISS' in hit_miss:
#        logs[ddomain]['miss'] += 1
#    else:
#        logs[ddomain]['other'] += 1
#
#for k in logs.keys():
#    #print k ,logs[k]['miss'],logs[k]['hit'],"%.2f%%" % (logs[k]['hit'] * 100/(logs[k]['hit'] + logs[k]['miss']))
#    print k ,logs[k]['miss'],logs[k]['hit'],"%d" % (logs[k]['hit'] * 100/(logs[k]['hit'] + logs[k]['miss']))
#    #print k
#    #for j in logs[k]:
#    #    print logs[k][j]
fd.close()
