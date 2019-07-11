#!/usr/bin/env python
#encoding: utf-8
#description: a MySQL client implemention

import MySQLdb

class MySQLClt:
    def __init__(self, host, user, pwd, charset='utf8'):
        self.host = host
        self.user = user
        self.pwd = pwd
        self.charset = charset
        try:
            self.conn = MySQLdb.connect(host=self.host, user=self.user, passwd=self.pwd)
            self.conn.set_character_set(self.charset)
            self.cur = self.conn.cursor()
        except MySQLdb.Error as e:
            print 'MySQL Error [%d]: %s' % (e.args[0], e.args[1])

    def __del__(self):
        try:
            self.cur.close()
            self.conn.close()
        except MySQLdb.Error as e:
            print 'MySQL Error [%d]: %s' % (e.args[0], e.args[1])
            pass

    def selectDB(self, db):
        try:
            self.conn.select_db(db)
        except MySQLdb.Error as e:
            print 'MySQL Error [%d]: %s' % (e.args[0], e.args[1])

    def query(self, sql):
        try:
            n = self.cur.execute(sql)
            return n
        except MySQLdb.Error as e:
            print 'MySQL Error [%d]: %s' % (e.args[0], e.args[1])

    def getRowCount(self):
        return self.cur.rowcount

    def queryOneRow(self, sql):
        self.query(sql)
        result = self.cur.fetchone()
        return result

    def queryAllRows(self, sql):
        self.query(sql)
        result = self.cur.fetchall()
        desc = self.cur.description
        d = []
        for inv in result:
            _d = {}
            for i in range(0, len(inv)):
                _d[desc[i][0]] = str(inv[i])
            d.append(_d)
        return d

    def insert(self, tbl_nm, data):
        for key in data:
            data[key] = "'""'" + str(data[key]) + "'"
        key = ','.join(data.keys())
        val = ','.join(data.values())
        sql = "INSERT INTO " + tbl_nm + " (" + key  + ") VALUES (" + val  + ") "
        return self.query(sql)

    def getLastInsertId(self):
        return self.cur.lastrowid

    def update(self, sql):
        try:
            result = self.cur.execute(sql)
            self.conn.commit()
        except MySQLdb.Error as e:
            print 'MySQL Error [%d]: %s' % (e.args[0], e.args[1])

    def commit(self):
        self.conn.commit()

'''
if __name__ == '__main__':
    #config here
    host = '127.0.0.1'
    user = 'root'
    passwd = 'root'

    db = MySQLClt(host, user, passwd)
    db.selectDB('dht')
    sql = 'SELECT * FROM `bt_main_new`'

    res = db.queryAllRows(sql)
    print res
'''
