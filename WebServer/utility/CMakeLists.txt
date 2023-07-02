cmake_minimum_required(VERSION 3.0)

# project(utility)
aux_source_directory(./ SRC)
message("UTILITY : ${SRC}")
# 将源文件添加到库中
add_library(utility ${SRC})

target_link_libraries(utility pthread rt)
set_target_properties(utility PROPERTIES OUTPUT_NAME "server_base")

add_subdirectory(logtest)