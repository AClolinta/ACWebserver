/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-23 11:30:28
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-30 12:10:38
 * @FilePath: /ACWebserver/WebServer/EventLoop.hpp
 * @Description: EventLoop
 * One loop per thread意味着每个线程只能有一个EventLoop对象，
 * EventLoop即是时间循环，每次从poller里拿活跃事件，并给到Channel里分发处理。
 * EventLoop中的loop函数会在最底层(Thread)中被真正调用，
 * 开始无限的循环，直到某一轮的检查到退出状态后从底层一层一层的退出。
 *  */

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
// #include "CDLatch.hpp"
using namespace aclolinta::thread;
using namespace aclolinta::util;

namespace aclolinta {
namespace event {
class EventLoop {
   public:
    using Functor = std::function<void()>;
    EventLoop(/* args */);
    ~EventLoop();
    void loop();
    void quit();
    void runInLoop(Functor&& cb);
    /// @brief Add tasks to the event loop's task queue and wake up the event
    /// loop thread to process the tasks when needed
    /// @param cb
    void queueInLoop(Functor&& cb);
    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
    void assertInLoopThread() { assert(isInLoopThread()); }
    void shutdown(std::shared_ptr<Channel> channel) {
        shutDownWR(channel->getFd());
    }
    void removeFromPoller(std::shared_ptr<Channel> channel) {
        // shutDownWR(channel->getFd());
        poller_->epoll_del(channel);
    }
    void updatePoller(std::shared_ptr<Channel> channel, int timeout = 0) {
        poller_->epoll_mod(channel, timeout);
    }
    void addToPoller(std::shared_ptr<Channel> channel, int timeout = 0) {
        poller_->epoll_add(channel, timeout);
    }

   private:
    /* data */
    // 声明顺序 wakeupFd_ > pwakeupChannel_
    bool looping_;                   // Loop status of objects
    std::shared_ptr<Epoll> poller_;  // shared_ptr of Epoll
    int wakeupFd_;
    bool qiut_;           // quit status of EventLoop obj
    bool eventHandling_;  // Is the event being processed tasks
    mutable MutexLock mutex_;
    std::vector<Functor> pendingFunctors_;  // User tasks waiting to be executed
    bool callingPendingFunctors_;  // Is the user tasks are being executed
    const pid_t threadId_;         // PID
    std::shared_ptr<Channel> pwakeupChannel_;  // shared_ptr of Channel
                                               // connected to the wakeupFd_

   private:
    /// @brief Wake up event loop
    void wakeup();

    /// @brief Ensure the stata of pwakeupChannel_ is consistent with the event
    /// listener when handling the connection event
    void handleRead();
    void doPendingFunctors();
    void handleConn();
};

}  // namespace event
}  // namespace aclolinta