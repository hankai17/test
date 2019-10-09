#!/usr/bin/env python
#coding=utf8

import subprocess
import json
from collections import namedtuple

disk_ntuple = namedtuple('partition', 'device mountpoint fstype')

def disk_partitions(all=False):
    """Return all mountd partitions as a nameduple.
    If all == False return phyisical partitions only.
    """
    phydevs = []
    f = open("/proc/filesystems", "r")
    for line in f:
        if not line.startswith("nodev"):
            phydevs.append(line.strip())

    retlist = []
    f = open('/etc/mtab', "r")
    for line in f:
        if not all and line.startswith('none'):
            continue
        fields = line.split()
        device = fields[0]
        mountpoint = fields[1]
        fstype = fields[2]
        if not all and fstype not in phydevs:
            continue
        if device == 'none':
            device = ''
        ntuple = disk_ntuple(device, mountpoint, fstype)
        retlist.append(ntuple)
    return retlist

def get_ppc_mount_point(mounts):
    ppc_arr = []
    for i in mounts:
        tmp_dict = i._asdict()
        if 'sd' in tmp_dict['device']:
            if 'home' in tmp_dict['mountpoint']:
                ppc_arr.append(tmp_dict['mountpoint'])
    return ppc_arr

all_mount = disk_partitions()
ppc_mounts = get_ppc_mount_point(all_mount)

mounts = []
for home in ppc_mounts:
    if len(home) != 0:
        mounts.append({'{#MOUNT_NAME}':home[1:]})
print json.dumps({'data':mounts},indent=4,separators=(',',':'))
