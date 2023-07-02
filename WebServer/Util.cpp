/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-24 10:06:39
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-07-02 03:20:23
 * @FilePath: /ACWebserver/WebServer/Util.cpp
 * @Description: 提供IO的工具*/
#include "Util.hpp"

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace aclolinta;

const int MAX_BUFF = 4096;

/// @brief 从fd读取数据放到缓冲区
/// @param fd 文件描述符
/// @param buff 缓冲区指针
/// @param n 读取的大小
/// @return 读取结果
ssize_t readn(int fd, void *buff, size_t n) {
    size_t nleft = n;     // 剩余需要读取的字节数
    ssize_t nread = 0;    // 每次读取的字节数
    ssize_t readSum = 0;  // 已经读取的总字节数
    char *ptr = (char *)buff;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0;  // 读取被中断
            else if (errno == EAGAIN)
                return readSum;  // 非阻塞模式下没有数据可读
            else
                return -1;  // 其他错误
        } else if (0 == nread)
            break;

        readSum += nread;
        nleft -= nread;
        ptr += nread;
    }
    return readSum;
}

/// @brief 从fd读取数据放到缓冲区
/// @param fd 文件描述符
/// @param inBuffer 缓冲区
/// @param zero 是否已经读取到文件末尾
/// @return 读取状态
ssize_t readn(int fd, std::string &inBuffer, bool &zero) {
    ssize_t nread = 0;
    ssize_t readSum = 0;

    while (true) {
        /* code */
        char buff[MAX_BUFF];
        if ((nread = read(fd, buff, MAX_BUFF)) < 0) {
            if (errno == EINTR)
                continue;
            else if (errno == EAGAIN) {
                return readSum;
            } else {
                perror("READ ERROR");
                return -1;
            }
        } else if (nread == 0) {
            zero = true;  // 读取完成
            break;
        }
        readSum += nread;
        inBuffer += std::string(buff, buff + nread);
    }
    return readSum;
}
/// @brief 从fd读取数据放到缓冲区
/// @param fd 文件描述符
/// @param inBuffer
/// @return 读取状态
ssize_t readn(int fd, std::string &inBuffer) {
    ssize_t nread = 0;
    ssize_t readSum = 0;
    while (true) {
        char buff[MAX_BUFF];
        if ((nread = read(fd, buff, MAX_BUFF)) < 0) {
            if (errno == EINTR)
                continue;
            else if (errno == EAGAIN) {
                return readSum;
            } else {
                perror("read error");
                return -1;
            }
        } else if (nread == 0) {
            break;
        }
        readSum += nread;
        inBuffer += std::string(buff, buff + nread);
    }
    return readSum;
}
/// @brief 向fd写入数据
/// @param fd 文件描述符
/// @param buff 缓冲区
/// @param n 写入的大小
/// @return 写入状态
ssize_t writen(int fd, void *buff, size_t n) {
    size_t nleft = n;
    ssize_t nwritten = 0;
    ssize_t writeSum = 0;
    char *ptr = (char *)buff;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0) {
                if (errno == EINTR) {
                    // 中断
                    nwritten = 0;
                    continue;
                } else if (errno == EAGAIN) {
                    // 无数据
                    return writeSum;
                } else
                    return -1;
            }
        }
        writeSum += nwritten;
        nleft -= nwritten;
        ptr += nwritten;
    }
    return writeSum;
}
/// @brief 向FD写入缓冲区的数据
/// @param fd
/// @param sbuff string类型的buff
/// @return 写入状态
ssize_t writen(int fd, std::string &sbuff) {
    size_t nleft = sbuff.size();
    ssize_t nwritten = 0;
    ssize_t writeSum = 0;
    const char *ptr = sbuff.c_str();
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0) {
                if (errno == EINTR) {
                    nwritten = 0;
                    continue;
                } else if (errno == EAGAIN)
                    break;
                else
                    return -1;
            }
        }
        writeSum += nwritten;
        nleft -= nwritten;
        ptr += nwritten;
    }
    if (writeSum == static_cast<int>(sbuff.size()))
        // 写入完成了，清空缓冲区
        sbuff.clear();
    else
        sbuff = sbuff.substr(writeSum);
    return writeSum;
}
// 处理 SIGPIPE 信号，阻止因为SIGPIPE信号而意外终止
void handle_for_sigpipe() {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    if (sigaction(SIGPIPE, &sa, NULL)) return;
}

// socket设置为非阻塞模式，使得在执行套接字操作时能够立即返回，而不会阻塞等待
int setSocketNonBlocking(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    if (flag == -1) return -1;

    flag |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flag) == -1) return -1;
    return 0;
}
// 禁用Nagle
void setSocketNodelay(int fd) {
    int enable = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&enable, sizeof(enable));
}
// 延迟关闭socket
void setSocketNoLinger(int fd) {
    struct linger linger_;
    linger_.l_onoff = 1;
    linger_.l_linger = 30;
    setsockopt(fd, SOL_SOCKET, SO_LINGER, (const char *)&linger_,
               sizeof(linger_));
}

void shutDownWR(int fd) {
    shutdown(fd, SHUT_WR);
    // printf("shutdown\n");
}

int socket_bind_listen(int port) {
    // 检查port值，取正确区间范围
    if (port < 0 || port > 65535) return -1;

    // 创建socket(IPv4 + TCP)，返回监听描述符
    int listen_fd = 0;
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) return -1;

    // 消除bind时"Address already in use"错误
    int optval = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval,
                   sizeof(optval)) == -1) {
        close(listen_fd);
        return -1;
    }

    // 设置服务器IP和Port，和监听描述副绑定
    struct sockaddr_in server_addr;
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)port);
    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
        -1) {
        close(listen_fd);
        return -1;
    }

    // 开始监听，最大等待队列长为LISTENQ
    if (listen(listen_fd, 2048) == -1) {
        close(listen_fd);
        return -1;
    }

    // 无效监听描述符
    if (listen_fd == -1) {
        close(listen_fd);
        return -1;
    }
    return listen_fd;
}