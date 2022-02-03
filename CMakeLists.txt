cmake_minimum_required(VERSION 3.16)

SET(CMAKE_TOOLCHAIN_FILE "/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(libwz C CXX)

include_directories(include)
file(GLOB SRC_FILES "src/*.cc")

find_package(ZLIB REQUIRED)

add_library(libwz STATIC ${SRC_FILES})
target_precompile_headers(libwz PRIVATE include/stdafx.h)
target_link_libraries(libwz PRIVATE ZLIB::ZLIB)