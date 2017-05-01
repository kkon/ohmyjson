set(json_test_suite_data ${CMAKE_BINARY_DIR}/data)

if(NOT EXISTS ${json_test_suite_data})
  include(ExternalProject)
  externalproject_add(
    jsonorg_data_ext
    PREFIX ${CMAKE_BINARY_DIR}/external/json-data
    URL https://github.com/kkon/json-data/archive/master.zip
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    BUILD_IN_SOURCE 0
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR> ${json_test_suite_data}
    UPDATE_COMMAND ""
  )

  add_dependencies(ohmyjson jsonorg_data_ext)
endif()
