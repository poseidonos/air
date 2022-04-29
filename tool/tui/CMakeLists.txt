file(GLOB_RECURSE SOURCE_FILES *.cpp)
add_executable(air_tui ${SOURCE_FILES})
target_include_directories(air_tui PUBLIC ${CMAKE_SOURCE_DIR})
target_compile_options(air_tui PRIVATE -std=c++14 -Wall -Werror -O3)
target_link_libraries(air_tui ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS air_tui
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
