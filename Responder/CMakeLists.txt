# 添加库 "Responder"
add_library(Responder Responder.cpp Responder.h)

target_link_libraries(Responder PUBLIC Logger)

# 公开头文件目录
target_include_directories(Responder PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}"
                           )