cmake_minimum_required(VERSION 3.2)
project(ohmyjson)

add_library(ohmyjson INTERFACE)
set(headers ohmy/json.hpp ohmy/num.hpp ohmy/exceptions.hpp)

target_include_directories(ohmyjson INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/ohmy>
)

install(TARGETS ohmyjson EXPORT ohmyjson_export)
install(EXPORT ohmyjson_export DESTINATION lib/cmake/ohmyjson)
install(FILES ${headers} DESTINATION include/ohmy)
export(TARGETS ohmyjson FILE ${PROJECT_BINARY_DIR}/ohmyjson-targets.cmake)

# testing
option (BUILD_TESTS "Build tests"     OFF)
option (BUILD_BENCH "Build benchmark" OFF)

if(BUILD_TESTS OR BUILD_BENCH)

  include(cmake/json-data.cmake)
  enable_testing()

  if(BUILD_TESTS)
    add_subdirectory(test)
  endif()

  if(BUILD_BENCH)
    add_subdirectory(bench)
  endif()

endif()


# ide
add_custom_target(ohmyjson-headers SOURCES ${headers})
set_target_properties(ohmyjson-headers PROPERTIES LINKER_LANGUAGE CXX)
