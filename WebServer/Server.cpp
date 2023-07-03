/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-30 12:29:48
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-07-03 02:37:23
 * @FilePath: /ACWebserver/WebServer/Server.cpp
 * @Description:  */
#include "Server.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <functional>

#include "Logging.hpp"
#include "Util.hpp"

Server::Server(EventLoop* loop, int threadNum, int port)
    : loop_(loop),
      threadNum_(threadNum),
      eventLoopThreadPool_(new EventLoopThreadPool(loop_, threadNum)),
      started_(false),
      acceptChannel_(new Channel(loop_)),
      port_(port),
      listenFd_(socket_bind_listen(port_)) {
    acceptChannel_->setFd(listenFd_);
    handle_for_sigpipe();
    if (setSocketNonBlocking(listenFd_) < 0) {
        perror("set socket non block failed");
        abort();
    }
}

void Server::start() {
    eventLoopThreadPool_->start();
    acceptChannel_->setEvents(EPOLLIN | EPOLLET);
    acceptChannel_->setReadHandler(std::bind(&Server::handNewConn, this));
    acceptChannel_->setConnHandler(std::bind(&Server::handThisConn, this));
    loop_->addToPoller(acceptChannel_, 0);
    started_ = true;
}

void Server::handNewConn() {
    sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(sockaddr_in));
    socklen_t client_addr_len = sizeof(client_addr);
    int accept_fd = 0;
    while ((accept_fd = accept(listenFd_, (sockaddr*)&client_addr,
                               &client_addr_len)) > 0) {
        EventLoop* loop = eventLoopThreadPool_->getNextLoop();
        LOG << "New connection from " << inet_ntoa(client_addr.sin_addr) << ":"
            << ntohs(client_addr.sin_port);
        // Limit the max number of connections
        if (accept_fd >= MAXFDS) {
            close(accept_fd);
            continue;
        }
        // set socket non block
        // LOG<<"SetSocketNonBlocking!";
        if (setSocketNonBlocking(accept_fd) < 0) {
            LOG << "Set non block failed!";
            return;
        }
        setSocketNodelay(accept_fd);
        // LOG << "SetSocketNodelay!";
        //bind channel and httpdata
        std::shared_ptr<HttpData> req_info(new HttpData(loop, accept_fd));
        req_info->getChannel()->setHolder(req_info);
        // put into the eventloop
        loop->queueInLoop(std::bind(&HttpData::newEvent, req_info));
    }
    acceptChannel_->setEvents(EPOLLIN | EPOLLET);
}