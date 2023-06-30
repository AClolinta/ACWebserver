/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-30 12:25:55
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-30 12:34:13
 * @FilePath: /ACWebserver/WebServer/Server.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include <memory>

#include "Channel.hpp"
#include "EventLoop.hpp"
#include "EventLoopThreadPool.hpp"

using namespace aclolinta::event;

namespace aclolinta {
namespace server {

class Server {
   public:
    Server(EventLoop* loop, int threadNum, int port);
    ~Server() { LOG << "~Server()"; }
    EventLoop* getLoop() const { return loop_; }
    void start();
    void handNewConn();
    void handThisConn() { loop_->updatePoller(acceptChannel_); }

   private:
    EventLoop* loop_;
    int threadNum_;
    std::unique_ptr<EventLoopThreadPool> eventLoopThreadPool_;
    bool started_;
    std::shared_ptr<Channel> acceptChannel_;
    int port_;
    int listenFd_;
    static const int MAXFDS = 100000;
};
}  // namespace server
}  // namespace aclolinta
