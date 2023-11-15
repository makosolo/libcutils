cmake_minimum_required(VERSION 3.6.0)

message(STATUS "******************************************************************")
message(STATUS "CMAKE_SOURCE_DIR->" ${CMAKE_SOURCE_DIR})
message(STATUS "PROJECT_SOURCE_DIR->)" ../../..)
message(STATUS "******************************************************************")

project(testUtils)

find_package(Git)

message(STATUS "GIT_REPOSITORY_VERSION ${GIT_REPO_VERSION}")

set(CMAKE_CXX_STANDARD 11)

if(${PLATFORM} STREQUAL "linux")
    SET(CMAKE_C_COMPILER "gcc")
    SET(CMAKE_CXX_COMPILER "g++")

elseif(${PLATFORM} STREQUAL "tda4vh")
    SET(CMAKE_C_COMPILER "/opt/tools/rootfs/TDA4VH/rtos_sdk_j784s4_0802/cgtools/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc")
    SET(CMAKE_CXX_COMPILER "/opt/tools/rootfs/TDA4VH/rtos_sdk_j784s4_0802/cgtools/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-g++")

elseif(${PLATFORM} STREQUAL "imx-x11")
    SET(CMAKE_C_COMPILER "/opt/tools/fsl-imx-x11/4.1.15-2.1.0/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-gcc")
    SET(CMAKE_CXX_COMPILER "/opt/tools/fsl-imx-x11/4.1.15-2.1.0/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-g++")

else()
    MESSAGE(FATAL_ERROR "unsupport compile platform")

endif()

set(SRC_DIR 
    main.c
    test_counter.c
    test_cycbuf.c
    test_list.c
    test_rbtree.c
    test_heap.c
    test_string.c
    test_task.c
    test_mutex.c
    test_event.c
    test_queue.c
    test_timer.c
    test_log.c
    test_map.c
    test_array.c)

#头文件
include_directories(../include/utils)

#链接库
link_directories(../out/)

#宏定义
add_definitions(-Wno-builtin-macro-redefined -g)

if(${BUILD_MODE} STREQUAL "debug")
    add_definitions(-DNDEBUG)
endif()

#add_compile_options(-Wall -g)

# test
add_executable(testUtils ${SRC_DIR})

target_link_libraries(testUtils cutils)

add_compile_options(-fdata-sections -Wno-psabi -ffunction-sections -fstack-protector-all -frtti -fexceptions)
