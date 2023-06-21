/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-21 12:42:29
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-21 13:18:58
 * @FilePath: /ACWebserver/WebServer /utility/Thread.cpp
 * @Description:线程类的实现 */

#include "Thread.hpp"
using namespace aclolinta::thread;

#include <assert.h>
#include <errno.h>
#include <linux/unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <memory>

#include "CurrentThread.hpp"

// 外部定义
namespace aclolinta::thread::CurrentThread {
__thread int t_cachedTid = 0;
__thread char t_tidString[32];
__thread int t_tidStringLength = 6;
__thread const char* t_threadName = "default";
}  // namespace aclolinta::thread::CurrentThread

// 获取当前线程的线程ID（tid）
pid_t gettid() {
    return static_cast<pid_t>(::syscall(SYS_gettid));
}  // 获取当前线程的线程ID（tid）

void aclolinta::thread::CurrentThread::cacheTid() {
    if (t_cachedTid == 0) {
        t_cachedTid = gettid();
        t_tidStringLength =
            snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
    }
}
// 为了在线程中保留name,tid数据
struct ThreadData {
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc func_;
    std::string name_;
    pid_t* tid_;
    CDLatch* latch_;

    ThreadData(const ThreadFunc& func, const std::string& name, pid_t* tid,
               CDLatch* latch)
        : func_(func), name_(name), tid_(tid), latch_(latch) {}

    void runInThread() {
        *tid_ = CurrentThread::tid();
        tid_ = NULL;
        latch_->countdown();
        latch_ = NULL;

        CurrentThread::t_threadName = name_.empty() ? "Thread" : name_.c_str();
        prctl(PR_SET_NAME, CurrentThread::t_threadName);

        func_();
        CurrentThread::t_threadName = "finished";
    }
};
void* startThread(void* obj) {
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return NULL;
}
Thread::Thread(const ThreadFunc& func, const std::string& n)
    : started_(false),
      joined_(false),
      pthreadId_(0),
      tid_(0),
      func_(func),
      name_(n),
      latch_(1) {
    set_default_name();
}
Thread::~Thread() {
    if (started_ && !joined_) pthread_detach(pthreadId_);
}

void Thread::set_default_name() {
    if (name_.empty()) {
        char buf[32];
        snprintf(buf, sizeof buf, "Thread");
        name_ = buf;
    }
}

void Thread::start() {
    assert(!started_);
    started_ = true;
    ThreadData* data = new ThreadData(func_, name_, &tid_, &latch_);
    if (pthread_create(&pthreadId_, NULL, &startThread, data)) {
        started_ = false;
        delete data;
    } else {
        latch_.wait();
        assert(tid_ > 0);
    }
}

int Thread::join() {
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadId_, NULL);
}