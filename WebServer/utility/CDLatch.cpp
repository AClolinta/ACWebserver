/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-21 12:08:48
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-07-01 03:12:26
 * @FilePath: /ACWebserver/WebServer /utility/CDLatch.cpp
 * @Description: 一个计时的锁 */
#include "CDLatch.hpp"

using namespace aclolinta;

CDLatch::CDLatch(size_t count) : m_mutex(), m_cond(m_mutex), m_count(count){};

void CDLatch::wait() {
    MutexLockGuard lock(m_mutex);
    while (m_count > 0) m_cond.wait();
}

void CDLatch::countdown() {
    MutexLockGuard lock(m_mutex);
    --m_count;
    if (0 == m_count) m_cond.notifyAll();
}