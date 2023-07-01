/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-30 10:54:41
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-07-01 03:16:54
 * @FilePath: /ACWebserver/WebServer/EventLoopThread.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-30 10:54:41
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-30 11:57:46
 * @FilePath: /ACWebserver/WebServer/EventLoopThread.hpp
 * @Description:  */
#pragma once

#include "Condition.hpp"
#include "EventLoop.hpp"
#include "MutexLock.hpp"
#include "Thread.hpp"
#include "noncopyable.hpp"


namespace aclolinta {

class EventLoopThread : public noncopyable {
   public:
    EventLoopThread(/* args */);
    ~EventLoopThread();

    EventLoop *startLoop();

   private:
    /* data */
    void threadFunc();
    EventLoop *loop_;
    bool exiting_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
};
}  // namespace aclolinta