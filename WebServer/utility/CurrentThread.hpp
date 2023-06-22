/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-21 11:58:18
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-21 12:53:25
 * @FilePath: /ACWebserver/WebServer /utility/CurrentThread.hpp
 * @Description: 记录线程信息*/
#pragma once

#include <stdint.h>

namespace aclolinta {
namespace thread {
namespace CurrentThread {
// internal
extern __thread int t_cachedTid;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;
extern __thread const char* t_threadName;
void cacheTid();
inline int tid() {
    if (__builtin_expect(t_cachedTid == 0, 0)) {
        cacheTid();
    }
    return t_cachedTid;
}

inline const char* tidString()  // for logging
{
    return t_tidString;
}

inline int tidStringLength()  // for logging
{
    return t_tidStringLength;
}

inline const char* name() { return t_threadName; }
}  // namespace CurrentThread
}  // namespace thread
}  // namespace aclolinta