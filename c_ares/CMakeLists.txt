cmake_minimum_required(VERSION 3.12)
project(ahost)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")


add_executable(ahost
        ahost.c
        ares.h
        ares.c
    ares_gethostbyname.c
    ares_init.c
    ares_process.c
        )

TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread) #must blow add_excutable!
