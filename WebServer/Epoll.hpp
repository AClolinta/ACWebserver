/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-25 04:20:15
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-07-01 03:16:12
 * @FilePath: /ACWebserver/WebServer/Epoll.hpp
 * @Description: Epoll真难啊 */
#pragma once
#include <sys/epoll.h>

#include <map>
#include <memory>
#include <vector>

#include "Channel.hpp"
#include "HttpData.hpp"
#include "Timer.hpp"

using SP_Channel = std::shared_ptr<Channel>;

namespace aclolinta {

class Epoll {
   public:
    Epoll(/* args */);
    ~Epoll();

   public:
    void epoll_add(SP_Channel request, int timeout);
    /// @brief
    /// @param request
    /// @param timeout When timeout == 0, it means that perform immediately
    void epoll_mod(SP_Channel request, int timeout);
    void epoll_del(SP_Channel request);
    std::vector<std::shared_ptr<Channel>> poll();
    std::vector<std::shared_ptr<Channel>> getEventsRequest(int events_num);
    void add_timer(std::shared_ptr<Channel> request_data, int timeout);
    int getEpollFd() { return epollFd_; }
    void handleExpired();

   private:
    /* data */
    int epollFd_;  // fd of epoll
    static const int MAXFDS = 100000;
    std::vector<epoll_event> events_;
    std::shared_ptr<Channel> fd2chan_[MAXFDS];
    std::shared_ptr<HttpData> fd2http_[MAXFDS];
    TimerManager timerManager_;
};

}  // namespace aclolinta
