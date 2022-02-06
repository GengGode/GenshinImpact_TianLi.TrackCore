//
// Created by GengG on 2022/1/30.
//

#ifndef CVAUTOTRACKDLL_BAILAN_UTIL_SPLITMATCHING_H
#define CVAUTOTRACKDLL_BAILAN_UTIL_SPLITMATCHING_H

#include "../Logger/LoggingFacility.h"

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

class SplitMatching_Paimon  {
    Logger log;
    int err=0;
    std::mutex lm_output;
    std::mutex lm_input;
    cv::Mat _mat;
    cv::Mat _out_mat;
public:
    SplitMatching_Paimon()=delete;
    SplitMatching_Paimon(const Logger& logService);
public:
    void setValue(const cv::Mat &mat);

    cv::Mat getValue();

    bool getSplitMatching();

};

class SplitMatching_MiniMap {
    Logger log;
    int err=0;
    std::mutex lm_output;
    std::mutex lm_input;
    cv::Mat _mat;
    cv::Mat _out_mat;
public:
    SplitMatching_MiniMap()=delete;
    SplitMatching_MiniMap(const Logger& logService);
public:
    void setValue(const cv::Mat &mat);

    cv::Mat getValue();

    bool getSplitMatching();
};

class SplitMatching_UID {
    Logger log;
    int err=0;
    std::mutex lm_output;
    std::mutex lm_input;
    cv::Mat _mat;
    cv::Mat _out_mat;
public:
    SplitMatching_UID()=delete;
    SplitMatching_UID(const Logger& logService);
public:
    void setValue(const cv::Mat &mat);

    cv::Mat getValue();

    bool getSplitMatching();
};

class SplitMatching_LeftGetItems {
    Logger log;
    int err=0;
    std::mutex lm_output;
    std::mutex lm_input;
    cv::Mat _mat;
    cv::Mat _out_mat;
public:
    SplitMatching_LeftGetItems()=delete;
    SplitMatching_LeftGetItems(const Logger& logService);
public:
    void setValue(const cv::Mat &mat);

    cv::Mat getValue();

    bool getSplitMatching();
};

class SplitMatching_RightGetItems {
    Logger log;
    int err=0;
    std::mutex lm_output;
    std::mutex lm_input;
    cv::Mat _mat;
    cv::Mat _out_mat;
public:
    SplitMatching_RightGetItems()=delete;
    SplitMatching_RightGetItems(const Logger& logService);
public:
    void setValue(const cv::Mat &mat);

    cv::Mat getValue();

    bool getSplitMatching();
};


#endif //CVAUTOTRACKDLL_BAILAN_UTIL_SPLITMATCHING_H
