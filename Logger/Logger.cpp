#include "Logger.h"
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>

const std::string rootFolderName = "/HttpServer/";

// Initialize static member variable
std::once_flag Logger::flag;
std::unique_ptr<Logger> Logger::instance = nullptr;

std::string Logger::getRelativePath(const std::string& fullPath) {
    auto pos = fullPath.find(rootFolderName);
    if (pos != std::string::npos) {
        // +1 是为了保留开始的斜杠 '/'
        return fullPath.substr(pos + 1);
    }
    return fullPath; // 如果找不到项目根目录名称，返回完整路径
}

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

    workerThread = std::thread(&Logger::process, this);
}

Logger::~Logger()
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        exit = true;
    }
    cv.notify_one();
    workerThread.join();
    if (logFile.is_open())
    {
        logFile.close();
    }
}

void Logger::initiateSingleton() { 
    instance.reset(new Logger); 
}

Logger* Logger::getInstance() {
    std::call_once(flag, &Logger::initiateSingleton);
    return instance.get();
}

void Logger::log(const std::string& message, const std::string& file, const std::string& function) {
    std::string logMessage = createLogMessage(message, file, function);
    auto logTask = [this, logMessage]() {
        logFile << logMessage << std::endl;
        std::cout << logMessage << std::endl;
    };
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        tasks.push(logTask);
    }
    cv.notify_one();
}

std::string Logger::createLogMessage(const std::string& message, const std::string& file, const std::string& function) {
    std::ostringstream timestamp;
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto localtime = *std::localtime(&now_time_t);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    timestamp << std::put_time(&localtime, "%Y-%m-%d %H:%M:%S");
    timestamp << '.' << std::setfill('0') << std::setw(3) << now_ms.count();
    timestamp << " [" << getRelativePath(file) << "::" << function << "]: " << message;

    return timestamp.str();
}

void Logger::process()
{
    while (!exit)
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        while (!exit || !tasks.empty()) {
            cv.wait(lock, [this]{ return !tasks.empty() || exit; });
            while (!tasks.empty()) {
                auto task = std::move(tasks.front());
                tasks.pop();
                lock.unlock();
                task();  // Execute task
                lock.lock();
            }
        }
    }
}