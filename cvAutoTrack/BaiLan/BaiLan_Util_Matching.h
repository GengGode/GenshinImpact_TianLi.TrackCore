//
// Created by GengG on 2022/2/6.
//

#ifndef CVAUTOTRACKDLL_BAILAN_UTIL_MATCHING_H
#define CVAUTOTRACKDLL_BAILAN_UTIL_MATCHING_H

#include "../Logger/LoggingFacility.h"

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

class Matching_Paimon
{
    Logger log;
    int err=0;
    std::mutex lm_output;
    std::mutex lm_input;
    cv::Mat _mat;
    bool _out_value;
public:
    Matching_Paimon()=delete;
    Matching_Paimon(const Logger& logService);
public:
    void setValue(const cv::Mat &mat);
    
    bool getValue();
    
    bool getMatchOutput();
};

class Matching_MiniMap
{
    Logger log;
    int err=0;
    std::mutex lm_output;
    std::mutex lm_input;
    cv::Mat _mat;
    cv::Point2d _out_value;
public:
    Matching_MiniMap()=delete;
    Matching_MiniMap(const Logger& logService);
public:
    void setValue(const cv::Mat &mat);
    
    cv::Point2d getValue();
    
    bool getMatchOutput();
};

#endif //CVAUTOTRACKDLL_BAILAN_UTIL_MATCHING_H
