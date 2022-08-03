file(GLOB_RECURSE SOURCE_FILES *.cpp)
add_executable(st_sparsely_log ${SOURCE_FILES})
target_include_directories(st_sparsely_log PUBLIC ${CMAKE_SOURCE_DIR})
target_compile_options(
    st_sparsely_log PRIVATE -std=c++14 -Wall -Werror
    -g -O3 -fno-omit-frame-pointer -mavx2 -fPIC
)
target_link_libraries(st_sparsely_log air pthread)

if (ASAN)
    add_compile_options(-fsanitize=address)
    target_link_libraries(st_sparsely_log -fsanitize=address)
endif()
