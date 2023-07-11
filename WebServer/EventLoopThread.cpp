/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-30 11:04:21
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-07-02 03:18:35
 * @FilePath: /ACWebserver/WebServer/EventLoopThread.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "EventLoopThread.hpp"

#include <functional>


EventLoopThread::EventLoopThread()
    : loop_(nullptr),
      exiting_(false),
      thread_(std::bind(&EventLoopThread::threadFunc, this)),
      mutex_(),
      cond_(mutex_) {}

EventLoopThread::~EventLoopThread() {
    exiting_ = true;
    if (loop_ != nullptr) {
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop() {
    assert(!thread_.isstart());
    thread_.start();
    {
        MutexLockGuard lock(mutex_);
        //Waiting for the thread to start
        while (loop_ == nullptr) cond_.wait();
    }
    return loop_;
}

void EventLoopThread::threadFunc() {
    EventLoop loop;
    {
        MutexLockGuard lock(mutex_);
        loop_ = &loop;
        cond_.notify();
    }
    loop_->loop();
    loop_ = nullptr;
}