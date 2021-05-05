cmake_minimum_required(VERSION 3.19)
project(flr-core-example VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Products/bin)

# ------ 分割线 ------

# 引入静态库 flr-core
set(FLR_CORE_ENGINE_DIR ./..)
# 引入方案一：源码引入 flr-core
add_subdirectory(${FLR_CORE_ENGINE_DIR} flr-core)

# 引入方案二：二进制引入 flr-core
#include_directories("${FLR_CORE_ENGINE_DIR}/Products/flr-core-lib/include")
#link_directories("${FLR_CORE_ENGINE_DIR}/Products/flr-core-lib/lib")

# ------ 分割线 ------

# example for flr-core-engine
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC
        flr-core
        )

