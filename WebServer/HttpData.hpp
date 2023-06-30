/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-22 12:51:04
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-30 11:50:48
 * @FilePath: /ACWebserver/WebServer/HttpData.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include <sys/epoll.h>
#include <unistd.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "Timer.hpp"

// using namespace aclolinta::task;

namespace aclolinta {
namespace http {

class aclolinta::event::EventLoop;
class aclolinta::timer::TimerNode;
class aclolinta::task::Channel;
// class HttpData;
// class MimeType;
// 状态代码

enum ProcessState {
    STATE_PARSE_URI = 1,
    STATE_PARSE_HEADERS,
    STATE_RECV_BODY,
    STATE_ANALYSIS,
    STATE_FINISH
};

enum URIState {
    PARSE_URI_AGAIN = 1,
    PARSE_URI_ERROR,
    PARSE_URI_SUCCESS,
};

enum HeaderState {
    PARSE_HEADER_SUCCESS = 1,
    PARSE_HEADER_AGAIN,
    PARSE_HEADER_ERROR
};

enum AnalysisState { ANALYSIS_SUCCESS = 1, ANALYSIS_ERROR };

enum ParseState {
    H_START = 0,
    H_KEY,
    H_COLON,
    H_SPACES_AFTER_COLON,
    H_VALUE,
    H_CR,
    H_LF,
    H_END_CR,
    H_END_LF
};

enum ConnectionState { H_CONNECTED = 0, H_DISCONNECTING, H_DISCONNECTED };

enum HttpMethod { METHOD_POST = 1, METHOD_GET, METHOD_HEAD };

enum HttpVersion { HTTP_10 = 1, HTTP_11 };

//

class HttpData : public std::enable_shared_from_this<HttpData> {
   public:
    HttpData(EventLoop *loop, int connfd);
    ~HttpData() { close(fd_); }

   public:
    void reset();
    void seperateTimer();
    void linkTimer(std::shared_ptr<TimerNode> mtimer) {
        // shared_ptr重载了bool, 但weak_ptr没有
        timer_ = mtimer;
    }
    std::shared_ptr<Channel> getChannel() { return channel_; }
    EventLoop *getLoop() { return loop_; }
    void handleClose();
    void newEvent();

   private:
    void handleRead();
    void handleWrite();
    void handleConn();
    void handleError(int fd, int err_num, std::string short_msg);
    URIState parseURI();
    HeaderState parseHeaders();
    AnalysisState analysisRequest();

   private:
    /* data */
    EventLoop *loop_;
    std::shared_ptr<Channel> channel_;
    int fd_;
    std::string inBuffer_;
    std::string outBuffer_;
    bool error_;
    ConnectionState connectionState_;

    HttpMethod method_;
    HttpVersion HTTPVersion_;
    std::string fileName_;
    std::string path_;
    int nowReadPos_;
    ProcessState state_;
    ParseState hState_;
    bool keepAlive_;
    std::map<std::string, std::string> headers_;
    std::weak_ptr<TimerNode> timer_;
};

class MimeType {
   private:
    MimeType(/* args */);
    MimeType(const MimeType &m);
    ~MimeType();

   public:
    static std::string getMime(const std::string &suffix);

   private:
    static void init();
    static std::map<std::string, std::string> mime;

   private:
    /* data */
    // 只创建一次
    static pthread_once_t once_control;
};

}  // namespace http
}  // namespace aclolinta
