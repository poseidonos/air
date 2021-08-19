file(GLOB_RECURSE SOURCE_FILES *.c)
add_executable(st_c_interface ${SOURCE_FILES})
target_include_directories(st_c_interface PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(st_c_interface air stdc++ pthread)
