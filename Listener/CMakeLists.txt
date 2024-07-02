# 添加库 "Listener"
add_library(Listener Listener.cpp Listener.h)

target_link_libraries(Listener PUBLIC Logger Responder)

# 公开头文件目录
target_include_directories(Listener PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}"
                           )