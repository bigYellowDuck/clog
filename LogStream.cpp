#include "LogStream.h"


namespace clog
{

namespace detail
{

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;
static_assert(sizeof(digits)==20, "size of digits is not equal to 20");

const char digitsHex[] = "0123456789ABCDEF";
static_assert(sizeof(digitsHex)==17, "size of digitsHex is not equal to 17");

// 把整形数快速转换成字符串，作者Matthew Wilson 
/*
template <typename T>
size_t convert(char buf[], T value)
{
    T i = value;
    char* p = buf;

    do 
    {
        int lsd = static_cast<int>(i % 10);
        *p++ = zero[lsd];
        i /= 10;
    } while (i != 0);

    if (value < 0)
        *p++ = '-';
    *p = '\0';

    std::reverse(buf, p);

    return p - buf;
}

size_t convertHex(char buf[], uintptr_t value)
{
    uintptr_t i = value;
    char* p = buf;

    do 
    {
        int lsd = static_cast<int>(i % 16);
        *p++ = digitsHex[lsd];
        i /= 16;
    } while (i != 0);

    *p = '\0';

    std::reverse(buf, p);
    
    return p - buf;
}
*/
}  // end of namespace detail



template <typename T>
size_t LogStream::convert(T value)
{
    auto& buf = _buffer.buffer();
    T i = value;
    int count = 0;    // 引入计数，不能直接用auto iter = buf.end()保留当前位置
                      // 因为push_back后迭代器可能失效

    do 
    {   
        int lsd = static_cast<int>(i % 10);
        i /= 10;
        buf.push_back(detail::zero[lsd]);
        ++count;
    } while (i != 0);

    if (value < 0)
    {
        buf.push_back('-');
        ++count;
    }

    std::reverse(buf.end() - count, buf.end());

    return count;
}

size_t LogStream::convertHex(uintptr_t value)
{
    auto& buf = _buffer.buffer();
    uintptr_t i = value;
    int count = 0;

    do
    {
        int lsd = static_cast<int>(i % 16);
        i /= 16;
        buf.push_back(detail::digitsHex[lsd]);
        ++count;
    } while(i != 0);
    
    std::reverse(buf.end() - count, buf.end());

    return count;
}

template <typename T>
void LogStream::formatInteger(T v)
{
    size_t len = convert(v);
    (void)len;
}

LogStream& LogStream::operator<<(short v)
{
    *this << static_cast<int>(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short v)
{
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream& LogStream::operator<<(int v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(const void *p)
{
    uintptr_t v = reinterpret_cast<uintptr_t>(p);
    auto& buf = _buffer.buffer();
    buf.push_back('0');
    buf.push_back('x');

    size_t len = convertHex(v);
    (void)len;

    return *this;

}

LogStream& LogStream::operator<<(double v)
{
    char buf[32];

    size_t len = snprintf(buf, 32, "%.12g", v);
    _buffer.append(buf, len);

    return *this;
}

}  // end of namespace log

