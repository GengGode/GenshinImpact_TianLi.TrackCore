//
// Created by GengG on 2022/2/8.
//

#ifndef CVAUTOTRACKDLL_BAILAN_UTIL_SPLITMATCHINGII_H
#define CVAUTOTRACKDLL_BAILAN_UTIL_SPLITMATCHINGII_H

#include "../Logger/LoggingFacility.h"

#include "Kalmanfilter.h"

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

class II_1_Position
{
    Logger log;
    int err=0;
    std::mutex lm_output;
    std::mutex lm_input;
    cv::Mat _mat;
    cv::Point2d _out_value;
public:
    II_1_Position()=delete;
    II_1_Position(const Logger& logService);
public:
    void setValue(const cv::Mat &mat);
    
    cv::Point2d getValue();
    
    bool getMatchOutput();

private:
    struct Matching_II_Position_Resource
    {
        cv::Mat MapTemplate;
    };
private:
    void init()
    {
        _out_value.x=0;
        _out_value.y=0;
    }
private:
    
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
    
    double MapWorldScale = 1.0;
    double screen_scale = 1;
private:
    double dis(cv::Point2d p)
    {
        return sqrt(p.x*p.x + p.y*p.y);
    }
    cv::Point2d SPC(std::vector<double> lisx, double sumx, std::vector<double> lisy, double sumy)
    {
        cv::Point2d pos;
        double meanx = sumx / lisx.size(); //均值
        double meany = sumy / lisy.size(); //均值
        double x = meanx;
        double y = meany;
        if (lisx.size() > 3 && lisy.size() > 3)
        {
            double accumx = 0.0;
            double accumy = 0.0;
            for (int i = 0; i < (lisx.size()>lisy.size()? lisy.size(): lisx.size()); i++)
            {
                accumx += (lisx[i] - meanx)*(lisx[i] - meanx);
                accumy += (lisy[i] - meany)*(lisy[i] - meany);
            }
            
            double stdevx = sqrt(accumx / (lisx.size() - 1)); //标准差
            double stdevy = sqrt(accumy / (lisy.size() - 1)); //标准差
            
            sumx = 0;
            sumy = 0;
            double numx = 0;
            double numy = 0;
            for (int i = 0; i < (lisx.size() > lisy.size() ? lisy.size() : lisx.size()); i++)
            {
                if (abs(lisx[i] - meanx) < 1 * stdevx)
                {
                    sumx += lisx[i];
                    numx++;
                }
                
                if (abs(lisy[i] - meany) < 1 * stdevy)
                {
                    sumy += lisy[i];
                    numy++;
                }
            }
            x = sumx / numx;
            y = sumy / numy;
            pos = cv::Point2d(x, y);
        }
        else
        {
            pos = cv::Point2d(x, y);
        }
        return pos;
    }
    
    
    cv::Point2d TransferTianLiAxes(cv::Point2d pos, cv::Point2d origin, double scale)
    {
        return cv::Point2d((pos + origin)*scale);
    }
    
    cv::Point2d TransferUserAxes(cv::Point2d pos, double x, double y, double scale)
    {
        return cv::Point2d((pos.x + x)*scale, (pos.y + y)*scale);
    }
};

class II_2_Direction
{
    Logger log;
    int err=0;
    std::mutex lm_output;
    std::mutex lm_input;
    cv::Mat _mat;
    double _out_value;
public:
    II_2_Direction()=delete;
    II_2_Direction(const Logger& logService);
public:
    void setValue(const cv::Mat &mat);
    
    double getValue();
    
    bool getMatchOutput();
private:
    double dis(cv::Point2d p)
    {
        return sqrt(p.x*p.x + p.y*p.y);
    }
    std::vector<cv::Point2f> Vector2UnitVector(std::vector<cv::Point2f> pLis)
    {
        double length = 1;
        std::vector<cv::Point2f> res;
        for (int i = 0; i < pLis.size(); i++)
        {
            length = sqrt(pLis[i].x*pLis[i].x + pLis[i].y*pLis[i].y);
            res.push_back(cv::Point2f((float)(pLis[i].x / length), (float)(pLis[i].y / length)));
        }
        return res;
    }
    double Line2Angle(cv::Point2f p)
    {
        const double rad2degScale = 180 / CV_PI;
        double res = atan2(-p.y, p.x)*rad2degScale;
        res = res - 90; //从屏幕空间左侧水平线为0度转到竖直向上为0度
        if (res < -180.0)res = res + 360;
        return res;
    }
};

class II_3_Rotation
{
    Logger log;
    int err=0;
    std::mutex lm_output;
    std::mutex lm_input;
    cv::Mat _mat;
    double _out_value;
public:
    II_3_Rotation()=delete;
    II_3_Rotation(const Logger& logService);
public:
    void setValue(const cv::Mat &mat);
    
    double getValue();
    
    bool getMatchOutput();
private:
    double Line2Angle(cv::Point2f p)
    {
        const double rad2degScale = 180 / CV_PI;
        double res = atan2(-p.y, p.x)*rad2degScale;
        res = res - 90; //从屏幕空间左侧水平线为0度转到竖直向上为0度
        if (res < -180.0)res = res + 360;
        return res;
    }
    double min(double a, double b)
    {
        return a < b ? a : b;
    }
};

class II_4_StarJson
{
    Logger log;
    int err=0;
    std::mutex lm_output;
    std::mutex lm_input;
    cv::Mat _mat;
    std::string _out_value;
public:
    II_4_StarJson()=delete;
    II_4_StarJson(const Logger& logService);
public:
    void setValue(const cv::Mat &mat);
    
    std::string getValue();
    
    bool getMatchOutput();
private:

};


#endif //CVAUTOTRACKDLL_BAILAN_UTIL_SPLITMATCHINGII_H
