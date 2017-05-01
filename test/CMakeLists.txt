cmake_minimum_required(VERSION 3.2)
project(ohmyjson-test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O3 -g -pedantic")


add_executable(test-parse test-parse.cpp)
add_test(NAME ohmyjson-parse COMMAND test-parse)

add_executable(test-iterate test-iterate.cpp)
add_test(NAME ohmyjson-iterate COMMAND test-iterate)

add_executable(test-jsonorg-data test-jsonorg-data.cpp read_file.h)
add_test(NAME ohmyjsonorgdata WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMAND test-jsonorg-data)


target_link_libraries(test-parse        ohmyjson)
target_link_libraries(test-iterate      ohmyjson)
target_link_libraries(test-jsonorg-data ohmyjson)



