cmake_minimum_required (VERSION 3.0)

project(melon)
INCLUDE_DIRECTORIES(inc )
INCLUDE_DIRECTORIES(src )
AUX_SOURCE_DIRECTORY(src DIR_SRCS)
SET(melon ${DIR_SRCS} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_compile_options(-std=c++11 -Wall -Wextra -Wsign-conversion)

ADD_EXECUTABLE(${PROJECT_NAME} ${melon})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} m crypto)

#FIND_LIBRARY(
#   LIBDB_CXX_LIBRARIES NAMES  boost_regex
#   PATHS /usr/lib/ /usr/local/lib/
#   )
#
#set(Headers
#        inc/ip138.hh
#        )
#
#set(Sources
#        src/ip138.cc
#        )
#add_subdirectory(src)
#add_executable(${EXECUTABLE_NAME} ${Sources} ${Headers})


