if(BUILD_ST)
    message(STATUS "build system test")
    add_subdirectory(api_histogram)
    add_subdirectory(benchmark)
    add_subdirectory(c_interface)
    add_subdirectory(data_transfer)
    add_subdirectory(io_mock)
    add_subdirectory(latency_resolution)
    add_subdirectory(many_log)
    add_subdirectory(sparsely_log)
endif()

add_custom_target(
    build_st
    COMMAND cmake ../.. -DBUILD_ST=ON -DBUILD_UT=OFF -DRUN_UT=OFF -DCONFIG=${CMAKE_SOURCE_DIR}/config/system_test.cfg
    COMMAND make -j 8
)
