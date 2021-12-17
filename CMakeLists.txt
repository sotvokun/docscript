cmake_minimum_required(VERSION 3.13)

# DO NOT FORGET TO CHANGE THE VERSION NUMBER AT `include/libdocscript/version.h'
# IF YOU CHANGED THEM HERE !!!
# 9999 = Development Pre-release
project(docscript VERSION 0.3.9999)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories("include")

add_subdirectory(src/docsir)
add_subdirectory(src/libdocscript)
add_subdirectory(libs/CLI11)

if(MSVC)
    target_compile_options(libdocscript PUBLIC /W4 /WX /permissive-)
    target_compile_options(docsir PUBLIC /W4 /WX /permissive-)
else()
    target_compile_options(libdocscript PUBLIC -Wall -Wextra -pedantic -pedantic-errors)
    target_compile_options(docsir PUBLIC -Wall -Wextra -pedantic -pedantic-errors)
endif()

target_link_libraries(docsir CLI11)
target_link_libraries(docsir libdocscript)