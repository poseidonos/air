file(GLOB_RECURSE SOURCE_FILES *.cpp)
add_executable(air_cli ${SOURCE_FILES})
target_include_directories(air_cli PUBLIC ${CMAKE_SOURCE_DIR})
target_compile_options(air_cli PRIVATE -std=c++14 -Wall -Werror -O3)
target_link_libraries(air_cli ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS air_cli
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
