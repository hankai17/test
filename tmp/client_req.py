import socket,select
import time

clientsock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

clientsock.connect(('127.0.0.1', 81))
#clientsock.send('GET /v3-hs.ixigua.com/1bade5996fafe4289a995638f9c0f90f/5d67945d/video/m/220af7d4d6f93c7478080040a9d7bfcbb8311632f39d00002ed96a64cb3b/?rc=amk8OzM5ZXRlbzMzM2YzM0ApbGl2cXJ3a3UpNTc1NDQ3MzQ2NDk8OzwzQClpN2c8NGZmZWQ3NDwzOzs4Z
yl1KUBnNnUpQGYzdSk0NGRtYmQvXjZxMzVfLS00LTBzc3VjclxscGw6aTAuMDAtLi8tLS0tLi8tLi9pXi0xXjIuMzJhLTQ1MjBiLjpjOmIwcCM6YS1wIzpgLy46 HTTP/1.1\r\nAccept-Encoding: identity\r\nRange: bytes=0-\r\nvideo_id: v0300c700000bl8b6qtb96a028g9efpg\r\nline: 
0\r\napp_id: 1112\r\nvquality: normal\r\nwatermark: 2\r\nlong_video: 0\r\nsf: 1\r\nts: 1567065457\r\nlive_sdk_version: 740\r\niid: 83867998712\r\ndevice_id: 57730526655\r\nac: wifi\r\nchannel: xiaomi\r\naid: 1112\r\napp_name: live_strea
m\r\nversion_code: 740\r\nversion_name: 7.4.0\r\ndevice_platform: android\r\nssmix: a\r\ndevice_type: Redmi Note 4\r\ndevice_brand: Xiaomi\r\nlanguage: zh\r\nos_api: 23\r\nos_version: 6.0\r\n\r\nuuid: 862844034731347\r\nopenudid: 3aa074
0b22fa5c40\r\nmanifest_version_code: 740\r\nresolution: 1080*1920\r\ndpi: 480\r\nupdate_version_code: 7402\r\n_rticket: 1567065661458\r\nab_version: 661939,712301,938230,1113904,1005399,1096187,1121140,692223,889329,1120467,830473,11258
55,1095512,1124136,955276,1039774,1038812,947985,1048437,932075,848691,1030028,819012,1080023,689931,1119778,957250,929432,841998,1108349,1063522,1076100,988862,1128566,994817,1124121,1032650,1136385,1011944,797937,839333,1104584,104848
6,1028658,1121875,1104675,1002041,1022366,1119160,1029612,1133591,943434,557631,682009,1075333,956109,1057900,1122992,1019139,1060649,972530,1135692,1032070,1017639,1069234,770250,1072545,1114547,915089,501249,841788,508755,1122978,8324
57,1092636,643979,1124754,1119266,1046183,1106060,1124556,975752,1055279,1077423,1075141,1132750,1123575,1050089\r\nclient_version_code: 740\r\njssdk_version: 2.12.2.h6\r\nmcc_mnc: 46000\r\nHost: 123.247.36.72:85\r\nConnection: Keep-Ali
ve\r\nUser-Agent: okhttp/3.10.0.1\r\n\r\n')
clientsock.send('GET /v3-hs.ixigua.com/1bade5996fafe4289a995638f9c0f90f/5d67945d/video/m/220af7d4d6f93c7478080040a9d7bfcbb8311632f39d00002ed96a64cb3b/?rc=amk8OzM5ZXRlbzMzM2YzM0ApbGl2cXJ3a3UpNTc1NDQ3MzQ2NDk8OzwzQClpN2c8NGZmZWQ3NDwzOzs4Zy
l1KUBnNnUpQGYzdSk0NGRtYmQvXjZxMzVfLS00LTBzc3VjclxscGw6aTAuMDAtLi8tLS0tLi8tLi9pXi0xXjIuMzJhLTQ1MjBiLjpjOmIwcCM6YS1wIzpgLy46 HTTP/1.1\r\nAccept-Encoding: identity\r\nRange: bytes=0-\r\nvideo_id: v0300c700000bl8b6qtb96a028g9efpg\r\nline: 0
\r\napp_id: 1112\r\nvquality: normal\r\nwatermark: 2\r\nlong_video: 0\r\nsf: 1\r\nts: 1567065457\r\nlive_sdk_version: 740\r\niid: 83867998712\r\ndevice_id: 57730526655\r\nac: wifi\r\nchannel: xiaomi\r\naid: 1112\r\napp_name: live_stream
\r\nversion_code: 740\r\nversion_name: 7.4.0\r\ndevice_platform: android\r\nssmix: a\r\ndevice_type: Redmi Note 4\r\ndevice_brand: Xiaomi\r\nlanguage: zh\r\nos_api: 23\r\nos_version: 6.0\r\n\r\nuuid: 862844034731347\r\nopenudid: 3aa0740
b22fa5c40\r\nmanifest_version_code: 740\r\nresolution: 1080*1920\r\ndpi: 480\r\nupdate_version_code: 7402\r\n_rticket: 1567065661458\r\nab_version: 661939,712301,938230,1113904,1005399,1096187,1121140,692223,889329,1120467,830473,112585
5,1095512,1124136,955276,1039774,1038812,947985,1048437,932075,848691,1030028,819012,1080023,689931,1119778,957250,929432,841998,1108349,1063522,1076100,988862,1128566,994817,1124121,1032650,1136385,1011944,797937,839333,1104584,1048486
,1028658,1121875,1104675,1002041,1022366,1119160,1029612,1133591,943434,557631,682009,1075333,956109,1057900,1122992,1019139,1060649,972530,1135692,1032070,1017639,1069234,770250,1072545,1114547,915089,501249,841788,508755,1122978,83245
7,1092636,643979,1124754,1119266,1046183,1106060,1124556,975752,1055279,1077423,1075141,1132750,1123575,1050089\r\nclient_version_code: 740\r\njssdk_version: 2.12.2.h6\r\nmcc_mnc: 46000\r\nHost: 123.247.36.72:85\r\nConnection: Keep-Aliv
e\r\nUser-Agent: okhttp/3.10.0.1\r\n\r\n')
#time.sleep(1)
#clientsock.send('Host: 123.247.36.72:85\r\nConnection: Keep-Alive\r\nUser-Agent: okhttp/3.10.0.1\r\n\r\n')
print "sendok"
time.sleep(10)
buff=clientsock.recv(10000)
print "no data coming"
