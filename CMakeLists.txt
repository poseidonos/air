cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")

project(
    AIR
    VERSION 0.8.2
    DESCRIPTION "Analytics In Real-time"
    LANGUAGES CXX C
)

find_package(benchmark)
find_package(Threads)

execute_process(COMMAND bash -c "rm -rf ${CMAKE_SOURCE_DIR}/build/tool/c_interface_gen")
execute_process(COMMAND bash -c "rm -rf ${CMAKE_SOURCE_DIR}/build/src")

if(DEFINED CONFIG)
    message(STATUS "CONFIG is defined: ${CONFIG}")
    set(AIR_CFG ${CONFIG})
else()
    message(STATUS "CONFIG is not defined, default value is set")
    set(AIR_CFG ${CMAKE_SOURCE_DIR}/config/system_test.cfg)
endif()
message(STATUS "AIR_CFG: ${AIR_CFG}")
add_definitions(-DAIR_CFG=${AIR_CFG})

if(DEFINED ASAN)
    set(ASAN ${ASAN})
else()
    set(ASAN FALSE)
endif()
message(STATUS "ASAN: ${ASAN}")

add_subdirectory(test)
add_subdirectory(tool)
add_subdirectory(src)
add_dependencies(air run_c_interface_gen)

install(FILES src/api/Air.h src/api/Air_c.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/air)
