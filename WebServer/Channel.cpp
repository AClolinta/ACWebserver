/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-25 04:10:39
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-07-01 03:41:22
 * @FilePath: /ACWebserver/WebServer/Channel.cpp
 * @Description:
 *  */
#include "Channel.hpp"

#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <queue>

#include "Epoll.hpp"
#include "EventLoop.hpp"
#include "Util.hpp"



Channel::Channel(EventLoop *loop)
    : loop_(loop), fd_(0), events_(0), revents_(0), lastEvents_(0) {}

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0), lastEvents_(0) {}

Channel::~Channel() = default;

// int Channel::getFd() { return fd_; }
// void Channel::setFd(int fd) { fd_ = fd; }

void Channel::handleRead() {
    if (readHandler_) {
        readHandler_();
    }
}

void Channel::handleWrite() {
    if (writeHandler_) {
        writeHandler_();
    }
}

void Channel::handleConn() {
    if (connHandler_) {
        connHandler_();
    }
}