//
// Created by GengG on 2022/1/28.
//

#ifndef CVAUTOTRACKDLL_SCREENSHOT_H
#define CVAUTOTRACKDLL_SCREENSHOT_H
#include "../../Logger/LoggingFacility.h"
#include <opencv2/opencv.hpp>
#include <windows.h>

// 截图类
class Screenshot {
    HWND giHandle;
    RECT giRect;
    RECT giClientRect;
    cv::Size giClientSize;

    double screen_scale;
    int err;
    bool result=false;
    std::mutex lm_output;
    std::mutex lm_input;
    cv::Mat giFrame;

    Logger log;
public:
    //Screenshot()=delete;
    Screenshot(const Logger& logService);
    ~Screenshot();

    void work();
    void setValue(const HWND& handle)
    {
        std::lock_guard<std::mutex> lock(lm_input);
        giHandle=handle;
    }
    cv::Mat getValue(){
        std::lock_guard<std::mutex> lock(lm_output);
        cv::Mat ret=giFrame.clone();
        return ret;
    }
    //void signal();
private:
    bool screenshot();
    void getGenshinImpactScale();
};


#endif //CVAUTOTRACKDLL_SCREENSHOT_H
