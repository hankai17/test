#!/usr/bin/env python
#coding=utf8

import os
import sys
import time
import errno
import stat
import datetime
import socket
import struct
import atexit
import logging
from itertools import islice

#log = {'1':1, '2':2}
log = {}

log['1'] = 0
log['2'] = 0
log['3'] = 0
log['31'] = 0
log['32'] = 0
log['33'] = 0
log['35'] = 0
log['30000'] = 0

AList = [1,2,3,1,2,100]
BList = [1,2,3,1,2,7,9]
u_alist = (set(AList))
u_blist = (set(BList))
print set(AList)|set(BList)
print set(AList)^set(BList)
print (set(AList)^set(BList))&set(BList) #b独有的

print [i for i in range(1, 11)]      #列表生成式
ll = (i for i in range(1024, 4096))  #列表生成器 这样就会有迭代器 迭代器在实例化时不会计算每个项的值他们只在你要求时计算它这被称为惰性评估
print next(ll)
print next(ll)
print next(ll)

l = [[1,2],[3,4],[5,6]]
x = [j for i in l 
        for j in i]  
print x

d1 = [
    {'name':'alice', 'age':38},
    {'name':'bob',   'age':18},
    {'name':'Carl',  'age':28},
]
print sorted(d1, key = lambda x : x["age"] ) #:前是参数 后面是函数实体
                                             #sorted的第二个参数是仿函数

gen = iter(range(10)) #iter()函数用来生成迭代器
for i in islice(gen, 0, 4000):
    print(i)

#line 55

def strappend(num):
    s ='first'
    for i in range(num):
        s += str(i)
    return s
print strappend(3)

def str_append(num):
    s = 'first'
    for i in range(num):
        s += str(i)
        yield s

for i in str_append(3):
    print(i)

print [i for i in range(1, 101) if i & 0x1 == 0]
print list(range(2, 101, 2))

#python反射四个关键函数分别是 getattr(某个对象是否有某个属性) hasattr(某对象是否有某个函数) setattr delattr            
a = [('a',1),('b',2),('c',3),('d',4)]
a_1 = list(map(lambda x : x[0],a) )
print a_1
