// @file auto_file_loader.h, 11/02/2015
// Copyright (c) 2015 ZhangAo, Inc. All rights reserved
//
// @author  Zhang Ao, zhangaoup@aliyun.com
// @brief   auto-file-loader.
// @version 1.5


#ifndef __AUTO_FILE_LOADER_H__
#define __AUTO_FILE_LOADER_H__

#include <string>
#include <list>
#include <pthread.h>
#include <unistd.h>
#include "utils.h"

class LoaderBase {
public:
    virtual ~LoaderBase() {}

    virtual int init() = 0;

    virtual const char* filename() = 0;

    virtual time_t last_load_time() = 0;

    virtual int load() = 0;
};

template<class T>
class LoaderWorkerBase {
public:
    virtual ~LoaderWorkerBase() {}

    virtual int add_line(const char* line_input, T* data) = 0;

    virtual int create(T& data) = 0;

    virtual void free(T& data) = 0;
};

template<class T, class LoaderWorker>
class AutoFileLoader: public LoaderBase {
public:
    // @brief: init parse_line and free call back function, 
    //         and filename to be loaded. 
    //         set _buffer to NULL.
    // @param[in] parse_line: line parsing function
    // @param[in] free: function that used to desctroy resouce
    // @param[in] filename: file to be loaded
    AutoFileLoader(const char* loader_name,
            const char* filename,
            const LoaderWorker& lw):
        _loader_name(loader_name),
        _filename(filename),
        _buf_tag(0),
        _is_loading(false),
        _loader_worker(NULL) {
            pthread_mutex_init(&_load_tag_mutex, NULL);
            _loader_worker = new(std::nothrow) LoaderWorker(lw);
//            if (_loader_worker != NULL) {
//                *_loader_worker = lw;
//            }
        }

    // @brief free resouce
    ~AutoFileLoader() {
        _loader_worker->free(_buffer[_buf_tag]);
        if (_loader_worker != NULL) {
            delete _loader_worker;
            _loader_worker = NULL;
        }
    }

    // @brief initializing: load the file for the first time,
    //        and regist file update auto checking.
    // @return 0: init successfully
    //        -1: init failed
    int init();

    // @brief get current available buffer data
    // @return current available buffer, 
    //         or NULL if buffer is not ready(init() is not finished or failed).
    T* get() {
        if (!_ready) {
            return NULL;
        }
        return &_buffer[_buf_tag];
    }

    // @brief return the filename of this FileLoader
    // @return the filename in char pointer.
    const char* filename() {
        return _filename.c_str();
    }

    time_t last_load_time() {
        return _last_load_time;
    }

    bool is_loading() {
        pthread_mutex_lock(&_load_tag_mutex);
        bool tmp_load_tag = _is_loading;
        pthread_mutex_unlock(&_load_tag_mutex);
        return tmp_load_tag;
    }

private:
    int load();

//    int (*init_instance_callback)(T& data, ConfType& conf);
//    void (*free_instance_callback)(T& data);
//    int (*add_line_callback)(const char* line, T* data);

private:
    std::string _loader_name; // current loader name, only for logging
    std::string _filename;  // file to be loaded

    bool _ready;    // does buffer ready(first loading is finished)
    bool _buf_tag;  // index of buffer, 0 or 1

    //ConfType _conf;  // buffer creation config
    LoaderWorkerBase<T>* _loader_worker;

    T _buffer[2];   // data buffer for auto loader

    bool _is_loading;
    pthread_mutex_t _load_tag_mutex;
    time_t _last_load_time;
};  // class AutoFileLoader

class AutoReloader {
public:
    ~AutoReloader() {}

    static AutoReloader& instance() {
        static AutoReloader arl;  // Auto Re Loader
        return arl;
    }

    template<class T>
    int regist(T* afl) {
        if (afl == NULL) {
            return -1;
        }

        if (_file_loader_list.size() == 0) {
            pthread_create(&_thread_id, NULL, AutoReloader::auto_reloader, this);
        }

        update_lock();
        _file_loader_list.push_back(afl);
        update_unlock();
    }

    std::list<LoaderBase*>& file_loader_list() {
        return _file_loader_list;
    }

    void update_lock() {
        pthread_mutex_lock(&_update_mutex);
    }

    void update_unlock() {
        pthread_mutex_unlock(&_update_mutex);
    }

    static void* auto_reloader(void* param) {
        if (param == NULL) {
            return NULL;
        }
        AutoReloader& arl = *(AutoReloader*)param;
        while (true) {
            arl.update_lock();
            for (std::list<LoaderBase*>::iterator it = arl.file_loader_list().begin();
                    it != arl.file_loader_list().end();
                    ++it) {
                time_t file_update_time;
                int ret = Utils::get_mtime((**it).filename(), file_update_time);
                if (ret != 0 || file_update_time == (**it).last_load_time()) {
                    continue;
                }
                (**it).load();
            }
            arl.update_unlock();
            sleep(60);
        }
    }

private:
    //AutoReloader(): _update_mutex(PTHREAD_MUTEX_INITIALIZER) {}
    AutoReloader() {
        pthread_mutex_init(&_update_mutex, NULL);
    }

    std::list<LoaderBase*> _file_loader_list;

    pthread_t _thread_id;

    pthread_mutex_t _update_mutex;
};  // class AutoReloader

#include "auto_file_loader.hpp"

#endif  /*__AUTO_FILE_LOADER_H__*/
