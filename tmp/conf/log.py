#!/usr/bin/env python
#encoding: utf-8

import os, logging, logging.handlers

class CLog:
    #fn-绝对路径的日志文件名,fmt-日志格式,size-日志文件大小,backup-保留日志文件个数, level-日志级别
    def __init__(self, fn, fmt, size, backup, level='DEBUG'):
        #如果日志存放目录不存在, 先创建一个
        path = os.path.dirname(os.path.abspath(fn))
        if not os.path.exists(path):
            os.makedirs(path)
        handler = logging.handlers.RotatingFileHandler(fn, maxBytes=size, backupCount=backup)
        formatter = logging.Formatter(fmt)
        handler.setFormatter(formatter)
        basename = os.path.basename(os.path.abspath(fn))
        print 'before==>', basename
        basename = basename.split('.')[0]
        print 'after==>', basename
        self.logger = logging.getLogger(basename)
        self.logger.addHandler(handler)
        if level == 'DEBUG':
            self.logger.setLevel(logging.DEBUG) #默认日志级别是DEBUG级别
        elif level == 'INFO':
            self.logger.setLevel(logging.INFO)
        elif level == 'WARNING':
            self.logger.setLevel(logging.WARNING)
        elif level == 'ERROR':
            self.logger.setLevel(logging.ERROR)
        elif level == 'CRITICAL':
            self.logger.setLevel(logging.CRITICAL)
        else:
            self.logger.setLevel(logging.DEBUG) #没指定日志级别就默认DEBUG级别

    #logging level:
    #logging.NOSET 0, logging.DEBUG 10, logging.INFO 20, logging.WARNING 30, logging.ERROR 40, logging.CRITICAL 50
    def write(self, level, msg):
        if level == 10:
            self.logger.debug(msg)
        elif level == 20:
            self.logger.info(msg)
        elif level == 30:
            self.logger.warning(msg)
        elif level == 40:
            self.logger.error(msg)
        elif level == 50:
            self.logger.critical(msg)
        else:
            self.logger.debug(msg)

'''
if __name__ == '__main__':
    fmt = '%(asctime)s [%(levelname)-8s][%(filename)s:%(lineno)-4d] %(message)s'
    size = 5 * 1024
    loger = CLog('./test.log', fmt, size, 5)
    loger.write(0, 'this is NOSET message')
    loger.write(10, 'this is DEBUG message')
    loger.write(20, 'this is INFO message')
    loger.write(30, 'this is WARNING message')
    loger.write(40, 'this is ERROR message')
    loger.write(50, 'this is CRITICAL message')
'''
