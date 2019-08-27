cmake_minimum_required(VERSION 3.2)

include_directories("/root/file/muduo", "root/file/muduo/muduo_examples/1filetransfer")
add_executable(download /root/file/muduo/muduo_examples/1filetransfer/1.2download.cc)
target_link_libraries(download /root/file/build/release-install-cpp11/lib/libmuduo_net.a /root/file/build/release-install-cpp11/lib/libmuduo_base.a pthread)

