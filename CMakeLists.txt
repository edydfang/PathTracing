# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (rayTracing)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add OpenMP Support
find_package(OpenMP)
if (NOT(CMAKE_BUILD_TYPE STREQUAL "Debug") AND OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

configure_file(./res/ARU.jpg ./ARU.jpg COPYONLY)

# 指定生成目标
add_executable(rayTracing rayTracing.cpp)
