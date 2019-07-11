#!/usr/bin/env python
#encoding: utf-8

import ConfigParser

class cConfParser:
    #overwrite-是否将配置文件的修改写到磁盘上面?1重写,0不重写
    def __init__(self, conf_path, overwrite=0):
        self.fpath = conf_path #配置文件路径,要求是绝对路径
        self.overwrite = overwrite
        self.cf = ConfigParser.ConfigParser() #ConfigParser对象实例
        self.cf.read(self.fpath) #一启动就读取配置文件

    def __del__(self):
        #是否将修改后的配置刷新到磁盘,会丢掉注释
        if self.overwrite == 1:
            #一关闭就将ConfigParser对象的内容序列化到本地配置文件中
            with open(self.fpath, 'w') as fh:
                self.cf.write(fh)

    #将字符串配置的表达式转换为整型,求形如500 * 1024字符串的积
    def make_product(self, val):
        if '*' not in val:
            return int(val)
        else:
            seg = val.split('*')
            product = 1
            for i in xrange(len(seg)):
                product = product * int(seg[i])
            return product

    #添加指定的节
    def add_section(self, s):
        if not self.cf.has_section(s):
            self.cf.add_section(s)

    #移除指定的节
    def remove_section(self, s):
        return self.cf.remove_section(s)

    #返回指定节内的选项值
    def get(self, s, o):
        return self.cf.get(s, o)

    #获取指定节内的整型的选项值
    def getint(self, s, o):
        return self.cf.getint(s, o)

    #新增或修改指定节内的选项值
    def set(self, s, o, v):
        if self.cf.has_section(s):
           self.cf.set(s, o, v)

    #移除指定节内的指定选项
    def remove_option(self, s, o):
        if self.cf.has_section(s):
            return self.cf.remove_option(s, o)
        return False

    #返回节内的(key, val)列表
    def items(self, s):
        return self.cf.items(s)

    #返回所有节的列表
    def sections(self):
        return self.cf.sections()

    #返回节内的key列表
    def options(self, s):
        return self.cf.options(s)

if __name__ == '__main__':
    config_file = './test.config'
    cp = cConfParser(config_file)
    print cp.sections()
    print cp.items('ats')
    print cp.options('ats')
    print cp.get('ats', 'ats_log')

    print cp.get('redis', 'redis_port')
    cp.set('redis', 'redis_port', '3')
    print cp.get('redis', 'redis_port')
    print cp.remove_option('redis', 'redis_p')

    #cp.add_section('Copyright')
    #print cp.remove_section('Copyright')
    #cp.add_section('Copyright2')
