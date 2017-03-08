// @file utils.h, 11/09/2015
// Copyright (c) 2015 ZhangAo, Inc. All rights reserved
//
// @author  Zhang Ao, zhangaoup@aliyun.com
// @brief   
// @version 1.0


#ifndef __UTILS_H__
#define __UTILS_H__

#include <sys/stat.h>

class Utils {
public:
    Utils() {}
    ~Utils() {}

    static int get_mtime(const char *fn, time_t &mt) {
        struct stat statbuf;
        int ret = stat(fn, &statbuf);
        if (0 == ret) {
            mt = statbuf.st_mtime;
            return 0;
        } else {
            return -1;
        }
    }

};  // class Utils

#endif  /*__UTILS_H__*/
