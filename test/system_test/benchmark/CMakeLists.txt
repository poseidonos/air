file(GLOB_RECURSE SOURCE_FILES *.cpp)
add_executable(st_benchmark ${SOURCE_FILES})
target_include_directories(st_benchmark PUBLIC ${CMAKE_SOURCE_DIR})
target_compile_options(
    st_benchmark PRIVATE -std=c++14 -Wall -Werror
    -g -O3 -fno-omit-frame-pointer -mavx2 -fPIC
)
target_link_libraries(st_benchmark air pthread benchmark::benchmark)
