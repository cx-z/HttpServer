# 添加库 "Logger"
add_library(Logger Logger.cpp Logger.h)

target_link_libraries(Logger PUBLIC Utils)

# 公开头文件目录
target_include_directories(Logger PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}"
                           )