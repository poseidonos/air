file(GLOB_RECURSE SOURCE_FILES *.cpp)
add_executable(st_latency_resolution ${SOURCE_FILES})
target_include_directories(st_latency_resolution PUBLIC ${CMAKE_SOURCE_DIR})
target_compile_options(
    st_latency_resolution PRIVATE -std=c++14 -Wall -Werror
    -g -O3 -fno-omit-frame-pointer -mavx2 -fPIC
)
target_link_libraries(st_latency_resolution air pthread)
