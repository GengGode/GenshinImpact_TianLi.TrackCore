//
// Created by GengG on 2022/2/9.
//

#ifndef CVAUTOTRACKDLL_SURF_MAP_H
#define CVAUTOTRACKDLL_SURF_MAP_H
#include "../Logger/LoggingFacility.h"

#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/core/core.hpp>
#include "Kalmanfilter.h"

class SURF_Map
{
public:
    SURF_Map()=delete;
    SURF_Map(const Logger& logService);
    ~SURF_Map();
    void init();
    // void set_kalman_param(float dt, float Accel_noise_mag);
    // void set_match_param(float ratio, int min_match);
    // void set_detect_param(int min_hessian);
private:
    
    bool is_init_end = false;
    
    //LoadGiMatchResource giMatchResource;
    
    cv::Mat giFrame;
    cv::Mat giPaimonRef;
    cv::Mat giMiniMapRef;
    
    bool isOnCity = false;
    bool isContinuity = false;
    bool isConveying = false;
    cv::Point2d _TransformHistory[3];
    
    int minHessian = 400;
    double ratio_thresh = 0.66;
    double mapScale = 1.3;//1.3;
    int someSizeR = 106;
    //double MatchMatScale = 2.0;
    
    cv::Ptr<cv::xfeatures2d::SURF> _detectorAllMap;
    cv::Ptr<cv::xfeatures2d::SURF> _detectorSomeMap;
    std::vector<cv::KeyPoint> _KeyPointAllMap;
    std::vector<cv::KeyPoint> _KeyPointSomeMap;
    std::vector<cv::KeyPoint> _KeyPointMiniMap;
    cv::Mat _DataPointAllMap;
    cv::Mat _DataPointSomeMap;
    cv::Mat _DataPointMiniMap;
    
    Kalmanfilter posFilter;
    //用户定义映射关系参数
    double UserWorldOrigin_X = 0;
    double UserWorldOrigin_Y = 0;
    double UserWorldScale = 1.0;
    
    // 绝对世界中心 绝对世界缩放系数
    //World Center on AbsAllMap Coor
    double WorldCenter_X = 5352; //Abs
    double WorldCenter_Y = 9432; //Abs
    double WorldScale = 1.0; //Abs
    
    
    //相对绝对空间缩放系数
    //Map and AbsAllMap Scale Value, Map * MapAbsScale = AbsAllMap.
    double MapAbsScale = 2.557; //from diff Image 67.40%
    
    //忘了是啥了，记得明天推导
    //09.07 是个固定值，不用变
    cv::Point2d MapWorldOffset = cv::Point2d(3412, 2025); // ? forget
    //double MapWorldOffset_X = 3412;
    //double MapWorldOffset_Y = 2025;
    
    //也忘了
    double MapWorldScale = 1.0;
    
    // ???
    double screen_scale = 1;
};


#endif //CVAUTOTRACKDLL_SURF_MAP_H
