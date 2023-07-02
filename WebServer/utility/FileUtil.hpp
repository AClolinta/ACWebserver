/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-21 13:27:05
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-21 13:38:04
 * @FilePath: /ACWebserver/WebServer /utility/FileUtil.hpp
 * @Description:
 * FileUtil是最底层的文件类，封装了Log文件的打开、写入并在类析构的时候关闭文件，底层使用了标准IO，该append函数直接向文件写。
 */
#pragma once

#include <string>
#include <string_view>

#include "noncopyable.hpp"

class AppendFile : noncopyable {
   public:
    explicit AppendFile(std::string filename);
    ~AppendFile();

   public:
    void append(const char *logline, const size_t len);
    void flush();

   private:
    size_t write(const char *logline, size_t len);

   private:
    /* data */
    FILE *fp_;
    char buffer_[64 * 1024];
};

  // namespace aclolinta