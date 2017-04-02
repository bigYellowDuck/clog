#ifndef CLOG_LOGSTREAM_H
#define CLOG_LOGSTREAM_H

#include <string.h>

#include <vector>
#include <algorithm>
#include <string>

namespace clog
{

namespace detail 
{

const int kSmallBuffer = 4 * 1024;
const int kLargeBuffer = 4 * 1024 * 1024;

std::string getLogFileName();


template <int SIZE>
class FixBuffer
{
 public:
    FixBuffer()
        : cur_(data_)
    {

    }

    ~FixBuffer()
    {
    }
    
    // 删除拷贝构造和拷贝赋值
    FixBuffer(const FixBuffer&) = delete;
    FixBuffer& operator=(const FixBuffer&) = delete;

    // 因为声明了析构函数，移动构造和移动赋值也会被删除

    size_t size() const noexcept { return cur_ - data_; }

    size_t avail() const noexcept { return end() - cur_; }

    const char* data() const noexcept { return data_; }

    void append(const char* data, size_t len)
    {
        if (avail() > len) 
        {
            memcpy(cur_, data, len);
            cur_ += len;
        }
    }
    
    char* current() const noexcept { return cur_; }
    void add(size_t len) { cur_ += len; }

    std::string toString() const
    {
        return std::string(data_, size());
    }

    void reset() { cur_ = data_; }
 private:
    const char* end() const { return data_ + sizeof(data_); }
    char data_[SIZE];
    char *cur_;
};

} // end of namespace detail

class LogStream 
{
    using self = LogStream;
 public:
    using Buffer = detail::FixBuffer<detail::kSmallBuffer>;
    LogStream() = default;
    ~LogStream() = default;
    LogStream(const LogStream&) = delete;
    LogStream& operator=(const LogStream&) = delete;


    self& operator<<(bool v) 
    {
        _buffer.append(v?"1":"0", 1);
        return *this;
    }

    self& operator<<(short);
    self& operator<<(unsigned short);
    self& operator<<(int);
    self& operator<<(unsigned int);
    self& operator<<(long);
    self& operator<<(unsigned long);
    self& operator<<(long long);
    self& operator<<(unsigned long long);

    self& operator<<(const void*);

    self& operator<<(float v)
    {
        *this << static_cast<double>(v);
        return *this;
    }
    self& operator<<(double);
    
    self& operator<<(char v)
    {
        _buffer.append(&v, 1);
        return *this;
    }

    self& operator<<(const char* str)
    {
        if (str)
        {
            _buffer.append(str, strlen(str));
        }
        else
        {
            _buffer.append("(null)", 6);
        }
        
        return *this;
    }

    self& operator<<(const unsigned char* str)
    {
        *this << reinterpret_cast<const char*>(str);
        return *this;
    }

    self& operator<<(const std::string& v)
    {
        _buffer.append(v.c_str(), v.size());
        return *this;
    }

    void append(const char* data, size_t len) { _buffer.append(data, len); }
    const Buffer& buffer() const noexcept { return _buffer; }
    void reset() { _buffer.reset(); }
 private:

    template <typename T>   
    void formatInteger(T);

    Buffer _buffer;
};


}   // end of namespace clog

#endif // CLOG_LOGSTREAM_H

