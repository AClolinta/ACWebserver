/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-22 12:33:21
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-22 12:39:24
 * @FilePath: /ACWebserver/WebServer/utility/Logging.cpp
 * @Description:  */
#include "Logging.hpp"

#include <assert.h>
#include <sys/time.h>
#include <time.h>

#include <iostream>

#include "AsyncLogging.hpp"
#include "CurrentThread.hpp"
#include "Thread.hpp"

using namespace aclolinta::logger;
using namespace aclolinta::thread;

static pthread_once_t once_control_ = PTHREAD_ONCE_INIT;
static AsyncLogging* AsyncLogger_;

std::string Logger::logFileName_ = "./WebServer.log";

// 静态函数，用于初始化异步日志记录器 AsyncLogger_
void once_init() {
    AsyncLogger_ = new AsyncLogging(Logger::getLogFileName());
    AsyncLogger_->start();
}
// 将日志内容追加到异步日志记录器中
void output(const char* msg, int len) {
    pthread_once(&once_control_, once_init);
    AsyncLogger_->append(msg, len);
}

void Logger::Impl::formatTime() {
    struct timeval tv;
    time_t time;
    char str_t[26] = {0};
    gettimeofday(&tv, NULL);
    time = tv.tv_sec;
    struct tm* p_time = localtime(&time);
    strftime(str_t, 26, "%Y-%m-%d %H:%M:%S\n", p_time);
    stream_ << str_t;
}

Logger::Logger(const char* fileName, int line) : impl_(fileName, line) {}

Logger::~Logger() {
    impl_.stream_ << " -- " << impl_.basename_ << ':' << impl_.line_ << '\n';
    const LogStream::Buffer& buf(stream().buffer());
    output(buf.data(), buf.length());
}