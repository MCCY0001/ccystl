# 检查 LIB_DIR 变量是否定义
if(NOT DEFINED LIB_DIR)
    message(FATAL_ERROR "LIB_DIR is not defined. Please set the LIB_DIR variable.")
endif()

# 链接库目录
link_directories(${LIB_DIR})

# 检查 MAIN_PROJECT_DIR 变量是否定义
if(NOT DEFINED MAIN_PROJECT_DIR)
    message(FATAL_ERROR "MAIN_PROJECT_DIR is not defined. Please set the MAIN_PROJECT_DIR variable.")
endif()

# 定义各个模块的目录
set(ADAPTER_DIR ${MAIN_PROJECT_DIR}/adapter)
set(ALGORITHM_DIR ${MAIN_PROJECT_DIR}/algorithm)
set(ALLOCATOR_DIR ${MAIN_PROJECT_DIR}/allocator)
set(CONTAINER_DIR ${MAIN_PROJECT_DIR}/container)
set(FUNCTOR_DIR ${MAIN_PROJECT_DIR}/functor)
set(ITERATOR_DIR ${MAIN_PROJECT_DIR}/iterator)
set(UTILS_DIR ${MAIN_PROJECT_DIR}/utils)

# 定义一个宏，用于添加子目录并检查目录是否存在
macro(add_subdirectory_safe dir)
    if(EXISTS ${dir})
        add_subdirectory(${dir})
    else()
        message(WARNING "Directory ${dir} does not exist. Skipping...")
    endif()
endmacro()

# 添加各个模块的子目录
add_subdirectory_safe(${ADAPTER_DIR})
add_subdirectory_safe(${ALGORITHM_DIR})
add_subdirectory_safe(${ALLOCATOR_DIR})
add_subdirectory_safe(${CONTAINER_DIR})
add_subdirectory_safe(${FUNCTOR_DIR})
add_subdirectory_safe(${ITERATOR_DIR})
add_subdirectory_safe(${UTILS_DIR})
