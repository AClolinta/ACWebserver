/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-22 13:17:47
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-22 13:35:46
 * @FilePath: /ACWebserver/WebServer/Channal.hpp
 * @Description:
 * Channel是Reactor结构中的“事件”，它自始至终都属于一个EventLoop，负责一个文件描述符的IO事件，
 * */
#pragma once

#include <sys/epoll.h>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "Timer.hpp"

class EventLoop;
class HttpData;

namespace aclolinta {
namespace task {
class Channal {
   private:
    /* data */
    using CallBack = std::function<void()>;

   public:
    Channal(/* args */);
    ~Channal();
};

}  // namespace task

}  // namespace aclolinta
