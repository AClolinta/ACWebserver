
aux_source_directory(./ SRCS)

message("${SRCS}")
# message(${PROJECT_SOURCE_DIR}/WebServer/utility)

include_directories(${PROJECT_SOURCE_DIR}/WebServer/utility)
add_executable(WebServer ${SRCS})


target_link_libraries(WebServer utility)


add_subdirectory(utility)
add_subdirectory(TestDemo)