@echo off
setlocal enabledelayedexpansion

REM 检查CMake是否安装
where cmake >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo CMake not found. Please install CMake and add it to your PATH.
    exit /b 1
)

REM 创建构建目录
if not exist build mkdir build

REM 进入构建目录
cd build

REM 配置项目
echo Configuring project...
cmake .. -DCMAKE_BUILD_TYPE=Release

REM 构建项目
echo Building project...
cmake --build . --config Release

REM 检查构建是否成功
if %ERRORLEVEL% neq 0 (
    echo Build failed.
    exit /b 1
)

echo Build successful!
echo Executable can be found in build\bin\Release\link16_app.exe

exit /b 0
