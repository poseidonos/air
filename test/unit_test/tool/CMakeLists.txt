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
    message(STATUS "${Green}build unit test: tool${ColorReset}")
    subdirlist(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
    foreach(SUBDIR ${SUBDIRS})
        set(UT_BIN_NAME ut_${SUBDIR})
        file(GLOB_RECURSE SOURCE_FILES ${SUBDIR}/*.cpp)
        add_executable(${UT_BIN_NAME} ${SOURCE_FILES})
        target_include_directories(${UT_BIN_NAME} PUBLIC ${CMAKE_SOURCE_DIR})
        target_compile_options(
            ${UT_BIN_NAME} PRIVATE -std=c++14 -Wall -Werror
            -g -O1 -fno-omit-frame-pointer -fprofile-arcs -ftest-coverage
        )
        target_link_libraries(${UT_BIN_NAME} gtest gmock gcov pthread)
    endforeach()
endif()

if(RUN_UT)
message("${Green}Start Unit Test [tool]\n${ColorReset}")
    subdirlist(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
    set(tc_total_pass_count 0)

    foreach(SUBDIR ${SUBDIRS})
        execute_process(
            COMMAND ${CMAKE_SOURCE_DIR}/build/test/unit_test/tool/ut_${SUBDIR}
            --gtest_output=xml:tool/report_ut_${SUBDIR}.xml
            RESULT_VARIABLE ut_var_result
            OUTPUT_VARIABLE ut_var_output
            ERROR_VARIABLE ut_var_error
        )
        message("${ut_var_output}")

        if(NOT ut_var_result EQUAL 0)
            message("${Red}[run failed] ${ut_var_error}${ColorReset}")
            return()
        endif()

        string(FIND ${ut_var_output} "PASSED" startIndex)
        string(FIND ${ut_var_output} "test" endIndex REVERSE)
        math(EXPR startIndex "${startIndex}+10")
        math(EXPR length "${endIndex}-${startIndex}-1")
        string(SUBSTRING ${ut_var_output} ${startIndex} ${length} tc_module_pass_count)
        math(EXPR tc_total_pass_count "${tc_total_pass_count}+${tc_module_pass_count}")
    endforeach()

    message("${Green}Finish Unit Test [tool] Passed Count : ${tc_total_pass_count} Cases\n${ColorReset}")
endif()
