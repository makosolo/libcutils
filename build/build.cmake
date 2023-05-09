find_package(Git)
#获取git 版本号
execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --format="%h"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE  GIT_REPO_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE)

#获取git 分支
execute_process(COMMAND ${GIT_EXECUTABLE} branch
		COMMAND grep *
		COMMAND awk  "{print $2}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE  GIT_REPO_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

message(STATUS "GIT_REPOSITORY_BRANCH ${GIT_REPO_BRANCH}")
message(STATUS "GIT_REPOSITORY_VERSION ${GIT_REPO_VERSION}")

# function(REDEFINE_FILE_MACRO targetname)
#     #获取当前目标的所有源文件
#     get_target_property(SOURCE_FILE "${targetname}" SOURCES)
#     #遍历源文件
#     foreach(sourcefile ${SOURCE_FILE})
#         #获取当前源文件的编译参数
#         get_property(defs SOURCE "${sourcefile}"
#                 PROPERTY COMPILE_DEFINITIONS)
#         #获取当前文件的绝对路径
#         get_filename_component(filepath "${sourcefile}" NAME)
#         #将绝对路径中的项目路径替换成空,得到源文件相对于项目路径的相对路径
#         string(REPLACE ${PROJECT_SOURCE_DIR}/ "" relpath ${filepath})
#         #将我们要加的编译参数(__FILE__定义)添加到原来的编译参数里面
#         list(APPEND defs "__FILE__=\"${relpath}\"")
#         #重新设置源文件的编译参数
#         set_property(SOURCE "${sourcefile}" PROPERTY COMPILE_DEFINITIONS ${defs})
#     endforeach()
# endfunction()

#构建信息
string(TIMESTAMP BUILD_DATA "%Y-%m-%d")
string(TIMESTAMP BUILD_TIME "%H:%M:%S")
set(BUILD_VERSION ${GIT_REPO_VERSION})
set(BUILD_BRANCH ${GIT_REPO_BRANCH})
message(STATUS "build data ->" ${BUILD_DATA})
message(STATUS "build time ->" ${BUILD_TIME})

# read file
file(READ "versionHistory" ver)
string(REGEX MATCH "Version: ([0-99][.][0-99][.][0-99])" _ ${ver})
set(VERSION ${CMAKE_MATCH_1})
message("version: ${VERSION}")

string(REPLACE "." ";" VERSION_LIST ${VERSION})
list(GET VERSION_LIST 0 major)
list(GET VERSION_LIST 1 minor)
list(GET VERSION_LIST 2 revised)

if (major MATCHES "^[0-9]*$")
    set(VERSION_MAJOR ${major})
else()
    message(STATUS "major version invalid ->" ${major})
endif()

if (minor MATCHES "^[0-9]*$")
    set(VERSION_MINOR ${minor})
else()
    message(STATUS "minor version invalid ->" ${minor})
endif()

if (revised MATCHES "^[0-9]*$")
    set(VERSION_REVISED ${revised})
else()
    message(STATUS "revised version invalid ->" ${revised})
endif()
