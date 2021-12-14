macro(subdirlist result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            list(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

if(BUILD_UT)
    message(STATUS "build unit test: tool")
    subdirlist(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
    foreach(SUBDIR ${SUBDIRS})
        set(UT_BIN_NAME ut_${SUBDIR})
        file(GLOB_RECURSE SOURCE_FILES ${SUBDIR}/*.cpp)
        add_executable(${UT_BIN_NAME} ${SOURCE_FILES})
        target_include_directories(${UT_BIN_NAME} PUBLIC ${CMAKE_SOURCE_DIR})
        target_compile_options(
            ${UT_BIN_NAME} PRIVATE -std=c++14 -Wall -Werror
            -g -fprofile-arcs -ftest-coverage
        )
        target_link_libraries(${UT_BIN_NAME} gtest gcov pthread)
    endforeach()
endif()

if(RUN_UT)
    message(STATUS "run unit test: tool")
    subdirlist(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
    foreach(SUBDIR ${SUBDIRS})
        execute_process(
            COMMAND ${CMAKE_SOURCE_DIR}/build/test/unit_test/tool/ut_${SUBDIR}
            --gtest_output=xml:tool/report_ut_${SUBDIR}.xml
        )
    endforeach()
endif()
