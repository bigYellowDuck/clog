#ifndef CLOG_LOGGER_H
#define CLOG_LOGGER_H

#include "LogStream.h"

#include <memory>

#include <string.h>

namespace clog
{

class Logger
{
 public:
    enum LogLevel
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        LogLevelNum
    };

class SourceFile 
{
 public:
    template <int N>
    inline SourceFile(const char (&arr)[N])
        : _data(arr),
          _size(N-1)
    {
        const char* slash = strrchr(_data, '/');
        if (slash)
        {
            _data = slash + 1;
        }
        _size = strlen(_data);
    }

    explicit SourceFile(const char* filename)
        : _data(filename)
    {
        const char* slash = strrchr(_data, '/');
        if (slash)
        {
            _data = slash + 1;
        }
        _size = strlen(_data);
    }
    
    const char* _data;
    size_t _size;
};
    
    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, LogLevel level, const char* func);
    Logger(SourceFile file, int line, bool toAbort);
    ~Logger();

    LogStream& stream() noexcept;

    static LogLevel logLevel() noexcept;
    static void setLogLevel(LogLevel level) noexcept;

    using OutputFunc = void (*)(const char* msg, size_t len);
    using FlushFunc = void (*)();
    static void setOutput(OutputFunc) noexcept;
    static void setFlush(FlushFunc) noexcept;
   
 private:
    class Impl;
    std::unique_ptr<Impl> _pImpl;
};

}  // end of namespace clog

#define LOG_TRACE if (clog::Logger::logLevel() <= clog::Logger::TRACE) \
 clog::Logger(__FILE__, __LINE__, clog::Logger::TRACE, __func__).stream()

#define LOG_DEBUG if (clog::Logger::logLevel() <= clog::Logger::DEBUG) \
 clog::Logger(__FILE__, __LINE__, clog::Logger::DEBUG, __func__).stream()

#define LOG_INFO if (clog::Logger::logLevel() <= clog::Logger::INFO) \
 clog::Logger(__FILE__, __LINE__, clog::Logger::INFO, __func__).stream()

#define LOG_WARN if (clog::Logger::logLevel() <= clog::Logger::WARN) \
 clog::Logger(__FILE__, __LINE__, clog::Logger::WARN, __func__).stream()

#define LOG_ERROR if (clog::Logger::logLevel() <= clog::Logger::ERROR) \
 clog::Logger(__FILE__, __LINE__, clog::Logger::ERROR, __func__).stream()

#define LOG_FATAL clog::Logger(__FILE__, __LINE__, clog::Logger::FATAL, __func__).stream()

#define LOG_SYSERR clog::Logger(__FILE__, __LINE__, false).stream()

#define LOG_SYSFATAL clog::Logger(__FILE__, __LINE__, true).stream()

#endif // CLOG_LOGGER_H

