cmake_minimum_required(VERSION 3.6.0)

message(STATUS "******************************************************************")
message(STATUS "CMAKE_SOURCE_DIR->" ${CMAKE_SOURCE_DIR})
message(STATUS "PROJECT_SOURCE_DIR->)" ../../..)
message(STATUS "******************************************************************")

project(libcutils)

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
    src/container/utils_list.c
    # src/container/utils_queue.c
    src/container/utils_cycbuf.c
    src/container/utils_rbtree.c
    src/string/utils_string.c
    # src/platform/posix/utils_timer.c
    src/platform/posix/utils_task.c
    src/platform/posix/utils_mutex.c
    src/platform/posix/utils_event.c
    src/platform/posix/utils_queue.c
    src/platform/posix/utils_file.c
    src/platform/posix/utils_log.c
    src/utils/utils_counter.c
    )

#头文件
include_directories(
                    .
                    src
                    include/utils/container
                    include/utils/string
                    include/utils/utils
                    include/utils/platform/include
                    )

#宏定义
add_definitions(-Wno-builtin-macro-redefined -g)

if(${BUILD_MODE} STREQUAL "debug")
    add_definitions(-DNDEBUG)
endif()

#add_compile_options(-Wall -g)

#源文件
add_library(cutils SHARED ${SRC_DIR})

target_link_libraries(cutils
    pthread
)

add_compile_options(-fdata-sections -Wno-psabi -ffunction-sections -fstack-protector-all -frtti -fexceptions)
