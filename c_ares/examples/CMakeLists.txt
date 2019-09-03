cmake_minimum_required(VERSION 3.13)
project(ahost)

include_directories("/root/file/tmp/c-ares-cares-1_1_0")
LINK_DIRECTORIES("/root/file/tmp/c-ares-cares-1_1_0")

add_executable(ahost ahost.c)
target_link_libraries(ahost /root/file/tmp/c-ares-cares-1_1_0/libcares.a)

