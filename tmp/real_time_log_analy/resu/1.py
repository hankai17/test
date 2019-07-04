#!/usr/bin/env python

import os
import sys
import time
import errno
import stat
import struct

from ctypes import *

class Item(Structure):
    _fields_ = [
    ('id', c_uint16),
    ('type', c_uint16),
    ('length', c_uint16),
    ('domain_len', c_uint16),
    #('domain_start', c_char_p)
    ('domain_start', c_void_p)
    ]

'''
pd = Item()
pd.id = 1
pd.type = 2
pd.length = 3
pd.domain_len = 4
pd.domain_start = "www.ifeng.com"

#pd.domain_len = len("www.ifeng.com")
#pd.domain_start = "www.ifeng.com"

#print (string_at(addressof(pd),sizeof(pd)))
print sizeof(pd)

fd = open('test_struct', 'wb+')
fd.write(string_at(addressof(pd),sizeof(pd)))
fd.close()
'''

data = struct.pack(
#'=H1024',
'HHHH1024s',
4,
3,
2,
1,
'www.ifeng.com'
)

print data

fd = open('test_struct', 'wb+')
fd.write(data)
fd.close()
