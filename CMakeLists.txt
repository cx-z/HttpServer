cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(HttpServer VERSION 1.0)

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# 可调试
set(CMAKE_BUILD_TYPE Debug)

# 添加可执行文件
add_executable(HttpServer main.cpp)

# 包含子目录
add_subdirectory(Logger)

# 链接 Logger 库到主可执行文件
target_link_libraries(HttpServer Logger)