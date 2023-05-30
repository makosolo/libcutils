cmake_minimum_required(VERSION 3.6.0)

message(STATUS "******************************************************************")
message(STATUS "CMAKE_SOURCE_DIR->" ${CMAKE_SOURCE_DIR})
message(STATUS "PROJECT_SOURCE_DIR->)" ../../..)
message(STATUS "******************************************************************")

project(libutils)

find_package(Git)

message(STATUS "GIT_REPOSITORY_VERSION ${GIT_REPO_VERSION}")

set(CMAKE_CXX_STANDARD 11)

# SET(CMAKE_C_COMPILER "/opt/tools/aos_hw/SDK/aos/arm64/common/bin/clang")
# SET(CMAKE_CXX_COMPILER "/opt/tools/aos_hw/SDK/aos/arm64/common/bin/clang++")
# SET(CMAKE_C_COMPILER "/opt/tools/rootfs/TDA4VH/rtos_sdk_j784s4_0802/cgtools/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc")
# SET(CMAKE_CXX_COMPILER "/opt/tools/rootfs/TDA4VH/rtos_sdk_j784s4_0802/cgtools/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-g++")

SET(CMAKE_C_COMPILER "gcc")
SET(CMAKE_CXX_COMPILER "g++")

set(SRC_DIR
    src/container/utils_list.c
    # src/container/utils_queue.c
    src/container/utils_cycbuf.c
    src/container/utils_rbtree.c
    src/string/utils_string.c
    )

#头文件
include_directories(
                    . 
                    src
                    src/container
                    src/string
                    src/utils
                    )

#宏定义
add_definitions(-Wno-builtin-macro-redefined -g)

if(${BUILD_MODE} STREQUAL "debug")
    add_definitions(-DNDEBUG)
endif()

#add_compile_options(-Wall -g)

#源文件
add_library(utils SHARED ${SRC_DIR})

add_compile_options(-fdata-sections -Wno-psabi -ffunction-sections -fstack-protector-all -frtti -fexceptions)
