if(BUILD_ST)
    message(STATUS "build system test")
    add_subdirectory(c_interface)
    add_subdirectory(io_mock)
    add_subdirectory(many_log)
endif()

add_custom_target(build_st DEPENDS build_system_test)
add_custom_command(
    OUTPUT build_system_test
    COMMAND cmake ../.. -DBUILD_ST=ON -DCONFIG=${CMAKE_SOURCE_DIR}/config/system_test.cfg
    COMMAND make -j 8
)
