cmake_minimum_required(VERSION 3.6.0)

message(STATUS "******************************************************************")
message(STATUS "CMAKE_SOURCE_DIR->" ${CMAKE_SOURCE_DIR})
message(STATUS "PROJECT_SOURCE_DIR->)" ../../..)
message(STATUS "******************************************************************")

project(libcutils)

find_package(Git)

message(STATUS "GIT_REPOSITORY_VERSION ${GIT_REPO_VERSION}")

set(CMAKE_CXX_STANDARD 11)

include(${PROJECT_SOURCE_DIR}/build/build_link.cmake)

add_compile_options(-fPIC -fstack-protector-strong)
add_compile_options(-Wall -fdata-sections -Wno-psabi -ffunction-sections -fstack-protector-all -fexceptions)

set(SRC_DIR
    src/utils_comm.c
    src/container/utils_list.c
    src/container/utils_cycqueue.c
    src/container/utils_cycbuf.c
    # src/container/utils_rbtree.c
    src/container/utils_map.c
    src/container/utils_array.c
    src/container/utils_heap.c
    src/string/utils_string.c
    src/platform/posix/utils_task.c
    src/platform/posix/utils_task_pool.c
    src/platform/posix/utils_mutex.c
    src/platform/posix/utils_event.c
    src/platform/posix/utils_queue.c
    src/platform/posix/utils_file.c
    src/platform/posix/utils_log.c
    src/platform/posix/utils_time.c
    src/utils/utils_counter.c
    src/utils/utils_bitops.c
    src/utils/utils_timer.c
    )

#头文件
include_directories(
                    .
                    src
                    include/cutils
                    include/cutils/container
                    include/cutils/string
                    include/cutils/utils
                    include/cutils/platform
                    )

#宏定义
add_definitions(-Wno-builtin-macro-redefined -g)

if(${BUILD_MODE} STREQUAL "debug")
    add_definitions("-Wall -g -O0 -Wno-unused-local-typedefs")
    add_definitions(-D__DEBUG__="1")
endif()

#源文件
add_library(cutils STATIC ${SRC_DIR})

target_link_libraries(cutils
    ${APP_DEPEND_LIBS}
    pthread
)
