file(GLOB_RECURSE SOURCE_FILES *.cpp)
add_library(air STATIC ${SOURCE_FILES})
target_include_directories(air PUBLIC ${CMAKE_SOURCE_DIR})
target_compile_options(air PRIVATE -std=c++14 -Wall -Werror -g -O3 -fno-omit-frame-pointer -mavx2 -fPIC)
if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 9.0)
    message(STATUS "compile option -fconstexpr-ops-limit is used")
    add_compile_options(-fconstexpr-ops-limit=123456789)
else()
    message(STATUS "compile option -fconstexpr-loop-limit is used")
    add_compile_options(-fconstexpr-loop-limit=123456789)
endif()

target_link_libraries(air pthread)

if (ASAN)
    add_compile_options(-fsanitize=address)
    target_link_libraries(air -fsanitize=address)
    message(STATUS "address sanitizer added")
endif()

install(TARGETS air
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
