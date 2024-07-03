#!/bin/bash

# 函数定义：构建项目
build_project() {
    # 检查是否存在build目录，如果不存在则创建
    if [ ! -d "build" ]; then
        echo "Creating build directory..."
        mkdir build
    fi

    # 进入build目录
    cd build || { echo "Failed to enter build directory"; exit 1; }

    # 清理build目录中的所有文件
    echo "Cleaning build directory..."
    rm -rf *

    # 执行cmake生成makefile
    echo "Running CMake..."
    cmake ..

    # 构建项目
    echo "Building project..."
    make

    # 返回到脚本所在目录
    cd ..

    echo "Build completed."
}

# 函数定义：运行项目
run_project() {
    # 执行构建项目
    build_project

    # 检查可执行文件是否存在
    if [ ! -f "build/HttpClient" ]; then
        echo "Executable not found. Build may have failed."
        exit 1
    fi

    # 运行可执行文件
    echo "Running HttpClient..."
    ./build/HttpClient
}

# 函数定义：清理项目
clean_project() {
    if [ -d "build" ]; then
        echo "Cleaning build directory..."
        rm -rf build/*
    else
        echo "Build directory does not exist. Nothing to clean."
    fi
}

# 主脚本逻辑：根据参数执行操作
case "$1" in
    build)
        build_project
        ;;
    run)
        run_project
        ;;
    clean)
        clean_project
        ;;
    *)
        echo "Usage: $0 {build|run|clean}"
        exit 1
        ;;
esac