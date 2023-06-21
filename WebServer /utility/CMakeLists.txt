cmake_minimum_required(VERSION 3.0)

aux_source_directory(./ SRC)

# 将源文件添加到库中
add_library(utility ${SRC})
