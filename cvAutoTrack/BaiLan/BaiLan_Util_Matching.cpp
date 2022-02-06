//
// Created by GengG on 2022/2/6.
//

#include "BaiLan_Util_Matching.h"

#include "BaiLan_Util_Resource.h"


Matching_Paimon::Matching_Paimon(const Logger &logService) {
    log=logService;
}

void Matching_Paimon::setValue(const cv::Mat &mat) {
    if(_mat.empty())
    {
        _mat=mat;
    }
    else
    {
        //TODO: 判断传入的图片是否变化，可能会有一些问题
        if(mat.data==_mat.data)
        {
            return;
        }
        else
        {
            std::lock_guard<std::mutex> lock(lm_input);
            //TODO: 这里的拷贝可能会出现问题，需要观察
            _mat=mat;
        }
    }
}

bool Matching_Paimon::getValue() {
    std::lock_guard<std::mutex> lock(lm_output);
    bool ret=_out_value;
    return std::move(ret);
}

bool Matching_Paimon::getMatchOutput() {
    //return getMatching_Paimon(_mat,_out_value);
    
    //TODO: 临时的加载资源方案
    static cv::Mat paimonTemplate=BaiLanResource.PaimonTemplate.clone();
    //TODO: 如何匹配的细节
    
    std::vector<cv::Mat> lisT, lisR;
    cv::split(paimonTemplate, lisT);
    cv::split(_mat, lisR);
    
    cv::Mat tmp;
    cv::Mat Template, Ref;
    cv::cvtColor(paimonTemplate, Template, cv::COLOR_RGBA2GRAY);
    cv::cvtColor(_mat, Ref, cv::COLOR_RGBA2GRAY);
    
    //cv::matchTemplate(paimonTemplate, giPaimonRef, tmp, cv::TM_CCOEFF_NORMED);
    //cv::matchTemplate(Template, Ref, tmp, cv::TM_CCOEFF_NORMED);
    cv::matchTemplate(lisT[3], lisR[3], tmp, cv::TM_CCOEFF_NORMED);
    
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(tmp, &minVal, &maxVal, &minLoc, &maxLoc);

    // #ifdef _DEBUG
    //     //显示Template
    //     cv::namedWindow("Template", cv::WINDOW_FREERATIO);
    //     cv::imshow("Template", lisT[3]);
    //
    //     //显示Ref
    //     cv::namedWindow("Ref", cv::WINDOW_FREERATIO);
    //     cv::imshow("Ref", lisR[3]);
    //
    //     //显示tmp
    //     cv::namedWindow("test2", cv::WINDOW_FREERATIO);
    //     cv::imshow("test2", tmp);
    //     cv::waitKey(1);
    //
    //     //logger.log("Paimon Match: " + minVal + "," + maxVal);
    // #endif
    
    //if (maxVal < 0.36 || maxVal == 1)
    if (maxVal < 0.51 || maxVal == 1)
    {
        _out_value=false;
        //err = 6;//未能匹配到派蒙
        
    }
    else
    {
        //TODO: 需要额外判断收取树脂时的灰色遮挡状态
        //如果ListR[3]的中心像素的颜色小于250，则判断为遮挡
        if(lisR[3].at<uchar>(static_cast<int>(lisR[3].rows/2), static_cast<int>(lisR[3].cols/2))<250)
        {
            //log->Log("Paimon Match: " + std::to_string(lisR[3].at<uchar>(static_cast<int>(lisR[3].rows/2), static_cast<int>(lisR[3].cols/2))));
            _out_value= false;
        }
        else
        {
            _out_value= true;
        }
    }
    
    return _out_value;
}

Matching_MiniMap::Matching_MiniMap(const Logger &logService) {
    log=logService;
}


void Matching_MiniMap::setValue(const cv::Mat &mat) {
    if(_mat.empty())
    {
        _mat=mat;
    }
    else
    {
        //TODO: 判断传入的图片是否变化，可能会有一些问题
        if(mat.data==_mat.data)
        {
            return;
        }
        else
        {
            std::lock_guard<std::mutex> lock(lm_input);
            //TODO: 这里的拷贝可能会出现问题，需要观察
            _mat=mat;
        }
    }
}

cv::Point2d Matching_MiniMap::getValue() {
    std::lock_guard<std::mutex> lock(lm_output);
    cv::Point2d ret=_out_value;
    return std::move(ret);
}

bool Matching_MiniMap::getMatchOutput() {
    //return getMatching_Paimon(_mat,_out_value);
    
    //TODO: 临时的加载资源方案
    static cv::Mat mapTemplate=BaiLanResource.MapTemplate.clone();
    //灰度化地图参考
    static bool isGrayMap= false;
    if(!isGrayMap)
    {
        cv::cvtColor(mapTemplate, mapTemplate, cv::COLOR_RGBA2RGB);
        isGrayMap=true;
    }
    //TODO: 如何匹配的细节
    //
    

    
    cv::Mat giMiniMapRef =_mat;
    
    cv::Mat img_scene(mapTemplate);
    cv::Mat img_object(giMiniMapRef(cv::Rect(30, 30, giMiniMapRef.cols - 60, giMiniMapRef.rows - 60)));
    
    return true;
}