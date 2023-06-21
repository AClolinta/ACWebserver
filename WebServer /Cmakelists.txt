cmake_minimum_required(VERSION 3.0)

get_filename_component(CURRENT_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
include_directories( ${CURRENT_DIR}/src )

add_executable(WebServer ${SRCS})