#pragma once

#include <sys/epoll.h>

#include <functional>
#include <memory>
#include <string>
#include <map>

#include "Timer.hpp"

namespace aclolinta{
    // namespace cha
}

class EventLoop;
class HttpData;

class Channel
{

public:
    using CallBack = std::function<void()>;
    Channel(/* args */);
    ~Channel();

   private:
    /* data */
};

