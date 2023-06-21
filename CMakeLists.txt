cmake_minimum_required(VERSION 3.0)

project(WebServer CXX)

set(CXX_FLAGS
    -g
    -Wall
    -std=c++17
    -D_PTHREADS
    -Wno-unused-parameter
)

add_subdirectory(WebServer)
