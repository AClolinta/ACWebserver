/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-21 12:03:37
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-21 12:15:09
 * @FilePath: /ACWebserver/WebServer /utility/thread.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <functional>
#include <memory>
#include <string>

#include "noncopyable.hpp"

