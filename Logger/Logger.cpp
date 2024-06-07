#include "Logger.h"
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>

Logger::Logger() : exit(false)
{
    // 创建 Logs 子目录
    struct stat st;
    if (stat("Logger/Logs", &st) != 0)
    {
        // 目录不存在，创建它
        mkdir("Logger/Logs", 0700); // UNIX/Linux 权限，仅允许所有者读写执行
    }

    // 创建日志文件在 Logger/Logs 文件夹下
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    char filename[64];
    std::strftime(filename, sizeof(filename), "Logger/Logs/Log_%Y-%m-%d_%H-%M-%S.txt", &tm);
    logFile.open(filename);

    worker = std::thread(&Logger::process, this);
}

Logger::~Logger()
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        exit = true;
    }
    cv.notify_one();
    worker.join();
    if (logFile.is_open())
    {
        logFile.close();
    }
}

void Logger::log(const std::string &message)
{
    std::ostringstream timestamp;
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto localtime = *std::localtime(&now_time_t);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    timestamp << std::put_time(&localtime, "%Y-%m-%d %H:%M:%S");
    timestamp << '.' << std::setfill('0') << std::setw(3) << now_ms.count() << ": ";

    std::lock_guard<std::mutex> lock(mtx);
    logQueue.push(timestamp.str() + message);
    cv.notify_one();
}

void Logger::process()
{
    while (!exit)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]()
                { return !logQueue.empty() || exit; });
        while (!logQueue.empty())
        {
            const std::string &logEntry = logQueue.front();
            logFile << logEntry << std::endl;
            std::cout << logEntry << std::endl;
            logQueue.pop();
        }
    }
}