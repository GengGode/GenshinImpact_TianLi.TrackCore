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
#include "BaiLan_Util_SplitMatchingII.h"
// 日志相关
#include "../Logger/LoggingFacility.h"
#include "../Logger/StandardOutputLogger.h"

// opencv依赖
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool Get_GI_Handle(const Logger& logger, HWND& giHandle)
{
    static HookHandle hookHandle(logger);
    if(hookHandle.getGenshinImpactHandle())
    {
        hookHandle.getValue(giHandle);
        return true;
    }
    else
    {
        return false;
    }
}

bool  Get_GI_Frame(const Logger& logger, const HWND &giHandle,cv::Mat& giFrame)
{
    static Screenshot screenshot(logger);
    screenshot.setValue(giHandle);
    //screenshot.work();
    if(screenshot.screenshot())
    {
        giFrame=screenshot.getValue();
        return true;
    }
    else
    {
        return false;
    }
}

bool  Get_GI_SplitArea(const Logger& logger, const cv::Mat &giFrame,std::vector<cv::Mat> &giSplitArea)
{
    static SplitArea splitArea(logger);
    splitArea.setValue(giFrame);;
    if(splitArea.getSplitArea())
    {
        giSplitArea=splitArea.getValue();
        return true;
    }
    else
    {
        return false;
    }

}

bool  Get_GI_SplitMatching_1_PaimonArea(const Logger& logger, const cv::Mat &giSplitArea_Paimon,cv::Mat &giArea_Paimon)
{
    static SplitMatching_Paimon splitMatching_paimon (logger);
    splitMatching_paimon.setValue(giSplitArea_Paimon);
    if(splitMatching_paimon.getSplitMatching())
    {
        giArea_Paimon=splitMatching_paimon.getValue();
        return true;
    }
    else
    {
        return false;
    }
}
bool  Get_GI_SplitMatching_2_MiniMapArea(const Logger& logger, const cv::Mat &giSplitArea_MiniMap,cv::Mat &giArea_MiniMap)
{
    static SplitMatching_MiniMap splitMatching_miniMap (logger);
    splitMatching_miniMap.setValue(giSplitArea_MiniMap);
    if(splitMatching_miniMap.getSplitMatching())
    {
        giArea_MiniMap=splitMatching_miniMap.getValue();
        return true;
    }
    else
    {
        return false;
    }
}
bool  Get_GI_SplitMatching_3_UIDArea(const Logger& logger, const cv::Mat &giSplitArea_UID,cv::Mat &giArea_UID)
{
    static SplitMatching_UID splitMatchingUID (logger);
    splitMatchingUID.setValue(giSplitArea_UID);
    if(splitMatchingUID.getSplitMatching())
    {
        giArea_UID=splitMatchingUID.getValue();
        return true;
    }
    else
    {
        return false;
    }
}
bool  Get_GI_SplitMatching_4_LeftGetItemsArea(const Logger& logger, const cv::Mat &giSplitArea_LeftGetItems,cv::Mat &giArea_LeftGetItems)
{
    static SplitMatching_LeftGetItems splitMatching_leftGetItems (logger);
    splitMatching_leftGetItems.setValue(giSplitArea_LeftGetItems);
    if(splitMatching_leftGetItems.getSplitMatching())
    {
        giArea_LeftGetItems=splitMatching_leftGetItems.getValue();
        return true;
    }
    else
    {
        return false;
    }
}
bool  Get_GI_SplitMatching_5_RightGetItemsArea(const Logger& logger, const cv::Mat &giSplitArea_RightGetItems, cv::Mat &giArea_RightGetItems)
{
    static SplitMatching_RightGetItems splitMatching_rightGetItems (logger);
    splitMatching_rightGetItems.setValue(giSplitArea_RightGetItems);
    if(splitMatching_rightGetItems.getSplitMatching())
    {
        giArea_RightGetItems=splitMatching_rightGetItems.getValue();
        return true;
    }
    else
    {
        return false;
    }
}

