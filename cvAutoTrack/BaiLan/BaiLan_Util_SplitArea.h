//
// Created by GengG on 2022/1/30.
//

#ifndef CVAUTOTRACKDLL_BAILAN_UTIL_SPLITAREA_H
#define CVAUTOTRACKDLL_BAILAN_UTIL_SPLITAREA_H
#include "../Logger/LoggingFacility.h"

#include <vector>
#include <opencv2/opencv.hpp>

// 分块 目标暂且只有五个 派蒙、小地图、UID、左侧已获取物品、右侧可捡取物品
class SplitArea {
    Logger log;
    int err=0;
    std::mutex lm_output;
    std::mutex lm_input;
public:
    cv::Mat _giFrame;
    std::vector<cv::Mat> _giSplitArea;
    SplitArea()=delete;
    SplitArea(const Logger& logService)
    {
        log=logService;
    }
    void setValue(const cv::Mat& Frame)
    {
        if(_giFrame.empty())
        {
            _giFrame=Frame;
        }
        else
        {
            //TODO: 判断传入的图片是否变化，可能会有一些问题
            if(Frame.data==_giFrame.data)
            {
                return;
            }
            else
            {
                std::lock_guard<std::mutex> lock(lm_input);
                //TODO: 这里的拷贝可能会出现问题，需要观察
                _giFrame=Frame;
            }
        }
    }
    // 此处才进行Mat的拷贝
    std::vector<cv::Mat> getValue()
    {
        std::lock_guard<std::mutex> lock(lm_output);
        std::vector<cv::Mat> ret;
        for(auto splitArea: _giSplitArea)
        {
            //TODO: 考虑下这里的拷贝，可能可以直接返回引用
            //TODO: 待测试
            ret.push_back(splitArea.clone());
        }
        return std::move(ret);
    }
    // 此处不对Mat进行拷贝，直接在类内返回Mat的浅拷贝
    bool getSplitArea()
    {
        if(_giFrame.empty())
        {
            err=1;
            return false;
        }
        if(_giFrame.cols<800||_giFrame.rows<600)
        {
            err=2;
            return false;
        }

        std::vector<cv::Rect> rects= CalculationMayArea_Rect(_giFrame.cols, _giFrame.rows);
        cv::Mat giFrame_Paimon_mayArea=_giFrame(rects[0]);
        cv::Mat giFrame_MiniMap_mayArea=_giFrame(rects[1]);
        cv::Mat giFrame_UID_mayArea=_giFrame(rects[2]);
        cv::Mat giFrame_LeftGetItmes_mayArea=_giFrame(rects[3]);
        cv::Mat giFrame_RightGetItmes_mayArea=_giFrame(rects[4]);

        _giSplitArea={
                giFrame_Paimon_mayArea,
                giFrame_MiniMap_mayArea,
                giFrame_UID_mayArea,
                giFrame_LeftGetItmes_mayArea,
                giFrame_RightGetItmes_mayArea
        };
        return true;
    }

private:
    // 计算各个区块目标的可能性区域
    // \x 传入的图片的宽度
    // \y 传入的图片的高度
    std::vector<cv::Rect> CalculationMayArea_Rect(int x,int y)
    {
        static std::vector<cv::Rect> mayArea{5,cv::Rect(0,0,x,y)};

        static int _last_x=0;
        static int _last_y=0;
        if(_last_x==x && _last_y==y)
        {
            return mayArea;
        }
        // 派蒙可能性区域计算参数
        int paimon_mayArea_left=                                        0;
        int paimon_mayArea_top=                                         0;
        int paimon_mayArea_width=              static_cast<int>(x * 0.10);
        int paimon_mayArea_height=             static_cast<int>(y * 0.10);
        // 派蒙可能性区域
        cv::Rect Area_Paimon_mayArea(
                paimon_mayArea_left,
                paimon_mayArea_top,
                paimon_mayArea_width,
                paimon_mayArea_height);
        mayArea[0] = Area_Paimon_mayArea;

        // 小地图可能性区域计算参数
        int miniMap_mayArea_left=                                       0;
        int miniMap_mayArea_top=                                        0;
        int miniMap_mayArea_width=             static_cast<int>(x * 0.18);
        int miniMap_mayArea_height=            static_cast<int>(y * 0.22);
        // 小地图可能性区域
        cv::Rect Area_MiniMap_mayArea(
                miniMap_mayArea_left,
                miniMap_mayArea_top,
                miniMap_mayArea_width,
                miniMap_mayArea_height);
        mayArea[1] = Area_MiniMap_mayArea;

        // UID可能性区域计算参数
        int UID_mayArea_left=                  static_cast<int>(x * 0.88);
        int UID_mayArea_top=                   static_cast<int>(y * 0.97);
        int UID_mayArea_width=                       x - UID_mayArea_left;
        int UID_mayArea_height=                       y - UID_mayArea_top;
        // UID可能性区域
        cv::Rect Area_UID_mayArea(
                UID_mayArea_left,
                UID_mayArea_top,
                UID_mayArea_width,
                UID_mayArea_height);
        mayArea[2] = Area_UID_mayArea;

        // 左侧已获取物品可能性区域计算参数
        int leftGetItems_mayArea_left=                                  0;
        int leftGetItems_mayArea_top=                                   0;
        int leftGetItems_mayArea_width=        static_cast<int>(x * 0.20);
        int leftGetItems_mayArea_height=       static_cast<int>(y * 0.03);
        // 左侧已获取物品可能性区域
        cv::Rect Area_LeftGetItems_mayArea(
                leftGetItems_mayArea_left,
                leftGetItems_mayArea_top,
                leftGetItems_mayArea_width,
                leftGetItems_mayArea_height);
        mayArea[3] = Area_LeftGetItems_mayArea;

        // 右侧可捡取物品可能性区域计算参数
        int rightGetItems_mayArea_left=                                 0;
        int rightGetItems_mayArea_top=                                  0;
        int rightGetItems_mayArea_width=       static_cast<int>(x * 0.20);
        int rightGetItems_mayArea_height=      static_cast<int>(y * 0.22);
        // 右侧可捡取物品可能性区域
        cv::Rect Area_RightGetItems_mayArea(
                rightGetItems_mayArea_left,
                rightGetItems_mayArea_top,
                rightGetItems_mayArea_width,
                rightGetItems_mayArea_height);
        mayArea[4] = Area_RightGetItems_mayArea;


        _last_x=x;
        _last_y=y;
        return mayArea;
    }
};

#endif //CVAUTOTRACKDLL_BAILAN_UTIL_SPLITAREA_H
