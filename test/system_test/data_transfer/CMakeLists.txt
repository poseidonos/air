file(GLOB_RECURSE SOURCE_FILES *.cpp)
add_executable(st_data_transfer ${SOURCE_FILES})
target_include_directories(st_data_transfer PUBLIC ${CMAKE_SOURCE_DIR})
target_compile_options(
    st_data_transfer PRIVATE -std=c++14 -Wall -Werror
    -g -O3 -fno-omit-frame-pointer -mavx2 -fPIC
)
target_link_libraries(st_data_transfer air pthread)

if (ASAN)
    add_compile_options(-fsanitize=address)
    target_link_libraries(st_data_transfer -fsanitize=address)
endif()
