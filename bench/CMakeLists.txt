cmake_minimum_required(VERSION 3.2)
project(ohmyjson-bench)


add_executable(ohmyjson-bench bench.cpp bench.h)
#add_test(NAME ohmy-bench WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMAND ohmyjson-bench)

target_link_libraries(ohmyjson-bench ohmyjson)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -std=c++14 -pedantic")
set_property(TARGET ohmyjson-bench PROPERTY CXX_STANDARD 14)


# Benchmark vs rapidjson

include(ExternalProject)
externalproject_add(
  rapidjson_ext
  PREFIX "${PROJECT_BINARY_DIR}/external/rapidjson"
  URL "https://github.com/miloyip/rapidjson/archive/master.zip"
  BUILD_IN_SOURCE 0
  INSTALL_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)

add_dependencies(ohmyjson-bench rapidjson_ext)
externalproject_get_property(rapidjson_ext SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)
