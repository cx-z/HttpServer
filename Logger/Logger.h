#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

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

    void log(const std::string& message);
};

#endif // LOGGER_H