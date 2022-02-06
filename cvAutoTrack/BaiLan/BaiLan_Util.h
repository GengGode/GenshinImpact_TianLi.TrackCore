//
// Created by GengG on 2022/1/30.
//

#ifndef CVAUTOTRACKDLL_BAILAN_UTIL_H
#define CVAUTOTRACKDLL_BAILAN_UTIL_H
//干活的类
#include "../GenshinFunction/HookHandle/HookHandle.h"
#include "../GenshinFunction/Screenshot/Screenshot.h"
#include "BaiLan_Util_SplitArea.h"
#include "BaiLan_Util_SplitMatching.h"
#include "BaiLan_Util_Matching.h"
// 日志相关
#include "../Logger/LoggingFacility.h"
#include "../Logger/StandardOutputLogger.h"

// opencv依赖
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

static Logger logger = std::make_shared<StandardOutputLogger>();

static bool Get_GI_Handle(HWND& giHandle)
{
    static HookHandle* hookHandle = new HookHandle(logger);
    if(hookHandle->getGenshinImpactHandle())
    {
        hookHandle->getValue(giHandle);
        return true;
    }
    else
    {
        return false;
    }
}

static bool Get_GI_Frame(const HWND &giHandle,cv::Mat& giFrame)
{
    static Screenshot* screenshot = new Screenshot(logger);
    screenshot->setValue(giHandle);
    //screenshot->work();
    if(screenshot->screenshot())
    {
        giFrame=screenshot->getValue();
        return true;
    }
    else
    {
        return false;
    }
}

static bool Get_GI_SplitArea(const cv::Mat &giFrame,std::vector<cv::Mat> &giSplitArea)
{
    static SplitArea* splitArea = new SplitArea(logger);
    splitArea->setValue(giFrame);;
    if(splitArea->getSplitArea())
    {
        giSplitArea=splitArea->getValue();
        return true;
    }
    else
    {
        return false;
    }

}

static bool Get_GI_SplitMatching_1_PaimonArea(const cv::Mat &giSplitArea_Paimon,cv::Mat &giArea_Paimon)
{
    static SplitMatching_Paimon* splitMatching_paimon = new SplitMatching_Paimon(logger);
    splitMatching_paimon->setValue(giSplitArea_Paimon);
    if(splitMatching_paimon->getSplitMatching())
    {
        giArea_Paimon=splitMatching_paimon->getValue();
        return true;
    }
    else
    {
        return false;
    }
}
static bool Get_GI_SplitMatching_2_MiniMapArea(const cv::Mat &giSplitArea_MiniMap,cv::Mat &giArea_MiniMap)
{
    static SplitMatching_MiniMap* splitMatching_miniMap = new SplitMatching_MiniMap(logger);
    splitMatching_miniMap->setValue(giSplitArea_MiniMap);
    if(splitMatching_miniMap->getSplitMatching())
    {
        giArea_MiniMap=splitMatching_miniMap->getValue();
        return true;
    }
    else
    {
        return false;
    }
}
static bool Get_GI_SplitMatching_3_UIDArea(const cv::Mat &giSplitArea_UID,cv::Mat &giArea_UID)
{
    static SplitMatching_UID* splitMatchingUID = new SplitMatching_UID(logger);
    splitMatchingUID->setValue(giSplitArea_UID);
    if(splitMatchingUID->getSplitMatching())
    {
        giArea_UID=splitMatchingUID->getValue();
        return true;
    }
    else
    {
        return false;
    }
}
static bool Get_GI_SplitMatching_4_LeftGetItemsArea(const cv::Mat &giSplitArea_LeftGetItems,cv::Mat &giArea_LeftGetItems)
{
    static SplitMatching_LeftGetItems* splitMatching_leftGetItems = new SplitMatching_LeftGetItems(logger);
    splitMatching_leftGetItems->setValue(giSplitArea_LeftGetItems);
    if(splitMatching_leftGetItems->getSplitMatching())
    {
        giArea_LeftGetItems=splitMatching_leftGetItems->getValue();
        return true;
    }
    else
    {
        return false;
    }
}
static bool Get_GI_SplitMatching_5_RightGetItemsArea(const cv::Mat &giSplitArea_RightGetItems, cv::Mat &giArea_RightGetItems)
{
    static SplitMatching_RightGetItems* splitMatching_rightGetItems = new SplitMatching_RightGetItems(logger);
    splitMatching_rightGetItems->setValue(giSplitArea_RightGetItems);
    if(splitMatching_rightGetItems->getSplitMatching())
    {
        giArea_RightGetItems=splitMatching_rightGetItems->getValue();
        return true;
    }
    else
    {
        return false;
    }
}

static bool Get_GI_Matching_1_Paimon(const cv::Mat &giArea_Paimon,bool &giMatchOutput_Paimon)
{
    static Matching_Paimon* matching_paimon = new Matching_Paimon(logger);
    matching_paimon->setValue(giArea_Paimon);
    if(matching_paimon->getMatchOutput())
    {
        giMatchOutput_Paimon=matching_paimon->getValue();
        return true;
    }
    else
    {
        return false;
    }
}

static bool Get_GI_Matching_2_MiniMap(const cv::Mat &giArea_MiniMap,cv::Point2d &giMatchOutput_MiniMap)
{
    //TODO: 02.26-03:00 实现具体的类
    static Matching_MiniMap* matching_miniMap = new Matching_MiniMap(logger);
    matching_miniMap->setValue(giArea_MiniMap);
    if(matching_miniMap->getMatchOutput())
    {
        giMatchOutput_MiniMap=matching_miniMap->getValue();
        return true;
    }
    else
    {
        return false;
    }
}

static void uninstall()
{
    // delete logger;
    // delete screenshot;
    // delete splitArea;
    // delete splitMatching_paimon;
    // delete splitMatching_miniMap;
    // delete splitMatching_leftGetItems;
    // delete splitMatching_rightGetItems;
    // delete matching_paimon;
    // delete matching_miniMap;
}
#endif //CVAUTOTRACKDLL_BAILAN_UTIL_H
