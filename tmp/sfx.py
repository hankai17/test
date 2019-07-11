#! /usr/bin/python
# coding=utf8
import os, sys
from urlparse import *

filename = sys.argv[1]

fd = open(filename, 'r')

logs = {}

def get_suffix(p):
    if len(p) == 1:
        #return "pure domain"
        return "nil"
    fields = p.split(".")
    if len(fields) == 0 or len(fields) == 1:
        #return "no sfx"
        return "null"
    #return fields[len(fields) - 1]
    if fields[len(fields) - 1].find('/') != -1:
        return "null"
    return fields[len(fields) - 1]

def get_suffix1(p):
    if len(p) == 1:
        #return "pure domain"
        return "nil"
    fields = p.split("/")
    if len(fields) == 0 or len(fields) == 1:
        return "null"
    fields1 = fields[len(fields) - 1].split(".")
    if len(fields1) == 0 or len(fields1) == 1:
        return "null"
    else:
        return fields1[len(fields1) - 1]


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

    sfx = get_suffix1(result.path)
    print sfx
