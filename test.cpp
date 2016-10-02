#include "Logger.h"
#include <iostream>
#include <thread>

using namespace std;

void tf()
{
    LOG_TRACE << "test TRACE";
    LOG_DEBUG << "test DEBUG";
    LOG_INFO << "test INFO";
    LOG_WARN << "test WARN";
    LOG_ERROR << "test ERROR";
    LOG_TRACE << "test TRACE";
    LOG_TRACE << "test TRACE";
    LOG_DEBUG << "test DEBUG";
    LOG_INFO << "test INFO";
    LOG_TRACE << "test TRACE";
    LOG_DEBUG << "test DEBUG";
    LOG_INFO << "test INFO";
    LOG_WARN << "test WARN";
    LOG_ERROR << "test ERROR";
    LOG_WARN << "test WARN";
    LOG_ERROR << "test ERROR";
    LOG_DEBUG << "test DEBUG";
    LOG_INFO << "test INFO";
    LOG_WARN << "test WARN";
    LOG_ERROR << "test ERROR";
    LOG_DEBUG << "test DEBUG";
    LOG_INFO << "test INFO";
    LOG_WARN << "test WARN";
    LOG_ERROR << "test ERROR";
    LOG_DEBUG << "test DEBUG";
    LOG_INFO << "test INFO";
    LOG_WARN << "test WARN";
    LOG_ERROR << "test ERROR";
}

int main()
{
    using namespace clog;
    Logger::setLogLevel(Logger::DEBUG);
    Logger::setConcurrentMode();
    thread t(tf);
    LOG_TRACE << "test TRACE";
    LOG_DEBUG << "test DEBUG";
    LOG_INFO << "test INFO";
    LOG_WARN << "test WARN";
    LOG_TRACE << "test TRACE";
    LOG_TRACE << "test TRACE";
    LOG_TRACE << "test TRACE";
    LOG_TRACE << "test TRACE";
    LOG_TRACE << "test TRACE";
    LOG_TRACE << "test TRACE";
    LOG_TRACE << "test TRACE";
    LOG_DEBUG << "test DEBUG";
    LOG_INFO << "test INFO";
    LOG_WARN << "test WARN";
    LOG_ERROR << "test ERROR";
    LOG_DEBUG << "test DEBUG";
    LOG_INFO << "test INFO";
    LOG_WARN << "test WARN";
    LOG_ERROR << "test ERROR";
    LOG_DEBUG << "test DEBUG";
    LOG_INFO << "test INFO";
    LOG_WARN << "test WARN";
    LOG_ERROR << "test ERROR";
    LOG_DEBUG << "test DEBUG";
    LOG_INFO << "test INFO";
    LOG_WARN << "test WARN";
    LOG_ERROR << "test ERROR";
    LOG_DEBUG << "test DEBUG";
    LOG_INFO << "test INFO";
    LOG_WARN << "test WARN";
    LOG_ERROR << "test ERROR";
    LOG_DEBUG << "test DEBUG";
    LOG_INFO << "test INFO";
    LOG_WARN << "test WARN";
    LOG_ERROR << "test ERROR";
    LOG_DEBUG << "test DEBUG";
    LOG_INFO << "test INFO";
    LOG_WARN << "test WARN";
    LOG_ERROR << "test ERROR";
    LOG_ERROR << "test ERROR";
    t.join();
    Logger::finishConcurrent();
    return 0;
}
