cmake_minimum_required(VERSION 3.20)

project(docscript)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

add_subdirectory("libdocscript")
add_subdirectory("docsir")

target_include_directories(docsir PUBLIC "${libdocscript_SOURCE_DIR}/include")
target_link_libraries(docsir libdocscript)