

#pragma once

#include "Condition.hpp"
#include "MutexLock.hpp"
#include "noncopyable.hpp"

namespace aclolinta {
namespace thread {
// CountDownLatch的主要作用是确保Thread中传进去的func真的启动了以后
// 外层的start才返回
class CDLatch : noncopyable {
   private:
    /* data */
    mutable MutexLock m_mutex;
    Condition m_cond;
    size_t m_count;

   public:
    explicit CDLatch(size_t count);

   public:
   void wait();
   void countdown();
};

}  // namespace thread
}  // namespace aclolinta