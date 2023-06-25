/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-23 11:30:48
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-25 03:16:13
 * @FilePath: /ACWebserver/WebServer/Util.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include <cstdlib>
#include <string>

namespace aclolinta{
    namespace util{
    ssize_t readn(int fd, void *buff, size_t n);
    ssize_t readn(int fd, std::string &inBuffer, bool &zero);
    ssize_t readn(int fd, std::string &inBuffer);
    ssize_t writen(int fd, void *buff, size_t n);
    ssize_t writen(int fd, std::string &sbuff);

    void handle_for_sigpipe();
    int setSocketNonBlocking(int fd);
    void setSocketNodelay(int fd);
    void setSocketNoLinger(int fd);
    void shutDownWR(int fd);
    int socket_bind_listen(int port);
    }
}

