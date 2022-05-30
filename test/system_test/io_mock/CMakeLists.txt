file(GLOB_RECURSE SOURCE_FILES *.cpp)
add_executable(st_io_mock ${SOURCE_FILES})
target_include_directories(st_io_mock PUBLIC ${CMAKE_SOURCE_DIR})
target_compile_options(
    st_io_mock PRIVATE -std=c++14 -Wall -Werror
    -g -O3 -fno-omit-frame-pointer -mavx2 -fPIC
)
target_link_libraries(st_io_mock air pthread)
