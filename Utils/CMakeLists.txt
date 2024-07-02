# 添加库 "Utils"
add_library(Utils STATIC
    Utils.cpp 
    Utils.h
)

# 公开头文件目录
target_include_directories(Utils PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}"
                           )