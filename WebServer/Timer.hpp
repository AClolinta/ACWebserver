
/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-22 12:45:22
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-22 12:51:45
 * @FilePath: /ACWebserver/WebServer/Time.hpp
 * @Description: 计时器 */
#pragma
#include <unistd.h>

#include <deque>
#include <memory>

#include "HttpData.hpp"
#include "MutexLock.hpp"
#include "noncopyable.hpp"

class HttpData;

class TimerNode
{
private:
    /* data */
public:
    TimerNode(/* args */);
    ~TimerNode();
};

