/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-30 11:55:23
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-30 12:05:08
 * @FilePath: /ACWebserver/WebServer/EventLoopThreadPool.hpp
 * @Description: 
 *  */
#pragma once

#include <memory>
#include <vector>

#include "EventLoopThread.hpp"
#include "Logging.hpp"
#include "noncopyable.hpp"

namespace aclolinta {

class EventLoopThreadPool : public noncopyable {
   public:
    EventLoopThreadPool(EventLoop* baseLoop, int numThreads);
    ~EventLoopThreadPool() { LOG << "~EventLoopThreadPool()"; }

   public:
    void start();
    EventLoop* getNextLoop();

   private:
    EventLoop* baseLoop_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;
};
}  // namespace aclolinta