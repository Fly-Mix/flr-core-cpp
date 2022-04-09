cmake_minimum_required(VERSION 3.19)
project(flr-core VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 引入工程开发配置：是否采用源码方式引入库
include(cmake_dev.cmake)

# ------ 分割线 ------

# 工程产物存放目录
set(PRODUCTS_DIR "${PROJECT_SOURCE_DIR}/Products/flr-core-lib")
message(STATUS "flr-core products dir：${PRODUCTS_DIR}")

set(VERSION "0.1.0")

# 三方库存放目录
set(3RDPARTY_DIR "${PROJECT_SOURCE_DIR}/3rdparty")

# src files
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
file(GLOB_RECURSE SOURCE_FILES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.h")
#set(SOURCE_FILES
#        src/flr.cpp
#        src/constant.h
#        src/util/file_util.cpp
#        src/util/code_util.cpp
#        src/util/asset_util.cpp
#        src/util/array_util.cpp
#        )
message(STATUS "flr-core src files：${SOURCE_FILES}")

# public header files
set(PUBLIC_HEADER_DIR "${PROJECT_SOURCE_DIR}/include")
file(GLOB_RECURSE PUBLIC_HEADER_FILES "${PUBLIC_HEADER_DIR}/*.h")
message(STATUS "flr-core public header files：${PUBLIC_HEADER_FILES}")

set(PRIVATE_HEADER_FILES
        )

set(REQUIRED_LIBRARIES
        )

add_library(${PROJECT_NAME} STATIC
        ${SOURCE_FILES}
        )
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PUBLIC_HEADER_DIR})

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

# 每次执行cmake时，导出头文件
file(REMOVE_RECURSE ${PRODUCTS_DIR})
file(COPY ${PUBLIC_HEADER_DIR} DESTINATION ${PRODUCTS_DIR})
# 每次执行make完成编译后，导出头文件
add_custom_command(
        # custom_command 依附的目标
        TARGET ${PROJECT_NAME}
        # custom_command 的执行时机：PRE_BUILD or POST_BUILD
        POST_BUILD
        # COMMAND关键字指定要执行的具体命令
        COMMAND
            # copy_directory：拷贝Src目录（./include）下的所有内容到Target目录（./Products/flr-core/include）
            ${CMAKE_COMMAND} -E copy_directory ${PUBLIC_HEADER_DIR} "${PRODUCTS_DIR}/include"
)

# 每次执行make完成编译后，在 api 工程根目录为其依赖库（flr-core和其依赖库）创建 symbolic link
set(API_PROJ_DIR "${PROJECT_SOURCE_DIR}/api")
set(API_PROJ_LIBS_DIR "${API_PROJ_DIR}/api-libs")
add_custom_command(
        # custom_command 依附的目标
        TARGET ${PROJECT_NAME}
        # custom_command 的执行时机：PRE_BUILD or POST_BUILD
        POST_BUILD
        # COMMAND关键字指定要执行的具体命令
        # 创建 api 工程的依赖库存储目录：./api/libs
        COMMAND
            ${CMAKE_COMMAND} -E make_directory ${API_PROJ_LIBS_DIR}
        # 为 api 工程根目录的依赖库（flr-core和其依赖库）创建 symbolic link
        # 可以使用 ln 或者 CMAKE_COMMAND 的 create_symlink
#        COMMAND
#            ln -s ${PROJECT_SOURCE_DIR}/Products/flr-core ${API_PROJ_LIBS_DIR}/flr-core
        COMMAND
            ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/Products/flr-core-lib ${API_PROJ_LIBS_DIR}/flr-core
        COMMAND
            ${CMAKE_COMMAND} -E create_symlink ${3RDPARTY_DIR}/fmt-lib ${API_PROJ_LIBS_DIR}/fmt
        COMMAND
            ${CMAKE_COMMAND} -E create_symlink ${3RDPARTY_DIR}/rapidyaml-lib ${API_PROJ_LIBS_DIR}/ryml
)

# ------ 分割线 ------
if (${FLR_USE_SRC})
    # FLR_USE_SRC 为 True，源码方式引入三方库
    message(STATUS "FLR_USE_SRC 值为：${FLR_USE_SRC}，flr-core 工程以源码方式引入三方库")

    # rapidyaml
    add_subdirectory(${3RDPARTY_DIR}/rapidyaml ryml)
    target_link_libraries(${PROJECT_NAME} PUBLIC ryml)

    # fmt
    add_subdirectory(${3RDPARTY_DIR}/fmt fmt)
    target_link_libraries(${PROJECT_NAME} PUBLIC fmt)

else()
    # FLR_USE_SRC 为 False，二进制方式引入三方库
    message(STATUS "FLR_USE_SRC 值为：${FLR_USE_SRC}，flr-core 工程以二进制方式引入三方库")

    # rapidyaml
    include_directories("${3RDPARTY_DIR}/rapidyaml-lib/include")
    # 库构建，不会做 link；可执行文件构建，才会做 link，故此处不需要设置 link_directories
    # link_directories("${3RDPARTY_DIR}/rapidyaml-lib/lib")

    # fmt
    include_directories("${3RDPARTY_DIR}/fmt-lib/include")

endif ()


# ------ 分割线 ------
# 参考资料
# - 《C++工程：总结 CMake 添加第三方库依赖方式 find_library、FetchContent、CPM等》：https://www.jianshu.com/p/f181b5bd0a63
# - 《Cmake 脚本对项目输出路径和输出头文件的路径定义》：https://blog.csdn.net/fengjunwang1980/article/details/78272203
