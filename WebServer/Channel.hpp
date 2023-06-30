/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-23 11:29:41
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-30 13:00:15
 * @FilePath: /ACWebserver/WebServer/Channel.hpp
 * @Description:
 * Channel类：Channel是Reactor结构中的“事件”，
 * 它自始至终都属于一个EventLoop，负责一个文件描述符的IO事件，在Channel类中保存这IO事件的类型以及对应的回调函数，
 * 当IO事件发生时，最终会调用到Channel类中的回调函数。
 * 因此，程序中所有带有读写时间的对象都会和一个Channel关联，包括loop中的eventfd，listenfd，HttpData等。
 */
#pragma once

#include <sys/epoll.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "Timer.hpp"

using namespace aclolinta::timer;
using namespace aclolinta::event;

namespace aclolinta {
namespace task {


// class aclolinta::event::EventLoop;
// class aclolinta::http::HttpData;

class Channel {
   private:
    /* data */
    using CallBack = std::function<void()>;
    EventLoop *loop_;
    int fd_;
    __uint32_t events_;   // 当前事件所关注的事件类型
    __uint32_t revents_;  // 当前事件发生后的就绪事件类型
    __uint32_t lastEvents_;
    // 方便找到上层持有该Channel的对象
    // A HttpData object contains a Channel object
    std::weak_ptr<HttpData> holder_;

   private:
    // int parse_URI();
    // int parse_Headers();
    // int analysisRequest();

   private:
    CallBack readHandler_;
    CallBack writeHandler_;
    CallBack errorHandler_;
    CallBack connHandler_;

   public:
    Channel(EventLoop *loop);
    Channel(EventLoop *loop, int fd);
    ~Channel();

   public:
    int getFd() { return fd_; }
    void setFd(int fd) { fd_ = fd; }

    void setHolder(std::shared_ptr<HttpData> holder) { holder_ = holder; }
    // Get the HttpData object which holds this Channel object and increase the reference count
    std::shared_ptr<HttpData> getHolder() {
        std::shared_ptr<HttpData> ret(holder_.lock());
        return ret;
    }

    void setReadHandler(CallBack &&readHandler) { readHandler_ = readHandler; }

    void setWriteHandler(CallBack &&writeHandler) {
        writeHandler_ = writeHandler;
    }

    void setErrorHandler(CallBack &&errorHandler) {
        errorHandler_ = errorHandler;
    }

    void setConnHandler(CallBack &&connHandler) { connHandler_ = connHandler; }

    // 处理逻辑
    void handleEvents() {
        events_ = 0;
        if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
            events_ = 0;
            return;
        }
        if (revents_ & EPOLLERR) {
            if (errorHandler_) errorHandler_();
            events_ = 0;
            return;
        }
        if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
            handleRead();
        }
        if (revents_ & EPOLLOUT) {
            handleWrite();
        }
        handleConn();
    }

    void handleRead();
    void handleWrite();
    void handleError(int fd, int err_num, std::string short_msg);
    void handleConn();

    void setRevents(__uint32_t ev) { revents_ = ev; }
    void setEvents(__uint32_t ev) { events_ = ev; }
    // The current event is updated to the last event.
    bool EqualAndUpdateLastEvents() {
        bool ret = (lastEvents_ == events_);
        lastEvents_ = events_;
        return ret;
    }
    __uint32_t &getEvents() { return events_; }
    __uint32_t getLastEvents() { return lastEvents_; }

    using SP_Channel = std::shared_ptr<Channel>;
};
}  // namespace task
}  // namespace aclolinta
