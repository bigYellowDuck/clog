#include "LogStream.h"
#include <iostream>
int main()
{
    clog::LogStream log;
    log << -66 << " " << 55.7 << " " << "abc" << " " << std::string("xca");
    auto str = log.buffer().toString();
    std::cout << str << std::endl;

    return 0;
}
