#!/bin/bash

# 检查CMake是否安装
if ! command -v cmake &> /dev/null; then
    echo "CMake not found. Please install CMake and add it to your PATH."
    exit 1
fi

# 创建构建目录
mkdir -p build

# 进入构建目录
cd build

# 配置项目
echo "Configuring project..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# 构建项目
echo "Building project..."
cmake --build . --config Release

# 检查构建是否成功
if [ $? -ne 0 ]; then
    echo "Build failed."
    exit 1
fi

echo "Build successful!"
echo "Executable can be found in build/bin/link16_app"

exit 0
