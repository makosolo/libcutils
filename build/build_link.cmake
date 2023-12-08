#交叉编译工具链信息
include(${PROJECT_SOURCE_DIR}/build/build.cmake)

#生成编译构建信息文件
configure_file (${PROJECT_SOURCE_DIR}/build/build_config.h.in
                ${PROJECT_SOURCE_DIR}/build/build_config.h)

#添加公共依赖的库
# set(APP_DEPEND_LIBS )

#添加针对平台依赖库平台添加依赖库
if(${PLATFORM} STREQUAL "tda4vh")
    include(${PROJECT_SOURCE_DIR}/build/build_tda4vh.cmake)
elseif(${PLATFORM} STREQUAL "j5")
    include(${PROJECT_SOURCE_DIR}/build/build_j5.cmake)
elseif(${PLATFORM} STREQUAL "imx-x11")
    include(${PROJECT_SOURCE_DIR}/build/build_imx-x11.cmake)
elseif(${PLATFORM} STREQUAL "x86")
    include(${PROJECT_SOURCE_DIR}/build/build_x86.cmake)
else()
    MESSAGE(FATAL_ERROR "unsupport compile platform")
endif()

set(APP_DEPEND_LIBS ${APP_DEPEND_LIBS} ${PLATFORM_DEPEND_LIBS})
