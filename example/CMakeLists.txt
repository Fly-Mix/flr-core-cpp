cmake_minimum_required(VERSION 3.19)
project(flr-core-example VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Products/bin)

# 引入工程开发配置：是否采用源码方式引入库
include(${PROJECT_SOURCE_DIR}/../cmake_dev.cmake)

# ------ 依赖库引入声明 ------
set(FLR_CORE_ENGINE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
set(3RDPARTY_DIR "${FLR_CORE_ENGINE_DIR}/3rdparty")
message(STATUS "FLR_CORE_ENGINE_DIR：${FLR_CORE_ENGINE_DIR}\n3RDPARTY_DIR：${3RDPARTY_DIR}")

# 引入三方库 glog
add_subdirectory(${3RDPARTY_DIR}/glog glog)

# 引入二方库 flr-core
if (${FLR_USE_SRC})
    # FLR_USE_SRC 为 True，源码方式引入 flr-core 和其三方库
    message(STATUS "FLR_USE_SRC 值为：${FLR_USE_SRC}，example 工程以源码方式引入 flr-core 和其三方库")

    # 引入方案一：源码引入 flr-core
    add_subdirectory(${FLR_CORE_ENGINE_DIR} flr-core)
else()
    # FLR_USE_SRC 为 False，二进制方式引入 flr-core 和其三方库
    message(STATUS "FLR_USE_SRC 值为：${FLR_USE_SRC}，example 工程以二进制方式引入 flr-core 和其三方库")

    # flr-core
    include_directories("${FLR_CORE_ENGINE_DIR}/Products/flr-core-lib/include")
    link_directories("${FLR_CORE_ENGINE_DIR}/Products/flr-core-lib/lib")

    # rapidyaml
    include_directories("${3RDPARTY_DIR}/rapidyaml-lib/include")
    link_directories("${3RDPARTY_DIR}/rapidyaml-lib/lib")

    # fmt
    include_directories("${3RDPARTY_DIR}/fmt-lib/include")
    link_directories("${3RDPARTY_DIR}/fmt-lib/lib")

endif ()



# ------ 工程编译链接 ------

# example for flr-core-engine
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC glog)

if (${FLR_USE_SRC})
    target_link_libraries(${PROJECT_NAME} PUBLIC
            flr-core
            )
else()
    target_link_libraries(${PROJECT_NAME} PUBLIC
            libflr-core.a
            libryml.a
            libfmt.a
            )
endif()
