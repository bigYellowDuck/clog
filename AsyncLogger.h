#ifndef CLOG_ASYNCLOGGER_H
#define CLOG_ASYNCLOGGER_H

#include "LogStream.h"

#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <memory>

namespace clog
{

class AsyncLogger
{
  public:
    AsyncLogger(int flushInterval = 3);

    ~AsyncLogger()
    {
        if (running_)
            stop();
    }
    
    void start();

    void stop();

    void append(const char* logline, size_t len);
  private:
    void threadFunc();

    using MutexLockGuard = std::lock_guard<std::mutex>;

    using Buffer = detail::FixBuffer<detail::kLargeBuffer>;
    using BufferVector = std::vector<std::unique_ptr<Buffer>>;
    using BufferPtr = BufferVector::value_type;

    const int flushInterval_;
    bool running_;
    std::thread thread_;
    std::mutex mutex_;
    std::condition_variable cond_;
    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    BufferVector buffers_;
};

}

#endif // CLOG_ASYNCLOGGER_H

