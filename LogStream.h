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

const int kInitCapacity = 2048;

} // end of namespace detail

class FixBuffer
{
 public:
    FixBuffer()
    {
        _buffer.reserve(detail::kInitCapacity);
    }

    ~FixBuffer()
    {
    }
    
    // 删除拷贝构造和拷贝赋值
    FixBuffer(const FixBuffer&) = delete;
    FixBuffer& operator=(const FixBuffer&) = delete;

    // 因为声明了析构函数，移动构造和移动赋值也会被删除
    
    std::vector<char>& buffer() { return _buffer; }

    size_t size() const noexcept { return _buffer.size(); }

    void append(const char* data, size_t len)
    {
        _buffer.resize(_buffer.size() + len);
        std::copy(data, data+len, _buffer.end()-len); 
    }

    std::string toString() const
    {
        return std::string(_buffer.cbegin(), _buffer.cend());
    }

    void reset() { _buffer.clear(); }
 private:
    std::vector<char> _buffer;
};


class LogStream 
{
    using self = LogStream;
 public:
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
    const FixBuffer& buffer() const noexcept { return _buffer; }
    void reset() { _buffer.reset(); }
 private:
    template <typename T>
    size_t convert(T);

    size_t convertHex(uintptr_t);

    template <typename T>   
    void formatInteger(T);

    FixBuffer _buffer;
};


}   // end of namespace clog

#endif // CLOG_LOGSTREAM_H

