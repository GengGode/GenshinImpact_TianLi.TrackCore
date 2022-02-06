//
// Created by GengG on 2022/2/6.
//

#ifndef CVAUTOTRACKDLL_BAILAN_UTIL_RESOURCE_H
#define CVAUTOTRACKDLL_BAILAN_UTIL_RESOURCE_H

#include <Windows.h>
#include <opencv2/opencv.hpp>

class BaiLan_Util_Resource
{
public:
    BaiLan_Util_Resource();
    ~BaiLan_Util_Resource();

public:
    cv::Mat PaimonTemplate;
    cv::Mat StarTemplate;
    cv::Mat MapTemplate;
    cv::Mat UID;
    cv::Mat UIDnumber[10];
    
    void install();
    void release();

private:
    void StarRGBA2A();
    void UIDnumberRGBA2A();
};
static BaiLan_Util_Resource BaiLanResource;
#endif //CVAUTOTRACKDLL_BAILAN_UTIL_RESOURCE_H
