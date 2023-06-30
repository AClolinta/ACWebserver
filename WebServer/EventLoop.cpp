#include "EventLoop.hpp"

#include <sys/epoll.h>
#include <sys/eventfd.h>

#include <iostream>

#include "Logging.hpp"
#include "Util.hpp"

using namespace aclolinta::event;

using namespace aclolinta::util;
using namespace aclolinta::logger;

__thread EventLoop *t_loopInThisThread = nullptr;

int createEventfd() {
    int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0) {
        LOG << ("Failed in eventfd");
        abort();
    }
    return evtfd;
}

EventLoop::EventLoop()
    : looping_(false),
      poller_(new Epoll()),
      wakeupFd_(createEventfd()),
      qiut_(false),
      eventHandling_(false),
      mutex_(),
      pendingFunctors_(),
      callingPendingFunctors_(false),
      threadId_(CurrentThread::tid()),
      pwakeupChannel_(new Channel(this, wakeupFd_)) {
    if (t_loopInThisThread) {
        LOG << ("Another EventLoop exists in this thread");
    } else {
        t_loopInThisThread = this;
    }
    pwakeupChannel_->setEvents(EPOLLIN | EPOLLET);
    pwakeupChannel_->setReadHandler(std::bind(&EventLoop::handleRead, this));
    pwakeupChannel_->setConnHandler(std::bind(&EventLoop::handleConn, this));
    poller_->epoll_add(pwakeupChannel_, 0);
}

void EventLoop::handleConn() { updatePoller(pwakeupChannel_, 0); }

EventLoop::~EventLoop() {
    close(wakeupFd_);
    t_loopInThisThread = nullptr;
}

void EventLoop::wakeup() {
    uint64_t one = 1;
    ssize_t n = writen(wakeupFd_, (char *)(&one), sizeof(one));
    if (n != sizeof(one)) {
        LOG << ("EventLoop::wakeup() writes " + std::to_string(n) +
                " bytes instead of 8");
    }
}
