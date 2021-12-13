cmake_minimum_required(VERSION 3.8)

# DO NOT FORGET TO CHANGE THE VERSION NUMBER AT `include/libdocscript/version.h'
# IF YOU CHANGED THEM HERE !!!
project(docscript VERSION 0.4.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_EXTENSIONS OFF)

include("src/CMakeLists.txt")

include_directories("include")

add_subdirectory(libs/CLI11)

add_library(libdocscript ${LIBDOCSCRIPT_SRC_FILES})
add_executable(docsir ${DOCSIR_SRC_FILES})

if(MSVC)
    target_compile_options(libdocscript PUBLIC /W4 /WX /permissive-)
    target_compile_options(docsir PUBLIC /W4 /WX /permissive-)
else()
    target_compile_options(libdocscript PUBLIC -Wall -Wextra -pedantic -pedantic-errors)
    target_compile_options(docsir PUBLIC -Wall -Wextra -pedantic -pedantic-errors)
endif()

target_link_libraries(docsir CLI11)
target_link_libraries(docsir libdocscript)