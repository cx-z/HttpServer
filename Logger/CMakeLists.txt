# 添加库 "Logger"
add_library(Logger Logger.cpp Logger.h)

# 可调试
set(CMAKE_BUILD_TYPE Debug)

# 公开头文件目录
target_include_directories(Logger PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}"
                           )