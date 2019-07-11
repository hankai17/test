#!/usr/bin/env python
#encoding: utf-8

import redis, time

class RedisClt:
    def __init__(self, host_, port_, db_=0):
        pool = redis.ConnectionPool(host=host_, port=port_, db=db_)
        self.r = redis.StrictRedis(connection_pool=pool)

    def ping(self):
        return self.r.ping()

    def set(self, name, value):
        return self.r.set(name, value)

    def setex(self, name, timeout, value):
        return self.r.setex(name, timeout, value)

    def get(self, name):
        return self.r.get(name)

    def delete(self, *names):
        return self.r.delete(*names)

    def exists(self, name):
        return self.r.exists(name)

    def expire(self, name, timeout):
        return self.r.expire(name, timeout)

    def persist(self, name):
        return self.r.persist(name)

    def ttl(self, name):
        return self.r.ttl(name)

    #强制缓存,阻塞调用
    def save(self):
        return self.r.save()

    #强制缓存,异步非阻塞调用
    def bgsave(self):
        return self.r.bgsave()

'''
if __name__ == '__main__':
    HOST = '127.0.0.1'
    #HOST = '10.10.110.162'
    PORT = 6379
    DB = 0
    rc = RedisClt(HOST, PORT, DB)
    rc.set('product', 'weixin')
    val = rc.get('product')
    print val
    cnt = rc.delete(['product'])
    print cnt
    if cnt:
        print 'del element on success, cnt: [%d]' % cnt

    val = rc.setex('phone', 2, 'xiaomi')
    print rc.get('phone')
    time.sleep(3)
    print rc.get('phone')

'''
