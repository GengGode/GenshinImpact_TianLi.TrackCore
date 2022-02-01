#pragma once
#include "TimerFacility.h"
#include <string>
#include <time.h>
#include <chrono>

class SystemTimer
{
public:
    SystemTimer() {}
    virtual std::string now()
    {
        return getCurrentTime();
    }
    // 引用自知乎，有改动
    // https://www.zhihu.com/question/21352455/answer/23011807
    // 获取当前时间
    // 头文件 #include <time.h> <chrono>
    std::string getCurrentTime() {
        auto nowtime   = std::chrono::system_clock::now();
        auto millitime = std::chrono::duration_cast<std::chrono::milliseconds>(nowtime.time_since_epoch());
        auto sectime   = std::chrono::duration_cast<std::chrono::seconds>(millitime);
        int32_t milltime = millitime.count() % 1000;

        std::time_t timet = sectime.count();
        struct tm curtime;
        // localtime_r(&timet, &curtime); // 无法编译，修改后如下
        localtime_s( &curtime,&timet);

        char buffer[64];
        static const char *timeFormat = "%4d-%02d-%02d %02d:%02d:%02d.%03d";
        sprintf(buffer, timeFormat, curtime.tm_year + 1900, curtime.tm_mon + 1,
                curtime.tm_mday, curtime.tm_hour, curtime.tm_min, curtime.tm_sec, milltime);
        return std::string(buffer);
    }
};
