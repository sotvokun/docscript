cmake_minimum_required(VERSION 3.20)

# DO NOT FORGET TO CHANGE THE VERSION NUMBER AT `include/docsir/version.h'
# IF YOU CHANGED THEM HERE !!!
project(docsir VERSION 0.3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

include_directories("include")
include_directories("libs/cli11/include")

set(SRC_FILES 
    "src/docsir.cpp"
)

add_executable(docsir ${SRC_FILES})