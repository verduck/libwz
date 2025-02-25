cmake_minimum_required(VERSION 3.16)

project(libwz C CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB SRC_FILES "src/*.cc")

find_package(ZLIB REQUIRED)

add_library(libwz SHARED ${SRC_FILES})
target_precompile_headers(libwz PRIVATE include/stdafx.h)
target_link_libraries(libwz PRIVATE ZLIB::ZLIB)