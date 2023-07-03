/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-30 12:05:42
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-07-02 04:35:23
 * @FilePath: /ACWebserver/WebServer/EventLoopThreadPool.cpp
 * @Description: 线程池 */
#include "EventLoopThreadPool.hpp"

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop, int numThreads)
    : baseLoop_(baseLoop), started_(false), numThreads_(numThreads), next_(0) {
    if (numThreads_ <= 0) {
        LOG << ("numThreads_ <= 0");
        abort();
    }
}

void EventLoopThreadPool::start() {
    baseLoop_->assertInLoopThread();
    started_ = true;
    for (int i = 0; i < numThreads_; ++i) {
        std::unique_ptr<EventLoopThread> t(new EventLoopThread());
        threads_.push_back(std::move(t));
        loops_.push_back(threads_[i]->startLoop());
    }
}

EventLoop* EventLoopThreadPool::getNextLoop() {
    baseLoop_->assertInLoopThread();
    assert(started_);
    EventLoop* loop = baseLoop_;
    if (!loops_.empty()) {
        loop = loops_[next_];
        next_ = (next_ + 1) % numThreads_;
    }
    return loop;
}
