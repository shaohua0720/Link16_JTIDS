cmake_minimum_required(VERSION 3.10)
project(Link16 VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 添加包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/lib
)

# 添加源文件
file(GLOB_RECURSE CORE_SOURCES 
    "src/core/*.cpp"
)

file(GLOB_RECURSE PROTOCOL_SOURCES 
    "src/protocol/*.cpp"
)

file(GLOB_RECURSE CODING_SOURCES 
    "src/coding/*.cpp"
)

file(GLOB_RECURSE PHYSICAL_SOURCES 
    "src/physical/*.cpp"
)

file(GLOB_RECURSE SIMULATION_SOURCES 
    "src/simulation/*.cpp"
)

file(GLOB_RECURSE APPLICATION_SOURCES 
    "src/application/*.cpp"
)

# 创建库
add_library(link16_core STATIC ${CORE_SOURCES})
add_library(link16_protocol STATIC ${PROTOCOL_SOURCES})
add_library(link16_coding STATIC ${CODING_SOURCES})
add_library(link16_physical STATIC ${PHYSICAL_SOURCES})
add_library(link16_simulation STATIC ${SIMULATION_SOURCES})

# 创建可执行文件
add_executable(link16_app ${APPLICATION_SOURCES})

# 链接库
target_link_libraries(link16_app
    link16_core
    link16_protocol
    link16_coding
    link16_physical
    link16_simulation
)

# 安装规则
install(TARGETS link16_app
    RUNTIME DESTINATION bin
)

# 添加测试
enable_testing()
add_subdirectory(tests)
