/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-21 11:24:51
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-22 09:11:34
 * @FilePath: /ACWebserver/WebServer /utility/MutexLock.hpp
 * @Description: 互斥锁 */
#pragma once

#include <pthread.h>

#include <cstdio>

#include "noncopyable.hpp"

namespace aclolinta {
namespace thread {
class MutexLock : noncopyable {
   public:
    MutexLock(/* args */);
    ~MutexLock();

   public:
    void lock();
    void unlock();

   public:
    pthread_mutex_t *get();

   private:
    pthread_mutex_t m_mutex;

    // 友元类不受访问权限影响
   private:
    friend class Condition;
};

MutexLock::MutexLock(/* args */) { pthread_mutex_init(&m_mutex, NULL); }

MutexLock::~MutexLock() {
    pthread_mutex_lock(&m_mutex);
    pthread_mutex_destroy(&m_mutex);
}

void MutexLock::lock() { pthread_mutex_lock(&m_mutex); }
void MutexLock::unlock() { pthread_mutex_unlock(&m_mutex); }

//一个自动锁，构造时上锁，析构解锁
class MutexLockGuard : noncopyable {
   private:
    /* data */
    MutexLock &m_mutex;

   public:
    explicit MutexLockGuard(MutexLock &_mutex);
    ~MutexLockGuard();
};

explicit MutexLockGuard::MutexLockGuard(MutexLock &_mutex) : m_mutex(_mutex) {
    m_mutex.lock();
}

MutexLockGuard::~MutexLockGuard() { m_mutex.unlock(); }

}  // namespace thread
}  // namespace aclolinta
