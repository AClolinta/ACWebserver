
/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-22 12:45:22
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-07-01 03:43:08
 * @FilePath: /ACWebserver/WebServer/Time.hpp
 * @Description: 计时器 */
#pragma once
#include <unistd.h>

#include <deque>
#include <memory>
#include <queue>

#include "HttpData.hpp"
#include "MutexLock.hpp"
#include "noncopyable.hpp"



namespace aclolinta {
// class aclolinta::http::HttpData;

class HttpData;

class TimerNode {
   public:
    TimerNode(std::shared_ptr<HttpData> requestData, int timeout);
    TimerNode(TimerNode &tn);
    ~TimerNode();

   public:
    void update(size_t timeout);
    bool isValid();
    void clearReq();
    void setDeleted() { deleted_ = true; }
    bool isDeleted() const { return deleted_; }
    size_t getExpTIme() const { return expiredTime_; }

   private:
    /* data */
    bool deleted_;
    size_t expiredTime_;
    std::shared_ptr<HttpData> SPHttpData;
};

struct TimerCMP {
    /* data */
    bool operator()(std::shared_ptr<TimerNode> &a,
                    std::shared_ptr<TimerNode> &b) const {
        return a->getExpTIme() > b->getExpTIme();
    }
};

class TimerManager {
   public:
    TimerManager(/* args */) = default;
    ~TimerManager() = default;

   public:
    void addTimer(std::shared_ptr<HttpData> SPHttpData, int timeout);
    void handleExpiredEvent();

   private:
    /* data */
    using SPTimerNode = std::shared_ptr<TimerNode>;
    std::priority_queue<SPTimerNode, std::deque<SPTimerNode>, TimerCMP>
        timerNodeQueue;
    // MutexLock lock;
};

}  // namespace aclolinta
