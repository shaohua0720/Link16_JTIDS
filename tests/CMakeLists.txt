cmake_minimum_required(VERSION 3.10)

# 添加测试源文件
file(GLOB_RECURSE TEST_SOURCES 
    "*.cpp"
)

# 创建测试可执行文件
add_executable(link16_tests ${TEST_SOURCES})

# 链接库
target_link_libraries(link16_tests
    link16_core
    link16_protocol
    link16_coding
    link16_physical
    link16_simulation
)

# 添加测试
add_test(NAME link16_tests COMMAND link16_tests)
