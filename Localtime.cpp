#include "Localtime.h"

#include <sys/time.h>
#include <stdio.h>

namespace clog
{

static_assert(sizeof(Localtime) == sizeof(long long), "size of Localtime is not equal to size of long long");

Localtime::Localtime(long long microSecondsSinceEpoch)
    : _microSecondsSinceEpoch(microSecondsSinceEpoch)
{
}

Localtime Localtime::now()
{
    struct timeval tv;
    ::gettimeofday(&tv, NULL);
    long long seconds = tv.tv_sec;
    return Localtime(seconds * detail::kMicroSecondsPerSecond + tv.tv_usec);
}

std::string Localtime::toFormattedString(bool showMicroSeconds) const
{
    char buf[32] = {0};
    time_t seconds = static_cast<time_t>(_microSecondsSinceEpoch / detail::kMicroSecondsPerSecond);
    struct tm tm_time;
    
    localtime_r(&seconds, &tm_time);
    
    if (showMicroSeconds)
    {
        int microSeconds = static_cast<int>(_microSecondsSinceEpoch % detail::kMicroSecondsPerSecond);
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
                 microSeconds);
    }
    else
    {
        snprintf(buf, sizeof(buf), "%04d%02d%02d %02d:%02d:%02d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);  
    }

    return buf;
}

}  // end of namespace clog

