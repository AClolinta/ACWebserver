/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-22 09:15:28
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-22 10:01:35
 * @FilePath: /ACWebserver/WebServer/utility/AsyncLogging.hpp
 * @Description:
 * AsyncLogging是核心，它负责启动一个log线程，专门用来将log写入LogFile，应用了“双缓冲技术”，其实有4个以上的缓冲区，但思想是一样的。
 * AsyncLogging负责(定时到或被填满时)将缓冲区中的数据写入LogFile中。*/
#pragma once
#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include "CDLatch.hpp"
#include "LogStream.hpp"
#include "MutexLock.hpp"
#include "Thread.hpp"
#include "noncopyable.hpp"

class AsyncLogging : noncopyable {
   public:
    AsyncLogging(std::string_view basename, int flushInterval = 2);
    ~AsyncLogging() {
        if (running_) stop();
    }

   public:
    void append(const char* logline, int len);

    void start() {
        running_ = true;
        thread_.start();
        latch_.wait();
    }

    void stop() {
        running_ = false;
        cond_.notify();
        thread_.join();
    }

   private:
    void threadFunc();

   private:
    /* data */
    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef std::vector<std::shared_ptr<Buffer>> BufferVector;
    typedef std::shared_ptr<Buffer> BufferPtr;
    const int flushInterval_;
    bool running_;
    std::string basename_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    BufferVector buffers_;
    CountDownLatch latch_;
};
