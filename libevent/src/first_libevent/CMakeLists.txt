cmake_minimum_required(VERSION 3.17)
project(first_libevent)

set(CMAKE_CXX_STANDARD 14)


link_directories(/usr/local/lib) #添加动态连接库的路径
target_link_libraries(first_libevent -levent  ) #添加libmxnet.so

add_executable(first_libevent main.cpp)