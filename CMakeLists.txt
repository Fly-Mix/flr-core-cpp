cmake_minimum_required(VERSION 3.19)
project(flr-core VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ------ 分割线 ------

set(PRODUCTS_DIR
        "${PROJECT_SOURCE_DIR}/Products/flr-core-lib"
        )

set(VERSION
        "0.1.0"
        )

set(SOURCE_FILES
        src/flr.cpp
        src/constant.h
        src/util/file_util.cpp
        src/util/code_util.cpp
        src/util/asset_util.cpp
        src/util/array_util.cpp
        )

set(PUBLIC_HEADER_FILES
        include/flr-core
        )

set(PRIVATE_HEADER_FILES
        )

set(REQUIRED_LIBRARIES
        )

add_library(${PROJECT_NAME} STATIC
        ${SOURCE_FILES}
        )
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

# 设置target属性
# - ARCHIVE_OUTPUT_DIRECTORY：指定归档的输出目录
# - LIBRARY_OUTPUT_DIRECTORY：指定静态库的输出目录
SET_TARGET_PROPERTIES(
        ${PROJECT_NAME}
        PROPERTIES
            FRAMEWORK OFF
            SOVERSION 0
            VERSION ${VERSION}
            PUBLIC_HEADER "${PUBLIC_HEADER_FILES}"
            PRIVATE_HEADER "${PRIVATE_HEADER_FILES}"
            ARCHIVE_OUTPUT_DIRECTORY "${PRODUCTS_DIR}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${PRODUCTS_DIR}/lib"
            OUTPUT_NAME  ${PROJECT_NAME}
)

# 头文件输出
file(COPY  ${PUBLIC_HEADER_FILES} DESTINATION  "${PRODUCTS_DIR}/include")

# ------ 分割线 ------
# 三方库
set(3RDPARTY_DIR
        "${PROJECT_SOURCE_DIR}/3rdparty"
        )
# 源码引入三方库-rapidyaml
add_subdirectory(${3RDPARTY_DIR}/rapidyaml ryml)
target_link_libraries(${PROJECT_NAME} PUBLIC ryml)

# 源码引入三方库-glog
add_subdirectory(${3RDPARTY_DIR}/glog glog)
target_link_libraries(${PROJECT_NAME} PUBLIC glog)

# 源码引入三方库-fmt
add_subdirectory(${3RDPARTY_DIR}/fmt fmt)
target_link_libraries(${PROJECT_NAME} PUBLIC fmt)
# ------ 分割线 ------
# 参考资料
# - 《C++工程：总结 CMake 添加第三方库依赖方式 find_library、FetchContent、CPM等》：https://www.jianshu.com/p/f181b5bd0a63
# - 《Cmake 脚本对项目输出路径和输出头文件的路径定义》：https://blog.csdn.net/fengjunwang1980/article/details/78272203
