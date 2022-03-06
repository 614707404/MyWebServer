cmake_minimum_required(VERSION 3.20)
project(MyWebServer)

set(CMAKE_CXX_STANDARD 14)
#add_executable(main.cpp src/web_server.cpp src/web_server.h)
add_executable(MyWebServer main.cpp src/httpparser.h src/httpparser.cpp)