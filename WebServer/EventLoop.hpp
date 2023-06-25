
#pragma once
#include <functional>
#include <memory>
#include <vector>

#include "Channel.hpp"
#include "CurrentThread.hpp"
#include "Epoll.hpp"
#include "Logging.hpp"
#include "Thread.hpp"
#include "Util.hpp"

namespace aclolinta {
namespace event {
 class EventLoop
 {
 private:
    /* data */
 public:
    EventLoop(/* args */);
    ~EventLoop();
 };
 
 EventLoop::EventLoop(/* args */)
 {
 }
 
 EventLoop::~EventLoop()
 {
 }
 
}
}  // namespace aclolinta