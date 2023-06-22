cmake_minimum_required(VERSION 3.0)
project(utility)
aux_source_directory(./ SRC)

# 将源文件添加到库中
add_library(utility ${SRC})

