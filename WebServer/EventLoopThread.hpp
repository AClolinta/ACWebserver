/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-30 10:54:41
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-30 11:57:46
 * @FilePath: /ACWebserver/WebServer/EventLoopThread.hpp
 * @Description:  */
#pragma once

#include "Condition.hpp"
#include "EventLoop.hpp"
#include "MutexLock.hpp"
#include "Thread.hpp"
#include "noncopyable.hpp"

using namespace aclolinta::thread;
using namespace aclolinta::event;
// using namespace aclolinta::

namespace aclolinta {
namespace event {

class EventLoopThread : public noncopyable {
   public:
    EventLoopThread(/* args */);
    ~EventLoopThread();

    EventLoop *startLoop();

   private:
    /* data */
    void threadFunc();
    EventLoop *loop_;
    bool exiting_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
};
}  // namespace event
}  // namespace aclolinta