bool  Get_GI_Matching_1_Paimon(const Logger& logger, const cv::Mat &giArea_Paimon,bool &giMatchOutput_Paimon)
{
    static Matching_Paimon matching_paimon (logger);
    matching_paimon.setValue(giArea_Paimon);
    if(matching_paimon.getMatchOutput())
    {
        giMatchOutput_Paimon=matching_paimon.getValue();
        return true;
    }
    else
    {
        return false;
    }
}
bool  Get_GI_Matching_2_MiniMap(const Logger& logger, const cv::Mat &giArea_MiniMap,cv::Point2d &giMatchOutput_MiniMap)
{
    //TODO: 02.26-03:00 实现具体的类
    static Matching_MiniMap matching_miniMap (logger);
    matching_miniMap.setValue(giArea_MiniMap);
    if(matching_miniMap.getMatchOutput())
    {
        giMatchOutput_MiniMap=matching_miniMap.getValue();
        return true;
    }
    else
    {
        return false;
    }
}
bool  Get_GI_Matching_3_UID(const Logger& logger, const cv::Mat &giArea_UID,int &giMatchOutput_UID)
{
    static Matching_UID matching_UID (logger);
    matching_UID.setValue(giArea_UID);
    if(matching_UID.getMatchOutput())
    {
        giMatchOutput_UID=matching_UID.getValue();
        return true;
    }
    else
    {
        return false;
    }
}
bool  Get_GI_Matching_4_LeftGetItems(const Logger& logger, const cv::Mat &giArea_LeftGetItems,std::string &giMatchOutput_LeftGetItems)
{
    static Matching_LeftGetItems matching_leftGetItems (logger);
    matching_leftGetItems.setValue(giArea_LeftGetItems);
    if(matching_leftGetItems.getMatchOutput())
    {
        giMatchOutput_LeftGetItems=matching_leftGetItems.getValue();
        return true;
    }
    else
    {
        return false;
    }
}
bool  Get_GI_Matching_5_RightGetItems(const Logger& logger, const cv::Mat &giArea_RightGetItems,std::string &giMatchOutput_RightGetItems)
{
    static Matching_RightGetItems matching_rightGetItems (logger);
    matching_rightGetItems.setValue(giArea_RightGetItems);
    if(matching_rightGetItems.getMatchOutput())
    {
        giMatchOutput_RightGetItems=matching_rightGetItems.getValue();
        return true;
    }
    else
    {
        return false;
    }
}

bool  Get_GI_II_1_Position(const Logger& logger, const cv::Mat &giArea_MiniMap,cv::Point2d &giII_1_Position)
{
    static II_1_Position ii_1_position (logger);
    ii_1_position.setValue(giArea_MiniMap);
    if(ii_1_position.getMatchOutput())
    {
        giII_1_Position=ii_1_position.getValue();
        return true;
    }
    else
    {
        return false;
    }
}

bool  Get_GI_II_2_Direction(const Logger& logger, const cv::Mat &giArea_MiniMap,double &giII_2_Direction)
{
    static II_2_Direction ii_2_direction (logger);
    ii_2_direction.setValue(giArea_MiniMap);
    if(ii_2_direction.getMatchOutput())
    {
        giII_2_Direction=ii_2_direction.getValue();
        return true;
    }
    else
    {
        return false;
    }
}

bool Get_GI_II_3_Rotation(const Logger& logger, const cv::Mat &giArea_MiniMap,double &giII_3_Rotation)
{
    static II_3_Rotation ii_3_rotation (logger);
    ii_3_rotation.setValue(giArea_MiniMap);
    if(ii_3_rotation.getMatchOutput())
    {
        giII_3_Rotation=ii_3_rotation.getValue();
        return true;
    }
    else
    {
        return false;
    }
}

bool Get_GI_II_4_StarJson(const Logger& logger, const cv::Mat &giArea_MiniMap,std::string &giII_4_StarJson)
{
    static II_4_StarJson ii_4_starJson(logger);
    ii_4_starJson.setValue(giArea_MiniMap);
    if(ii_4_starJson.getMatchOutput())
    {
        giII_4_StarJson=ii_4_starJson.getValue();
        return true;
    }
    else
    {
        return false;
    }
}
#endif //CVAUTOTRACKDLL_BAILAN_UTIL_H
