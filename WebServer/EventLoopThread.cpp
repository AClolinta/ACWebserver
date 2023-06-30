#include "EventLoopThread.hpp"

#include <functional>

using namespace aclolinta::thread;
using namespace aclolinta::event;

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