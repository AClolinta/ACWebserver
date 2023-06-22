/*
 * @Author: AClolinta AClolinta@gmail.com
 * @Date: 2023-06-21 11:19:19
 * @LastEditors: AClolinta AClolinta@gmail.com
 * @LastEditTime: 2023-06-21 11:20:22
 * @FilePath: /ACWebserver/WebServer /noncopyable.hpp
 * @Description: 不可拷贝的类 */
#pragma once

class noncopyable {
   protected:
    noncopyable() {}
    ~noncopyable() {}

   private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};