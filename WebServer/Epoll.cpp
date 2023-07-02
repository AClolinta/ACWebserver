/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-29 11:35:48
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-07-02 03:27:35
 * @FilePath: /ACWebserver/WebServer/Epoll.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "Epoll.hpp"

#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#include <deque>
#include <queue>

#include "Logging.hpp"
#include "Util.hpp"

const int EVENTSNUM = 4096;
const int EPOLLWAIT_TIME = 10000;

Epoll::Epoll() : epollFd_(epoll_create1(EPOLL_CLOEXEC)), events_(EVENTSNUM) {
    assert(epollFd_ > 0);
}

Epoll::~Epoll() = default;

// register the new fd to epoll
void Epoll::epoll_add(SP_Channel request, int timeout) {
    int fd = request->getFd();
    if (timeout > 0) {
        add_timer(request, timeout);
        fd2http_[fd] = request->getHolder();
    }
    epoll_event event;
    event.data.fd = fd;
    event.events = request->getEvents();

    request->EqualAndUpdateLastEvents();

    fd2chan_[fd] = request;
    if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event) < 0) {
        perror("epoll_add error");
        fd2chan_[fd].reset();
    }
}

// modify the fd in epoll
void Epoll::epoll_mod(SP_Channel request, int timeout) {
    if (timeout > 0) add_timer(request, timeout);
    int fd = request->getFd();
    // Check if the current event is equal to the last event and update the last
    // event
    if (!request->EqualAndUpdateLastEvents()) {
        epoll_event event;
        event.data.fd = fd;
        event.events = request->getEvents();
        if (epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &event) < 0) {
            perror("epoll_mod error");
            fd2chan_[fd].reset();
        }
    }
}
// Delete the fd form epoll
void Epoll::epoll_del(SP_Channel request) {
    int fd = request->getFd();
    epoll_event event;
    event.data.fd = fd;
    event.events = request->getLastEvents();
    if (epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &event) < 0) {
        perror("epoll_del error");
    }
    fd2chan_[fd].reset();
    fd2http_[fd].reset();
}
// return the count of events that are active
std::vector<SP_Channel> Epoll::poll() {
    while (true) {
        // Need to block the thread
        int event_count = epoll_wait(epollFd_, &*events_.begin(),
                                     events_.size(), EPOLLWAIT_TIME);
        if (event_count < 0) perror("epoll wait error");
        std::vector<SP_Channel> req_data = getEventsRequest(event_count);
        if (req_data.size() > 0) return req_data;
    }
}
// Handle the events that are expired
void Epoll::handleExpired() { timerManager_.handleExpiredEvent(); }

// Dispatch the function to handle events
std::vector<SP_Channel> Epoll::getEventsRequest(int events_sum) {
    std::vector<SP_Channel> req_data;
    for (int i = 0; i < events_sum; ++i) {
        // Get the fd  which have a event
        int fd = events_[i].data.fd;
        SP_Channel cur_req = fd2chan_[fd];
        if (cur_req) {
            cur_req->setRevents(events_[i].events);
            cur_req->setEvents(0);
            // Add the current request to the vector
            req_data.push_back(cur_req);
        } else {
            LOG << "SP cur_req is invalid";
        }
    }
    return req_data;
}

void Epoll::add_timer(SP_Channel request_data, int timeout) {
    std::shared_ptr<HttpData> _t = request_data->getHolder();
    if (_t) {
        timerManager_.addTimer(_t, timeout);
    } else {
        LOG << "TIMER ADD FAILL";
    }
}
