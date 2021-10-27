cmake_minimum_required(VERSION 3.20)

# Don't forge to change the version value in `include/docscript.h` 
# if you changed this version.
project(docscript VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

include_directories(AFTER "include")

set(LIB_SOURCE_FILES "src/libdocscript/tokenstream.cpp"
                     "src/libdocscript/parser.cpp")

set(DOCSIR_SOURCE_FILES "src/docsir/docsir.cpp"         # Entry point file
                        "src/docsir/value.cpp"
                        "src/docsir/stdproc.cpp"
                        "src/docsir/stdproc_num.cpp"
                        "src/docsir/stdproc_str.cpp"
                        "src/docsir/stdproc_list.cpp"
                        "src/docsir/stdproc_ext.cpp"
                        "src/docsir/interpreter.cpp"
)

add_library(libdocscript ${LIB_SOURCE_FILES})
add_executable(docsir ${DOCSIR_SOURCE_FILES})

target_link_libraries(docsir libdocscript)