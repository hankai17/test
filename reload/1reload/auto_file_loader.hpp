// @file auto_file_loader.hpp, 11/09/2015
// Copyright (c) 2015 ZhangAo, Inc. All rights reserved
//
// @author  Zhang Ao, zhangaoup@aliyun.com
// @brief   implementation of auto_file_loader
// @version 1.1


#ifndef __AUTO_FILE_LOADER_HPP__
#define __AUTO_FILE_LOADER_HPP__

#include <fstream>
#include <iostream>

template<class T, class LoaderWorker>
int AutoFileLoader<T, LoaderWorker>::init() {
    if (_loader_worker == NULL) {
        return -1;
    }

    int ret = load();
    if (ret != 0) {
        return -2;
    }

    _ready = true;

    ret = AutoReloader::instance().regist(this);
    if (ret != 0) {
        return -3;
    }

    return 0;
}

template<class T, class LoaderWorker>
int AutoFileLoader<T, LoaderWorker>::load() {
    pthread_mutex_lock(&_load_tag_mutex);
    _is_loading = true;
    pthread_mutex_unlock(&_load_tag_mutex);

    int ret_code = 0;

    T& to_update_buffer = _buffer[!_buf_tag];
    int ret = _loader_worker->create(to_update_buffer);
    if (ret != 0) {
        ret_code = -1;
        pthread_mutex_lock(&_load_tag_mutex);
        _is_loading = false;
        pthread_mutex_unlock(&_load_tag_mutex);
        return ret_code;
    }

    std::ifstream ifs(_filename.c_str());
    std::string line;
    while (getline(ifs, line)) {
        ret = _loader_worker->add_line(line.c_str(), &to_update_buffer);
        if (ret != 0) {
            std::cerr << "add line error: " << line << std::endl;
        }
    }
    ifs.close();

    _buf_tag = !_buf_tag;
    if (_ready) {
        _loader_worker->free(_buffer[!_buf_tag]);
    }

    ret = Utils::get_mtime(filename(), _last_load_time);

    pthread_mutex_lock(&_load_tag_mutex);
    _is_loading = false;
    pthread_mutex_unlock(&_load_tag_mutex);

    return ret_code;
}

#endif  /*__AUTO_FILE_LOADER_HPP__*/
