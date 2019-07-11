#!/usr/bin/env python
#encoding: utf-8
#description: a FTP client implemention including dir list, files download, upload etc
#date: 2015-10-13
#note: must setup a ftp vsftpd server in advance

from ftplib import FTP, error_perm
import os, sys, string, datetime, time, socket


def debug_print(s):
    print s


class FTPClt:
    def __init__(self, host, user, pwd, remotedir, port=21):
        self.host = host
        self.port = int(port)
        self.user = user
        self.pwd = pwd
        if os.path.isdir(remotedir):
            self.remote_dir = os.path.abspath(remotedir)
        else:
            self.remote_dir = os.path.abspath(os.path.dirname(remotedir))
        self.ftp = FTP()
        self.file_list = []

    def __del__(self):
        self.ftp.close()

    def login(self):
        ftp = self.ftp
        try:
            timeout = 300
            socket.setdefaulttimeout(timeout)
            #设置passive模式...
            ftp.set_pasv(True)
            print u'starting to connect to FTP Server [%s:%d]' % (self.host, self.port)
            ftp.connect(self.host, self.port)
            print u'succeed to connect to %s' % (self.host)
            print u'start to login FTP Server %s' % (self.host)
            ftp.login(self.user, self.pwd)
            print u'succeed to login %s' % (self.host)
            debug_print(ftp.getwelcome())
        except Exception, e:
            print u'connect or login failed'
            print e
            return

        try:
            print '[remote dir]:%s' % self.remote_dir
            ftp.cwd(self.remote_dir)
        except Exception:
            print u'change dir failed'

    def get_filename(self, line):
        pos = line.rfind(':')
        while (line[pos] != ' '):
            pos += 1
        while (line[pos] == ' '):
            pos += 1
        file_arr = [line[0], line[pos:]]
        return file_arr

    def get_filelist(self, line):
        file_arr = self.get_filename(line)
        if file[1] not in ['.', '..']:
            self.file_list.append(file_arr)

    #比较远程和本地同名文件大小是否相同?相同返回true
    def compare_filesize(self, local_fn, remote_fn):
        try:
            remote_filesize = self.ftp.size(remote_fn)
        except Exception:
            remote_filesize = -1
        try:
            local_filesize = os.path.getsize(local_fn)
        except Exception:
            local_filesize = -1
        print remote_filesize, local_filesize
        debug_print('local file size:[%d], remote file size[%d]' % (local_filesize, remote_filesize),)
        return remote_filesize == local_filesize

    #下载远程文件,使用FTP.retrbinary()
    def download_file(self, local_fn, remote_fn):
        print '[local file:%s]<====[remote file:%s]'%(local_fn, remote_fn)
        if self.compare_filesize(local_fn, remote_fn):
            debug_print(u'%s has the same size, no need to download' % local_fn)
            return
        else:
            debug_print(u'============== starting to download %s ============' % local_fn)
        fh = open(local_fn, 'wb')
        self.ftp.retrbinary(u'RETR %s' %(remote_fn), fh.write)
        fh.close()


    #下载远程文件夹
    def download_files(self, local_dir='./', remote_dir='./'):
        print '[local dir:%s]<====[remote dir:%s]'%(local_dir, remote_dir)
        try:
            self.ftp.cwd(remote_dir)
        except Exception:
            debug_print(u'dir [%s] no exist, continuing ......' % remote_dir)
            return
        if not os.path.isdir(local_dir):
            os.makedirs(local_dir)
        debug_print(u'change dir [%s]' % self.ftp.pwd())
        dir_entries = []
        self.ftp.dir(remote_dir, dir_entries.append)
        files = [f.split(' ')[-1] for f in dir_entries if f.startswith('-')]
        dirs = [f.split(' ')[-1] for f in dir_entries if f.startswith('d')]
        for fn in files:
            local_fn = os.path.join(local_dir, fn)
            remote_fn = os.path.join(remote_dir, fn)
            print 'get file [%s] ==> [%s]' % (remote_fn, local_fn)
            self.download_file(local_fn, remote_fn)
        for d in dirs:
            local_d = os.path.join(local_dir, d)
            remote_d = os.path.join(remote_dir, d)
            print 'get dir [%s] ==> [%s]' % (remote_d, local_d)
            self.download_files(local_d, remote_d)
        self.ftp.cwd('..')
        debug_print(u'change back to parent dir %s' % self.ftp.pwd())

    #上传本地文件
    def upload_file(self, local_fn, remote_fn):
        print '[local file:%s]====>[remote file:%s]'%(local_fn, remote_fn)
        if not os.path.isfile(local_fn):
            return
        if self.compare_filesize(local_fn, remote_fn):
            debug_print(u'the same size, skip [%s]' %local_fn)
            return
        fh = open(local_fn, 'rb')
        self.ftp.storbinary('STOR %s' % remote_fn, fh)
        fh.close()
        debug_print(u'uploaded [%s]' %local_fn)

    #递归上传本地文件夹内的所有文件(可包括该级目录)到远程FTP服务器下面的目录中(不存在的子目录会直接创建)
    #注意: 涉及用户和组的权限, 不要手动在FTP服务器上直接创建,要通过FTP客户端远程操作
    def upload_files(self, local_dir='./', remote_dir='./'):
        print '[local dir:%s]====>[remote dir:%s]'%(local_dir, remote_dir)
        if not os.path.isdir(local_dir):
            return
        local_names = os.listdir(local_dir)
        #如果ftp上传文件夹不存在,先创建它,可以尝试先切换目录再做异常处理
        #注意:只能创建一级子目录,并不能创建多级子目录
        try:
            self.ftp.cwd(remote_dir)
        except error_perm:
            try:
                self.ftp.mkd(remote_dir)
            except error_perm:
                print 'Error: no authority to make dir'
                return
        for item in local_names:
            src = os.path.join(local_dir, item)
            dst = os.path.join(remote_dir, item)
            if os.path.isdir(src):
                print 'put dir [%s] ==> [%s]' % (src, dst)
                try:
                    self.ftp.mkd(dst)
                except Exception:
                    debug_print(u'dir [%s] already exist!' % dst)
                self.upload_files(src, dst)
            else:
                print 'put file [%s] ==> [%s]' % (src, dst)
                self.upload_file(src, dst)
        self.ftp.cwd('..')


#=============== main entry point =====================
'''
if __name__ == '__main__':
    #config here ...
    host = '10.10.110.158'
    port = 21 #必须是整型
    user = 'test'
    pwd = 'hankai123'
    local_dir = '/home/file/ftp'
    remote_dir = '/home/test'
    remote_dir2 = '/home/test/upload'

    ftp = FTPClt(host, user, pwd, remote_dir, port)
    ftp.login()
    #ftp.download_files(local_dir, remote_dir)
    ftp.upload_files(local_dir, remote_dir2)
'''
