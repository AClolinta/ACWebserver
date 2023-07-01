/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-22 08:58:57
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-22 09:07:47
 * @FilePath: /ACWebserver/WebServer/utility/LogFile.hpp
 * @Description:
 * LogFile进一步封装了FileUtil，并设置了一个循环次数，没过这么多次就flush一次
 * */
#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "FileUtil.hpp"
#include "MutexLock.hpp"
#include "noncopyable.hpp"

using namespace aclolinta::thread;

namespace aclolinta {
class LogFile {
   private:
    /* data */
    const std::string basename_;
    const int flushEveryN_;

    int count_;
    std::unique_ptr<MutexLock> mutex_;
    std::unique_ptr<AppendFile> file_;

   private:
    void append_unlocked(const char* logline, int len);

   public:
    LogFile(std::string_view basename, int flushEveryN = 1024);
    ~LogFile();

   public:
    void append(const char* logline, int len);
    void flush();
    bool rollFile();
};
}  // namespace aclolinta
