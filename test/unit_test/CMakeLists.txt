if(BUILD_UT OR RUN_UT)
    message(STATUS "build unit test")
    add_subdirectory(src)
    add_subdirectory(tool)
endif()

add_custom_target(
    build_ut
    COMMAND cmake ../.. -DBUILD_UT=ON -DRUN_UT=OFF -DCONFIG=${CMAKE_SOURCE_DIR}/config/unit_test.cfg
    COMMAND make -j 8
)

add_custom_target(
    run_ut
    COMMAND cmake ../.. -DBUILD_UT=OFF -DRUN_UT=ON
    COMMAND make -j 8
)
