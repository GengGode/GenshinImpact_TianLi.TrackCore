//
// Created by GengG on 2022/2/6.
//

#ifndef CVAUTOTRACKDLL_BAILAN_UTIL_RESOURCE_H
#define CVAUTOTRACKDLL_BAILAN_UTIL_RESOURCE_H

#include <Windows.h>
#include <opencv2/opencv.hpp>

class BaiLan_Util_Resource
{
    BaiLan_Util_Resource();
public:
    BaiLan_Util_Resource(const BaiLan_Util_Resource&) = delete;
    BaiLan_Util_Resource& operator=(const BaiLan_Util_Resource&) = delete;
    static BaiLan_Util_Resource& getInstance()
    {
        static BaiLan_Util_Resource instance;
        return instance;
    }
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
//static BaiLan_Util_Resource *BaiLanResource= BaiLan_Util_Resource::getInstance();
#define BaiLanResource BaiLan_Util_Resource::getInstance()
#endif //CVAUTOTRACKDLL_BAILAN_UTIL_RESOURCE_H
