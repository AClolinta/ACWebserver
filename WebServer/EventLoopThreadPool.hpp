/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-30 11:55:23
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-30 11:56:58
 * @FilePath: /ACWebserver/WebServer/EventLoopThreadPool.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include <memory>
#include <vector>
#include "EventLoopThread.hpp"
#include "Logging.hpp"
#include "noncopyable.hpp"

using namespace aclolinta::thread;
using namespace aclolinta::event;
using namespace aclolinta::logger;