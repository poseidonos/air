file(GLOB_RECURSE SOURCE_FILES *.cpp)
add_library(air STATIC ${SOURCE_FILES})
target_include_directories(air PUBLIC ${CMAKE_SOURCE_DIR})
if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 9.0)
    message(STATUS "compile option -fconstexpr-ops-limit is used")
    target_compile_options(air PRIVATE -std=c++14 -Wall -Werror -g -O3 -fno-omit-frame-pointer -mavx2 -fPIC -fconstexpr-ops-limit=123456789)
else()
    message(STATUS "compile option -fconstexpr-loop-limit is used")
    target_compile_options(air PRIVATE -std=c++14 -Wall -Werror -g -O3 -fno-omit-frame-pointer -mavx2 -fPIC -fconstexpr-loop-limit=123456789)
endif()
target_link_libraries(air pthread)
install(TARGETS air
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
