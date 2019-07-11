#!/usr/bin/env python
#encoding: utf-8
#description: a SFTP client implemention including dir list, files download, files upload etc
#date: 2015-10-12
#note: must install python paramiko module in advance

import paramiko, os
from stat import S_ISDIR

class SFTPClt:
    def __init__(self, host, user, pwd, remotedir, port=22):
        self.host = host
        self.port = int(port)
        self.user = user
        self.pwd = pwd
        self.remote_dir = remotedir
        try:
            t = paramiko.Transport((host, port))
            t.connect(username=user, password=pwd)
            sftp = paramiko.SFTPClient.from_transport(t)
        except Exception, e:
            print 'ssh %s@%s: %s' %(user, host, e)
            exit()
        self.ssh = t
        self.sftp = sftp

    def __del__(self):
        #close ssh session
        self.sftp.close()
        self.ssh.close()

    def download_file(self, remote_path, local_path):
        self.sftp.get(remote_path, local_path)

    def upload_file(self, local_path, remote_path):
        self.sftp.put(local_path, remote_path)

    # sftp version of os.walk
    def sftp_walk(self, remote_dir):
        root = remote_dir
        files = []
        folders = []
        for f in self.sftp.listdir_attr(remote_dir):
            if S_ISDIR(f.st_mode):
                folders.append(f.filename)
            else:
                files.append(f.filename)
        print (root, folders, files)
        #防止大目录结构时卡住
        yield root, folders, files
        for folder in folders:
            abs_path = os.path.join(root, folder)
            for x in self.sftp_walk(abs_path):
                yield x

    def download_files(self, remote_dir, local_dir):
        fpath, fname = os.path.split(remote_dir)
        self.sftp.chdir(fpath)
        parent = fname
        print parent
        try:
            os.mkdir(local_dir)
        except:
            pass
        for w in self.sftp_walk(parent):
            try:
                os.mkdir(os.path.join(local_dir, w[0]))
            except:
                pass
            for f in w[2]:
                local_fn = os.path.join(local_dir, w[0], f)
                remote_fn = os.path.join(w[0], f)
                self.download_file(remote_fn, local_fn)

    #这里要求本地文件夹末尾包含/结尾，不过我已经做了容错处理
    def upload_files(self, remote_dir, local_dir):
        #循环上传整个目录,目录最后不能有/
        if local_dir[-1] == '/':
            local_dir = local_dir[:-1]
        fpath, fname = os.path.split(local_dir)
        os.chdir(fpath)
        parent = fname
        print parent
        for w in os.walk(parent):
            print (w[0], w[1], w[2])
            remote_folder = os.path.join(remote_dir, w[0])
            try:
                self.sftp.mkdir(remote_folder)
            except:
                print 'creat directory [%s] failed' % remote_folder
                pass
            for f in w[2]:
                local_fn = os.path.join(w[0],f)
                remote_fn = os.path.join(remote_dir,w[0],f)
                print '[%s]==>[%s]' %(local_fn, remote_fn)
                self.upload_file(local_fn, remote_fn)

    def write_cmd(self, text, remote_fn):
        #将命令写入远程文件中,并更改权限
        self.sftp.open(remote_fn,'w').write(text)
        self.sftp.chmod(remote_fn, 755)

    def exec_cmd(self, cmd):
        #批量远程执行命令并返回命令的执行结果
        channel = self.ssh.open_session()
        channel.get_pty()
        channel.invoke_shell()
        channel.settimeout(20.0)
        resp = ''
        try:
            resp += channel.recv(1024)
        except:
            channel.send('\n')
            resp += channel.recv(1024)
        for line in cmd.split('\n'):
            channel.send(line.strip() + '\n')
            resp += channel.recv(1024)
        return resp

    #请使用这个函数版本
    def exec_cmd2(self, cmd):
        s = paramiko.SSHClient()
        s.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        s.connect(hostname=self.host, port=self.port, username=self.user, password=self.pwd)
        stdin, stdout, stderr = s.exec_command(cmd)
        print stdout.read()
        s.close()

#================ main entry point =====================
'''
if __name__ == '__main__':
    HOST = '10.10.110.158'
    PORT = 22
    USER = 'root'
    PWD = 'hankai123'
    REMOTE_DIR = '/root/sftp_demo/' #末尾/可有可无，因为如果文件名为空，在sftp.listdir_attr中默认为当前目录，为稳妥这时必须设置chdir命令
    REMOTE_DIR2 = '/tmp' #必须是已经存在的目录，/可有可没有
    LOCAL_DIR = '/usr/local/src/sftp_demo/' #末尾不能有/

    sftpd = SFTPClt(HOST, USER, PWD, REMOTE_DIR, PORT)
    #sftpd.download_files(REMOTE_DIR, LOCAL_DIR)
    #print 'download all files in %s' %REMOTE_DIR
    #sftpd.upload_files(REMOTE_DIR2, LOCAL_DIR)
    #print 'upload all files in %s' %LOCAL_DIR
    data = 'service ats restart'
    fn = '/tmp/sftp_demo.txt'
    sftpd.write_cmd(data, fn)
    cmd = 'cd /usr/local/src\nls -la\nps auxf | grep traffic'
    result = sftpd.exec_cmd(cmd)
    print result
    cmd2 = 'free -m;df -hT;ps auxf| grep traffic'
    sftpd.exec_cmd2(cmd2)
'''
