/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-21 11:17:02
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-30 12:46:56
 * @FilePath: /ACWebserver/WebServer/main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <getopt.h>

#include <string>

#include "EventLoop.hpp"
#include "Logging.hpp"
#include "Server.hpp"

using namespace aclolinta::event;
using namespace aclolinta::logger;
using namespace aclolinta::server;

int main(int argc, char *argv[]) {
    int threadNum = 4;
    int port = 80;
    std::string logPath = "./WebServer.log";

    // parse args
    int opt;
    const char *str = "t:l:p:";
    while ((opt = getopt(argc, argv, str)) != -1) {
        switch (opt) {
            case 't': {
                threadNum = atoi(optarg);
                break;
            }
            case 'l': {
                logPath = optarg;
                if (logPath.size() < 2 || optarg[0] != '/') {
                    printf("logPath should start with \"/\"\n");
                    abort();
                }
                break;
            }
            case 'p': {
                port = atoi(optarg);
                break;
            }
            default:
                break;
        }
    }
    Logger::setLogFileName(logPath);
// STL库在多线程上应用
#ifndef _PTHREADS
    LOG << "_PTHREADS is not defined !";
#endif
    EventLoop mainLoop;
    Server myHTTPServer(&mainLoop, threadNum, port);
    myHTTPServer.start();
    mainLoop.loop();
    return 0;
}