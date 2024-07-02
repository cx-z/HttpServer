#ifndef LOGGER_H
#define LOGGER_H

#include"Utils.h"

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#define LOG(format, ...) Logger::getInstance()->log(Utils::string_format(format, ##__VA_ARGS__), __FILE__, __FUNCTION__)

class Logger {
private:
    static std::once_flag flag;
    static std::unique_ptr<Logger> instance;

    std::queue<std::function<void()>> tasks;
    std::thread workerThread;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool exit = false;

    std::ofstream logFile;

    Logger();
    static void initiateSingleton();
    void process();
    std::string getRelativePath(const std::string& fullPath);
    std::string createLogMessage(const std::string& message, const std::string& file, const std::string& function);

public:
    ~Logger();
    static Logger* getInstance();
    void log(const std::string& message, const std::string& file, const std::string& function);
};

#endif // LOGGER_H