cmake_minimum_required(VERSION 3.17)
project(main)
set(CMAKE_CXX_STANDARD 14)


include_directories(/usr/local/include)
link_directories(/usr/local/lib) #添加动态连接库的路径
link_libraries(event) #链接动态库   环境变量中
add_executable(main main.cpp)
