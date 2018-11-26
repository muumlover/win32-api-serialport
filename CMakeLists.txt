cmake_minimum_required(VERSION 3.12)
project(MyCpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")

include_directories("D:/TDM-GCC-64/x86_64-w64-mingw32/include")
link_directories("D:/TDM-GCC-64/x86_64-w64-mingw32/lib32" "D:/TDM-GCC-64/x86_64-w64-mingw32/lib")

add_executable(MyCpp main.cpp)