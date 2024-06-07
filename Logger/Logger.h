#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#define LOG(logger, message) (logger).log((message), __FILE__, __FUNCTION__)

class Logger {
private:
    std::ofstream logFile;
    std::queue<std::string> logQueue;
    std::thread worker;
    std::mutex mtx;
    std::condition_variable cv;
    bool exit;

    void process();

public:
    Logger();
    ~Logger();

    void log(const std::string& message, const std::string& file, const std::string& function);
};

#endif // LOGGER_H