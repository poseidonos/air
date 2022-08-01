file(GLOB_RECURSE SOURCE_FILES *.c)
add_executable(st_c_interface ${SOURCE_FILES})
target_include_directories(st_c_interface PUBLIC ${CMAKE_SOURCE_DIR})
target_compile_options(
    st_c_interface PRIVATE -Wall -Werror
    -g -O3 -fno-omit-frame-pointer -mavx2 -fPIC
)
target_link_libraries(st_c_interface air stdc++ pthread)

if (ASAN)
    add_compile_options(-fsanitize=address)
    target_link_libraries(st_c_interface -fsanitize=address)
endif()
