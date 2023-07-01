/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-21 12:05:02
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-07-01 03:12:27
 * @FilePath: /ACWebserver/WebServer/utility/CDLatch.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */


#pragma once

#include "Condition.hpp"
#include "MutexLock.hpp"
#include "noncopyable.hpp"

namespace aclolinta {
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

}  // namespace aclolinta