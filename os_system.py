#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import os

f = open("1.log", "r")
for line in f.readlines():
    line = line.strip().strip('\r\n')
    git_command = 'git rm -rf ' + line
    print git_command
    os.system(git_command)

f.close()
