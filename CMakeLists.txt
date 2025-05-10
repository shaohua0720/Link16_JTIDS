cmake_minimum_required(VERSION 3.10)
project(Link16 VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 检查编译器是否支持C++17
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
if(NOT COMPILER_SUPPORTS_CXX17)
    message(FATAL_ERROR "编译器 ${CMAKE_CXX_COMPILER} 不支持 C++17 标准")
endif()

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 设置编译选项
if(MSVC)
    # Visual Studio编译选项
    add_compile_options(/W4 /MP /EHsc)
    # 使用UTF-8编码
    add_compile_options(/utf-8)
else()
    # GCC/Clang编译选项
    add_compile_options(-Wall -Wextra -Wpedantic)
    # 使用UTF-8编码
    add_compile_options(-finput-charset=UTF-8)
endif()

# 添加包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/lib
)

# 检测操作系统
if(WIN32)
    message(STATUS "Building for Windows")
    add_definitions(-DPLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    message(STATUS "Building for Linux")
    add_definitions(-DPLATFORM_LINUX)
elseif(APPLE)
    message(STATUS "Building for macOS")
    add_definitions(-DPLATFORM_MACOS)
else()
    message(WARNING "Unknown platform")
endif()

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

# 应用层源文件
set(APPLICATION_SOURCES
    src/application/main.cpp
    src/application/Link16System.cpp
    src/application/real/Link16App.cpp
    src/application/simulation/SimulationApp.cpp
    # 移除 src/architecture/Link16System.cpp
)

file(GLOB_RECURSE API_SOURCES
    "src/api/*.cpp"
)

# 创建库
add_library(link16_core STATIC ${CORE_SOURCES})
add_library(link16_protocol STATIC ${PROTOCOL_SOURCES})
add_library(link16_coding STATIC ${CODING_SOURCES})
add_library(link16_physical STATIC ${PHYSICAL_SOURCES})
add_library(link16_simulation STATIC ${SIMULATION_SOURCES})
add_library(link16_api STATIC ${API_SOURCES})

# 创建可执行文件
add_executable(link16_app ${APPLICATION_SOURCES})

# 链接库
target_link_libraries(link16_app
    link16_core
    link16_protocol
    link16_coding
    link16_physical
    link16_simulation
    link16_api
)

# 设置API库的依赖关系
target_link_libraries(link16_api
    link16_core
    link16_protocol
    link16_coding
    link16_physical
    link16_simulation
)

# 在Linux系统上链接额外的库
if(UNIX AND NOT APPLE)
    target_link_libraries(link16_app
        pthread
        stdc++fs  # 对于较旧的GCC版本，需要显式链接filesystem库
    )

    target_link_libraries(link16_api
        pthread
        stdc++fs
    )
endif()

# 安装规则
install(TARGETS link16_app
    RUNTIME DESTINATION bin
)

# 添加测试
enable_testing()
add_subdirectory(tests)

# 添加文档生成
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

# 添加打包配置
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT "your.email@example.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Link16 Protocol Implementation")

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_MODIFY_PATH ON)
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
else()
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Your Name")
    set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6)")
endif()

include(CPack)
