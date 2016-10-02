# clog
一个用C++编写的练手日志库项目。

### 用法
在单线程下直接`LOG_XXXX << "log entry"`。<br>
在多线程下需要先`Logger::setConcurrentMode()`，并且在程序结束时调用`Logger::finishConcurrent()`,不过应该有更好到办法，想到再来修改。<br>

### 日志等级
6个日志等级:<br>
TRACE<br>
DEBUG<br>
INFO<br>
WARN<br>
ERROR<br>
FATAL<br>

### 文件
LogStream.h/cpp 流对象，内部使用一个std::string作为缓冲区。<br>
Localtime.h/cpp 当地时间对象，获取当地时间，使用的时`gettimeofday`和`localtime_r`。<br>
BlockingQueu.h  阻塞队列，多线程模式下使用。<br>
Logger.h/cpp    日志对象，单线程下把日志记录写到缓冲区中，多线程模式下，专门开了一个线程来记录日志，使用阻塞队列来传递日志消息。<br>
