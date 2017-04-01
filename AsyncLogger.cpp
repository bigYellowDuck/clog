#include "AsyncLogger.h"

#include <assert.h>
#include <stdio.h>

#include <iostream>

using namespace clog;

AsyncLogger::AsyncLogger(int flushInterval)
    : flushInterval_(flushInterval),
      running_(false),
      thread_(),
      mutex_(),
      cond_(),
      currentBuffer_(new Buffer),
      nextBuffer_(new Buffer),
      buffers_()
{
    buffers_.reserve(16);
}

void AsyncLogger::start()
{
    running_ = true;
    thread_ = std::thread([this](){  this->threadFunc(); });
}

void AsyncLogger::stop()
{
    running_ = false;
    cond_.notify_one();
    thread_.join();
}

void AsyncLogger::append(const char* logline, size_t len)
{
    MutexLockGuard lock(mutex_);
    if (currentBuffer_->avail() > len)
    {
        currentBuffer_->append(logline, len);
    }
    else
    {
        buffers_.push_back(std::move(currentBuffer_));

        if (nextBuffer_)
        {
            currentBuffer_ = std::move(nextBuffer_);
        }
        else
        {
            currentBuffer_.reset(new Buffer);  // Rarely happens
        }
        currentBuffer_->append(logline, len);
        cond_.notify_one();
    }
}


void AsyncLogger::threadFunc()
{
    assert(running_ == true);
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);

    FILE* stream = fopen(detail::getLogFileName().data(), "w");
    assert(stream);

    while (running_)
    {
        assert(newBuffer1 && newBuffer1->size() == 0);
        assert(newBuffer2 && newBuffer2->size() == 0);
        assert(buffersToWrite.empty());

        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (buffers_.empty())  // unusual usage!
            {
                cond_.wait_for(lock, std::chrono::seconds(flushInterval_));
            }

            buffers_.push_back(std::move(currentBuffer_));
            currentBuffer_ = std::move(newBuffer1);
            buffersToWrite.swap(buffers_);

            if (!nextBuffer_)
            {
                nextBuffer_ = std::move(newBuffer2);
            }
        }

        assert(!buffersToWrite.empty());

        if (buffersToWrite.size() > 25)
        {
            // TODO::dropping log messages
            printf("two much\n");
        }

        for (size_t i = 0; i < buffersToWrite.size(); ++i)
        {
            fwrite(buffersToWrite[i]->data(), 1, buffersToWrite[i]->size(), stream); 
        }

        if (buffersToWrite.size() > 2)
        {
            buffersToWrite.resize(2);
        }

        if (!newBuffer1)
        {
            assert(!buffersToWrite.empty());
            newBuffer1 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer1->reset();
        }

        if (!newBuffer2)
        {
            assert(!buffersToWrite.empty());
            newBuffer2 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer2->reset();
        }

        buffersToWrite.clear();
        fflush(stream);
    }
    fflush(stream);
}

