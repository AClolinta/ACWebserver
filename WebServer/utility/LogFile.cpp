/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-22 09:06:11
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-07-01 04:19:41
 * @FilePath: /ACWebserver/WebServer/utility/LogFile.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "LogFile.hpp"

#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "FileUtil.hpp"

using namespace aclolinta;

LogFile::LogFile(std::string_view basename, int flushEveryN )
    : basename_(basename),
      flushEveryN_(flushEveryN),
      count_(0),
      mutex_(new MutexLock) {
    // 要存储的新指针
    file_.reset(new AppendFile(basename.data()));
}
LogFile::~LogFile() = default;

void LogFile::append(const char* logline, int len) {
    MutexLockGuard lock_(*mutex_);
    append_unlocked(logline, len);
}

void LogFile::append_unlocked(const char* logline, int len) {
    file_->append(logline, len);
    ++count_;
    if (count_ >= flushEveryN_) {
        count_ = 0;
        file_->flush();
    }
}

void LogFile::flush() {
    MutexLockGuard lock(*mutex_);
    file_->flush();
}