#include "Logger.h"
#include "Localtime.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>

namespace clog
{

thread_local char t_errnobuf[512];


const char* strerror_tl(int savedErrno)
{
    return strerror_r(savedErrno, t_errnobuf, sizeof(t_errnobuf));
}

const char* LogLevelName[Logger::LogLevelNum]
{
    "TRACE ",
    "DEBUG ",
    "INFO  ",
    "WARN  ",
    "ERROR ",
    "FATAL "
};

// 当已知字符串长度时，可以直接使用这个类
// 从而避免重新计算字符串的长度
class T
{
 public:
    T(const char* str, size_t len)
        : _str(str),
          _len(len)
    {
        assert(strlen(str) == len);
    }

    const char* _str;
    const size_t _len;
};

inline LogStream& operator<<(LogStream& s, T v)
{
    s.append(v._str, v._len);
    return s;
}

inline LogStream& operator<<(LogStream&s ,Logger::SourceFile file)
{
    s.append(file._data, file._size);
    return s;
}

void defaultOutput(const char* msg, size_t len)
{
    fwrite(msg, 1, len, stdout);
}

void defaultFlush()
{
    fflush(stdout);
}

Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc g_flush = defaultFlush;
Logger::LogLevel g_logLevel = Logger::INFO;  // 默认等级INFO

}  // end of namespace clog

using namespace clog;

class Logger::Impl
{
 public:
    using LogLevel = Logger::LogLevel; 
     
    LogLevel _level;
    Logger::SourceFile _file;
    int _line;
    Localtime _time;
    LogStream _stream;
    
    void finish();

    Impl(LogLevel level, int savedErrno, const Logger::SourceFile& file, int line);
    
};


Logger::Impl::Impl(LogLevel level, int savedErrno, const Logger::SourceFile& file, int line)
    : _level(level),
      _file(file),
      _line(line),
      _time(Localtime::now()),
      _stream()
{
    _stream << _time.toFormattedString() << ' ';
    _stream << T(LogLevelName[_level], 6);
    
    if (savedErrno)
    {
         _stream << clog::strerror_tl(savedErrno) << " (errno=" << savedErrno << ") "; 
    }
}

void Logger::Impl::finish()
{
    _stream << " - " << _file << ":" << _line << '\n'; 
}

Logger::Logger(SourceFile file, int line)
    : _pImpl(new Impl(INFO, 0, file, line))
{
}

Logger::Logger(SourceFile file, int line, LogLevel level)
    : _pImpl(new Impl(level, 0, file, line))
{
}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
    : _pImpl(new Impl(level, 0, file, line))
{
    _pImpl->_stream << func << ' ';
}

Logger::Logger(SourceFile file, int line, bool toAbort)
    : _pImpl(new Impl(toAbort?FATAL:ERROR, errno, file, line))
{
}

Logger::~Logger()
{
    _pImpl->finish();
    const FixBuffer& buffer(_pImpl->_stream.buffer());
    g_output(buffer.data(), buffer.size());
    if (_pImpl->_level == FATAL)
    {
        g_flush();
        abort();
    }
}

LogStream& Logger::stream() noexcept
{
    return _pImpl->_stream;
}

Logger::LogLevel Logger::logLevel() noexcept
{
    return g_logLevel;
}

void Logger::setLogLevel(Logger::LogLevel level) noexcept
{
    g_logLevel = level;
}

void Logger::setOutput(Logger::OutputFunc output) noexcept
{
    g_output = output;
}

void Logger::setFlush(Logger::FlushFunc flush) noexcept
{
    g_flush = flush;
}

