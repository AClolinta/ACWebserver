/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-21 12:03:37
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-07-01 03:13:26
 * @FilePath: /ACWebserver/WebServer /utility/thread.hpp
 * @Description: 线程类
 * */
#pragma once

#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <functional>
#include <memory>
#include <string>

#include "CDLatch.hpp"
#include "noncopyable.hpp"

namespace aclolinta {

class Thread {
   public:
    using ThreadFunc = std::function<void()>;
    explicit Thread(const ThreadFunc&, const std::string& name = std::string());
    ~Thread();

   public:
    void start();
    int join();
    bool isstart() const { return started_; };
    pid_t get_tid() const { return tid_; }

   private:
    void set_default_name();

   private:
    /* data */
    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
    CDLatch latch_;
};

}  // namespace aclolinta
