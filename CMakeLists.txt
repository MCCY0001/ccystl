# 检查 CMake 版本，如果不满足要求，终止配置并给出错误信息
cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
project(ccystl)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 定义项目目录
set(MAIN_PROJECT_DIR ${CMAKE_SOURCE_DIR}/ccystl)
set(MAIN_PROJECT_TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(EXTERNAL_HEADER_DIR ${CMAKE_SOURCE_DIR}/include)
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

# 确保路径格式正确
file(TO_CMAKE_PATH "${BUILD_DIR}/lib" LIB_DIR)
file(TO_CMAKE_PATH "${BUILD_DIR}/bin" EXEC_DIR)

# 设置库和可执行文件的输出路径（推荐用法）
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXEC_DIR})

# 检查目录是否存在
if(NOT EXISTS ${MAIN_PROJECT_DIR})
    message(FATAL_ERROR "Main project directory ${MAIN_PROJECT_DIR} does not exist.")
endif()

if(NOT EXISTS ${MAIN_PROJECT_TEST_DIR})
    message(FATAL_ERROR "Test directory ${MAIN_PROJECT_TEST_DIR} does not exist.")
endif()

# 输出配置信息
message(STATUS "Main project directory: ${MAIN_PROJECT_DIR}")
message(STATUS "Test directory: ${MAIN_PROJECT_TEST_DIR}")
message(STATUS "External header directory: ${EXTERNAL_HEADER_DIR}")
message(STATUS "Build directory: ${BUILD_DIR}")
message(STATUS "Library output directory: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "Executable output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# 添加头文件搜索路径
include_directories(${MAIN_PROJECT_DIR})

# 添加子目录到构建过程
add_subdirectory(${MAIN_PROJECT_DIR})
add_subdirectory(${MAIN_PROJECT_TEST_DIR})
