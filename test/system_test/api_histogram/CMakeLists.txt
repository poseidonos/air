file(GLOB_RECURSE SOURCE_FILES *.cpp)
add_executable(st_api_histogram ${SOURCE_FILES})
target_include_directories(st_api_histogram PUBLIC ${CMAKE_SOURCE_DIR})
target_compile_options(
    st_api_histogram PRIVATE -std=c++14 -Wall -Werror
    -g -O3 -fno-omit-frame-pointer -mavx2 -fPIC
)
target_link_libraries(st_api_histogram air pthread)
