/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-23 11:29:41
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-25 04:09:38
 * @FilePath: /ACWebserver/WebServer/Channel.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include <sys/epoll.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "Timer.hpp"

namespace aclolinta {
namespace task {
    using SP_CHannel = std::shared_ptr<Channel>;

class EventLoop;
class HttpData;

class Channel {
   private:
    /* data */
    using CallBack = std::function<void()>;
    EventLoop *loop_;
    int fd_;
    __uint32_t events_;
    __uint32_t revents_;
    __uint32_t lastEvents_;
    // 方便找到上层持有该Channel的对象
    std::weak_ptr<HttpData> holder_;

   public:
    Channel(/* args */);
    ~Channel();

   public:
    int getFd() { return fd_; }
    void setFd(int fd) { fd_ = fd; }

    void setHolder(std::shared_ptr<HttpData> holder) { holder_ = holder; }
    std::shared_ptr<HttpData> getHolder() {
        std::shared_ptr<HttpData> ret(holder_.lock());
        return ret;
    }

    void setReadHandler(CallBack &&readHandler) { readHandler_ = readHandler; }

    void serWriteHandler(CallBack &&writeHandler) {
        writeHandler_ = writeHandler;
    }

    void setErrorHandler(CallBack &&errorHandler) {
        errorHandler_ = errorHandler;
    }

    void setConnHandler(CallBack &&connHandler) { connHandler_ = connHandler; }

    //处理逻辑
    void handleEvents(){
        events_ = 0;
        if((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)){
            events_ = 0;
            return;
        }
        if(revents_ & EPOLLERR){
            if(errorHandler_) errorHandler_();
            events_ = 0;
            return;
        }
        if(revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)){
            handleRead();
        }
        if(revents_ & EPOLLOUT){
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
    

    bool EqualAndUpdateLastEvents() {
        bool ret = (lastEvents_ == events_);
        lastEvents_ = events_;
        return ret;
    }
    __uint32_t &getEvents() { return events_; }
    __uint32_t getLastEvents() { return lastEvents_; }

   private:
    int parse_URI();
    int parse_Headers();
    int analysisRequest();

   private:
    CallBack readHandler_;
    CallBack writeHandler_;
    CallBack errorHandler_;
    CallBack connHandler_;
};
}  // namespace task
}  // namespace aclolinta
