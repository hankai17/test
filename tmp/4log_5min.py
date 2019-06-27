#! /usr/bin/python
# coding=utf8
import os, sys

#1511107364.826 0.001 200 [20/Nov/2017:00:02:44 +0800] 112.65.48.236 "iosapps.itunes.apple.com HEAD /apple-assets-us-std-000001/Purple12" 988 "153.101.56.247" TCP_MEM_HIT:DIRECT - "com.apple.apd/1.0 iOS/11.0.3 model/iPhone8,2 hwp/s8000 build/15A432 (6; dt:121)" application/octet-stream "127.0.0.1:8081"

filename = sys.argv[1]
print filename

fd = open(filename, 'r')

logs = {}  #5min: ip: {req_num sumflow}
ip = ''

for line in fd.readlines():
    fields = line.strip().split()
    all_time = fields[3]
    list_min = all_time.split(':')[1:]
    #print list_min

    key_min = int(list_min[1])
    five_min_key = key_min - key_min % 5

    key_time = list_min[0] + '%02d' % (five_min_key)
    #print key_time

    ip = fields[5]
    flow = int(fields[10])

    time_dict = None
    if logs.has_key(key_time):
        time_dict = logs[key_time]
    else:
        logs[key_time] = {}
        time_dict = logs[key_time]

    ip_dict = None
    if time_dict.has_key(ip):
        ip_dict = time_dict[ip]
    else:
        time_dict[ip] = {'req_num':0, 'sum_flow':0}
        ip_dict = time_dict[ip]

    ip_dict['req_num'] = ip_dict['req_num'] + 1
    ip_dict['sum_flow'] = ip_dict['sum_flow'] + flow 

#print ' '.join(logs)


for k in logs.keys():
    print k 
    for ip in logs[k]:
        #print '\t', ip, logs[k][ip]['req_num'], logs[k][ip]['sum_flow'] / (1024 * 1024)
        print '\t', ip, logs[k][ip]['req_num'], logs[k][ip]['sum_flow'] 
fd.close()

#! /usr/bin/python
# coding=utf8
import os, sys

#1533571201.755 0.626 206 [07/Aug/2018:00:00:01 +0800] 10.17.130.26 "182.254.49.76 GET /om.tc.qq.com/ADeBrAzY02xB3u2E7vpNnA5ilNjSxcp_Fp6l2ttng4NM/e05534ogx24.mp4?vkey=9A14C1464AC1FF42A45DCFCFCF45EEB7172240E7D2C4711515967678BC94765FFAAA2EBE35966D932A6BA4865E6975A19312134F6E44FFA36AF6F6EC23A3F6A7A74C8920B468A8C4C98B318D67F923DC7107E4D097FC19EE&platform=4180301 HTTP/1.1" 262690 "182.254.49.76" TCP_MISS:DIRECT - "MicroMessenger Client" video/mp4 "127.0.0.1:9012"

filename = sys.argv[1]
print filename

fd = open(filename, 'r')

logs = {}
ip = ''

for line in fd.readlines():
    fields = line.strip().split()
    all_time = fields[3]
    list_min = all_time.split(':')[1:]
    #print list_min #['00', '17', '43']

    key_min = int(list_min[1])
    five_min_key = key_min - key_min % 5

    key_time = list_min[0] + '%02d' % (five_min_key)
    #print key_time

    flow = int(fields[10])
    hit_flow = 0
    miss_flow = 0
    other_flow = 0
    
    domain = fields[6]
    url = fields[8]
    if domain.find('ums.uc.cn') != -1:
    #if url.find('ums\.uc\.cn') != -1:
    #if r'om.tc.qq.com' in url:
    #if r'video.ums.uc.cn' in url:
        if fields[12].find('HIT') != -1:
            hit_flow = flow
        elif fields[12].find('MISS') != -1:
            miss_flow = flow
        else:
            other_flow = flow

        hmo_dict = None
        if logs.has_key(key_time):
            hmo_dict = logs[key_time]
        else:
            logs[key_time] = {}
            hmo_dict = logs[key_time]

        if hmo_dict.has_key('hit') or hmo_dict.has_key('miss') or hmo_dict.has_key('other'):
            hmo_dict['hit'] += hit_flow
            hmo_dict['miss'] += miss_flow
            hmo_dict['other'] += other_flow
        else:
            hmo_dict = {'hit':0, 'miss':0, 'other':0}
            logs[key_time] = hmo_dict
            hmo_dict['hit'] += hit_flow
            hmo_dict['miss'] += miss_flow
            hmo_dict['other'] += other_flow

for k in logs.keys():
    print k ,logs[k]
#    print k
#    for j in logs[k]:
#        print logs[k][j]
fd.close()

 cat result.txt | awk 'BEGAIN{sum_miss = 0;} { len=split($7, s, "}"); if(len>0) {sum_miss+=s[1]}; print sum_miss}'
  cat result.txt | awk 'BEGAIN{sum_miss = 0;} { len=split($3, s, ","); if(len>0) {sum_miss+=s[1]}; print sum_miss}'
   cat result.txt | awk 'BEGAIN{hit_miss = 0;} { len=split($3, s, ","); if(len>0) {miss=s[1]}; len1=split($7, a, "}"); if(len1>0) {hit=a[1]} printf("%f\n", (hit+miss)/miss) }'

