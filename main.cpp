#include"Logger/Logger.h"

int main() {
    Logger logger;
    std::cout << "Hello world" << std::endl;
    LOG(logger, "log message one.");
    LOG(logger, "log message two.");

    return 0;
}