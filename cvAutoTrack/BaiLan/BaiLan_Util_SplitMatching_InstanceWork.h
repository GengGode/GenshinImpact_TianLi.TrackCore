//
// Created by GengG on 2022/1/30.
//

#ifndef CVAUTOTRACKDLL_BAILAN_UTIL_SPLITMATCHING_INSTANCEWORK_H
#define CVAUTOTRACKDLL_BAILAN_UTIL_SPLITMATCHING_INSTANCEWORK_H
#include <opencv2/opencv.hpp>

bool            getSplitMatching_Paimon(cv::Mat& mat,cv::Rect& rect) {
    if(mat.empty())
    {
        return false;
    }

    return true;
}
bool           getSplitMatching_MiniMap(cv::Mat& mat,cv::Rect& rect) {
    if(mat.empty())
    {
        return false;
    }

    return true;
}
bool               getSplitMatching_UID(cv::Mat& mat,cv::Rect& rect) {
    if(mat.empty())
    {
        return false;
    }

    return true;
}
bool      getSplitMatching_LeftGetItems(cv::Mat& mat,cv::Rect& rect) {
    if(mat.empty())
    {
        return false;
    }

    return true;
}
bool     getSplitMatching_RightGetItems(cv::Mat& mat,cv::Rect& rect) {
    if(mat.empty())
    {
        return false;
    }

    return true;
}

#endif //CVAUTOTRACKDLL_BAILAN_UTIL_SPLITMATCHING_INSTANCEWORK_H
