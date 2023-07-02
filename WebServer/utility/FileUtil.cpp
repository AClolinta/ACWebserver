/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-21 13:36:56
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-22 09:07:45
 * @FilePath: /ACWebserver/WebServer /utility/FileUtil.cpp
 * @Description:  */
#include "FileUtil.hpp"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

/// @brief  用户提供缓冲区,以追加方式写入
/// @param filename
AppendFile::AppendFile(std::string filename) : fp_(fopen(filename.c_str(), "ae")) {

    setbuffer(fp_, buffer_, sizeof buffer_);
}
AppendFile::~AppendFile() { fclose(fp_); }

/// @brief 要写入的数据的指针（logline），每个数据项的大小（1字节），要写入的数据项的个数（len），以及要写入的文件指针（fp_）。
/// @param logline
/// @param len
/// @return
size_t AppendFile::write(const char* logline, size_t len) {
    // 不使用文件锁定机制
    return fwrite_unlocked(logline, 1, len, fp_);
}

void AppendFile::append(const char* logline, const size_t len){
    size_t n_ = this->write(logline,len);
    size_t remain_line = len - n_;
    while(remain_line > 0){
        size_t x_ = this->write(logline + n_, len);
        if(x_ ==0 ){
            int err = ferror(fp_);
            if (err) fprintf(stderr, "AppendFile::append() failed !\n");
            break;
        }
        n_ += x_;
        remain_line = len - n_;
    }
}

void AppendFile::flush() { fflush(fp_); }
