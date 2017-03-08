// @file demo.cpp, 11/09/2015
// Copyright (c) 2015 ZhangAo, Inc. All rights reserved
//
// @author  Zhang Ao, zhangaoup@aliyun.com
// @brief   demo of using auto-file-loader
// @version 2.0

#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <map>
#include <string>

#include "auto_file_loader.h"

// type of data to be loaded
typedef std::map<std::string, int> load_data_type_t;

class LoaderWorker: public LoaderWorkerBase<load_data_type_t> {
public:
    LoaderWorker(int hash_size): _hash_size(hash_size) {}

    LoaderWorker(const LoaderWorker& other) {
        _hash_size = other.hash_size();
    }

    int add_line(const char* line_input, load_data_type_t* data) {
        std::string line = line_input;
        size_t sep_pos = line.find('\t');
        if (sep_pos == std::string::npos) {
            return -1;
        }
        std::string key = line.substr(0, sep_pos);
        int value = strtol(line.substr(sep_pos + 1).c_str(), NULL, 10);

        (*data)[key] = value;
        return 0;
    }

    int create(load_data_type_t& data) {
        // create with _hash_size
        return 0;
    }

    void free(load_data_type_t& data) {
        data.clear();
    }

    int hash_size() const {
        return _hash_size;
    }

private:
    int _hash_size;
};

int main(int argc, const char *argv[])
{
    LoaderWorker lw(100);
    AutoFileLoader<load_data_type_t, LoaderWorker> afl("afl_test", "data/test.in", lw);
    int ret = afl.init();
    if (ret != 0) {
        std::cerr << "init AutoFileLoader failed with code " << ret << std::endl;
        return -1;
    }

    while (true) {
        load_data_type_t* data = afl.get();
        if (data == NULL) {
            std::cerr << "get loader buffer errro!" << std::endl;
            break;
        }
        std::cerr << "print loaded data:" << std::endl;
        std::cerr << "data size: " << data->size() << std::endl;
        for (load_data_type_t::iterator it = data->begin();
                it != data->end();
                ++it) {
            std::cout << it->first << " |\\t| " << it->second << std::endl;
            std::cout << std::flush;
        }
        sleep(10);
    }
    return 0;
}
