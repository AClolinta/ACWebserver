#include "EventLoop.hpp"

#include <sys/epoll.h>
#include <sys/eventfd.h>

#include <iostream>

#include "Logging.hpp"
#include "Util.hpp"

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
// wake up the eventloop, as the *one*
void EventLoop::wakeup() {
    uint64_t one = 1;
    ssize_t n = writen(wakeupFd_, (char *)(&one), sizeof(one));
    if (n != sizeof(one)) {
        LOG << ("EventLoop::wakeup() writes " + std::to_string(n) +
                " bytes instead of 8");
    }
}

void EventLoop::handleRead() {
    uint64_t one = 1;
    ssize_t n = readn(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one)) {
        LOG << ("EventLoop::handleRead() reads " + std::to_string(n) +
                " bytes instead of 8");
    }
    pwakeupChannel_->setEvents(EPOLLIN | EPOLLET);
}

// Cheack weather the current thread is the thread of the eventloop located
void EventLoop::runInLoop(Functor &&cb) {
    if (isInLoopThread())
        cb();
    else
        queueInLoop(std::move(cb));
}

void EventLoop::queueInLoop(Functor &&cb) {
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.emplace_back(std::move(cb));
    }

    if (!isInLoopThread() || callingPendingFunctors_) wakeup();
}

void EventLoop::loop() {
    assert(!looping_);
    assert(isInLoopThread());
    looping_ = true;
    qiut_ = false;
    std::vector<SP_Channel> ret;
    while (!qiut_) {
        ret.clear();
        ret = poller_->poll();
        eventHandling_ = true;
        for (auto &it : ret) it->handleEvents();
        eventHandling_ = false;
        doPendingFunctors();
        poller_->handleExpired();
    }
    looping_ = false;
}

void EventLoop::doPendingFunctors() {
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;
    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors_);
    }
    // perform the pending functors
    for (auto it = functors.begin(); it != functors.end(); ++it) (*it);
    callingPendingFunctors_ = false;
}

void EventLoop::quit() {
    qiut_ = true;
    if (!isInLoopThread()) wakeup();
}