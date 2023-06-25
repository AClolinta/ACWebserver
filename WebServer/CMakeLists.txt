# set(SRCS
#     main.cpp
# )
# get_filename_component(CURRENT_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
# message(${CURRENT_DIR})

aux_source_directory(./ SRCS)


include_directories(${PROJECT_SOURCE_DIR}/WebServer/utility)
# message("${SRCS}")

add_executable(WebServer ${SRCS})
target_link_libraries(WebServer libserver_base)


# add_subdirectory(base)
# add_subdirectory(tests)