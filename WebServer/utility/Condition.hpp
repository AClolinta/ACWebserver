/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-21 11:26:11
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-21 11:56:16
 * @FilePath: /ACWebserver/WebServer /utility/Condition.hpp
 * @Description: 条件变量 */
#pragma once
#include <errno.h>
#include <pthread.h>
#include <time.h>

#include <cstdint>

#include "MutexLock.hpp"
#include "noncopyable.hpp"

class Condition : noncopyable {
   private:
    /* data */
    MutexLock &m_mutex;
    pthread_cond_t m_cond;

   public:
    explicit Condition(MutexLock &mutex) : m_mutex(mutex) {
        pthread_cond_init(&m_cond, NULL);
    }
    ~Condition() { pthread_cond_destroy(&m_cond); }

   public:
    void wait() { pthread_cond_wait(&m_cond, m_mutex.get()); }
    void notify() { pthread_cond_signal(&m_cond); }
    void notifyAll() { pthread_cond_broadcast(&m_cond); }
    bool waitForSeconds(size_t seconds) {
        struct timespec abstime_;
        clock_gettime(CLOCK_REALTIME, &abstime_);
        abstime_.tv_sec += static_cast<time_t>(seconds);
        return ETIMEDOUT ==
               pthread_cond_timedwait(&m_cond, m_mutex.get(), &abstime_);
    }
};

  // namespace aclolinta