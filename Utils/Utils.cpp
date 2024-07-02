#include "Utils.h"

#include <sstream>
#include <stdexcept>
#include <cstdio>
#include <cstdarg>

std::string Utils::string_format(const std::string& format, ...) {
    // 预估格式化后字符串的长度
    int size = 1024;
    char* buffer = new char[size];
    
    va_list args;
    va_start(args, format);
    int length = vsnprintf(buffer, size, format.c_str(), args);
    va_end(args);

    if (length < 0) {
        delete[] buffer;
        throw std::runtime_error("Formatting error");
    }
    
    // 如果 buffer 不够大，则重新分配
    if (length >= size) {
        size = length + 1;
        delete[] buffer;
        buffer = new char[size];

        va_start(args, format);
        vsnprintf(buffer, size, format.c_str(), args);
        va_end(args);
    }

    std::string formattedStr(buffer);
    delete[] buffer;
    return formattedStr;
}