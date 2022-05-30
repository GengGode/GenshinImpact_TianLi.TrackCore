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
    auto ret=_out_value;
    return std::move(ret);
}

bool Matching_Paimon::getMatchOutput() {
    //return getMatching_Paimon(_mat,_out_value);
    
    //TODO: 临时的加载资源方案
    static cv::Mat paimonTemplate=BaiLanResource.PaimonTemplate.clone();
    //TODO: 如何匹配的细节
    
    // 判断入参为空的情况
    if(_mat.empty())
    {
        log->warning("Matching_Paimon::getMatchOutput() _mat is empty");
        return false;
    }
    
    
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
         //显示Template
         cv::namedWindow("Template", cv::WINDOW_FREERATIO);
         cv::imshow("Template", lisT[3]);
    
         //显示Ref
         cv::namedWindow("Ref", cv::WINDOW_FREERATIO);
         cv::imshow("Ref", lisR[3]);
    
         //显示tmp
         cv::namedWindow("test2", cv::WINDOW_FREERATIO);
         cv::imshow("test2", tmp);
         cv::waitKey(1);
    
         //logger.log("Paimon Match: " + minVal + "," + maxVal);
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
    auto ret=_out_value;
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

Matching_UID::Matching_UID(const Logger &logService) {
    log=logService;
}

void Matching_UID::setValue(const cv::Mat &mat) {
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

int Matching_UID::getValue() {
    std::lock_guard<std::mutex> lock(lm_output);
    auto ret=_out_value;
    return std::move(ret);
}

bool Matching_UID::getMatchOutput() {
    //return getMatching_Paimon(_mat,_out_value);
    
    //TODO: 临时的加载资源方案
    static Matching_UID_Resource giMatchResource;
    static cv::Mat UIDTemplate=BaiLanResource.UID.clone();
    static std::vector<cv::Mat> UIDnumberTemplate;//=BaiLanResource.UIDnumber.clone();
    //灰度化地图参考
    static bool isGrayMap= false;
    if(!isGrayMap)
    {
        //cv::cvtColor(mapTemplate, mapTemplate, cv::COLOR_RGBA2RGB);
        isGrayMap=true;
    }
    //加载数组类型资源
    static bool isLoadNumber= false;
    if(!isLoadNumber)
    {
        for (int i = 0;  i < 10; i++)
        {
            UIDnumberTemplate.push_back(BaiLanResource.UIDnumber[i].clone());
        }
        giMatchResource.UID=UIDTemplate;
        giMatchResource.UIDnumber=UIDnumberTemplate;
    }
    //TODO: 如何匹配的细节
    
    // 判断入参为空的情况
    if (_mat.empty())
    {
        log->warning("Matching_UID::getMatchOutput() _mat is empty");
        return false;
    }
    
    // 入参赋予有意义引用变量名
    cv::Mat giUIDRef=_mat;


    std::vector<cv::Mat> channels;
    
    split(giUIDRef, channels);
    giUIDRef = channels[0];
    
    int _uid = 0;
    int _NumBit[9] = { 0 };
    
    int bitCount = 1;
    cv::Mat matchTmp;
    cv::Mat Roi;
    cv::Mat checkUID = giMatchResource.UID;

#ifdef _DEBUG
    //if (checkUID.rows > Roi.rows)
    //{
    //	cv::resize(checkUID, checkUID, cv::Size(), Roi.rows/ checkUID.rows);
    //}
#endif
    giUIDRef.copyTo(Roi);
    
    if (checkUID.rows > Roi.rows)
    {
        cv::resize(Roi, Roi, cv::Size(cvRound(1.0 * checkUID.rows / Roi.rows * Roi.cols), checkUID.rows), 0);
    }
    
    cv::matchTemplate(Roi, checkUID, matchTmp, cv::TM_CCOEFF_NORMED);
    
    double minVal = 0;
    double maxVal = 0;
    cv::Point minLoc = cv::Point(0, 0);
    cv::Point maxLoc = cv::Point(0, 0);
    //寻找最佳匹配位置
    cv::minMaxLoc(matchTmp, &minVal, &maxVal, &minLoc, &maxLoc);
    if (maxVal > 0.75)
    {
        int x = maxLoc.x + checkUID.cols + 7;
        int y = 0;
        double tmplis[10] = { 0 };
        int tmplisx[10] = { 0 };
        for (int p = 8; p >= 0; p--)
        {
            _NumBit[p] = 0;
            for (int i = 0; i < 10; i++)
            {
                cv::Rect r(x, y, giMatchResource.UIDnumber[i].cols + 2, giUIDRef.rows);//180-46/9->140/9->16->16*9=90+54=144
                if (x + r.width > giUIDRef.cols)
                {
                    r = cv::Rect(giUIDRef.cols - giMatchResource.UIDnumber[i].cols - 2, y, giMatchResource.UIDnumber[i].cols + 2, giUIDRef.rows);
                }
                
                cv::Mat numCheckUID = giMatchResource.UIDnumber[i];
                Roi = giUIDRef(r);
                
                cv::matchTemplate(Roi, numCheckUID, matchTmp, cv::TM_CCOEFF_NORMED);
                
                double minVali, maxVali;
                cv::Point minLoci, maxLoci;
                //寻找最佳匹配位置
                cv::minMaxLoc(matchTmp, &minVali, &maxVali, &minLoci, &maxLoci);
                
                tmplis[i] = maxVali;
                tmplisx[i] = maxLoci.x + numCheckUID.cols - 1;
                if (maxVali > 0.91)
                {
                    _NumBit[p] = i;
                    x = x + maxLoci.x + numCheckUID.cols - 1;
                    break;
                }
                if (i == 10 - 1)
                {
                    _NumBit[p] = getMaxID(tmplis, 10);
                    x = x + tmplisx[_NumBit[p]];
                }
            }
        }
    }
    
    _uid = 0;
    for (int i = 0; i < 9; i++)
    {
        _uid += _NumBit[i] * bitCount;
        bitCount = bitCount * 10;
    }
    
    
    if (_uid == 0)
    {
        err = 207;//未能在UID区域检测到有效UID
        return false;
    }
    _out_value = _uid;
    return true;
}

Matching_LeftGetItems::Matching_LeftGetItems(const Logger &logService) {
    log = logService;
}

void Matching_LeftGetItems::setValue(const cv::Mat &mat) {
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

std::string Matching_LeftGetItems::getValue() {
    std::lock_guard<std::mutex> lock(lm_output);
    auto ret=_out_value;
    return std::move(ret);
}

bool Matching_LeftGetItems::getMatchOutput() {
    
    //TODO: 临时的加载资源方案
    //static cv::Mat UIDTemplate=BaiLanResource.UID.clone();
    //灰度化地图参考
    //static bool isGrayMap= false;
    //if(!isGrayMap)
    //{
    //    //cv::cvtColor(mapTemplate, mapTemplate, cv::COLOR_RGBA2RGB);
    //    isGrayMap=true;
    //}
    ////加载数组类型资源
    //static bool isLoadNumber= false;
    //if(!isLoadNumber)
    //{
    //}
    //TODO: 如何匹配的细节
    
    // 判断入参为空的情况
    if (_mat.empty())
    {
        log->warning("Matching_LeftGetItems::getMatchOutput() _mat is empty");
        return false;
    }
    
    // 入参赋予有意义引用变量名
    //cv::Mat giUIDRef=_mat;
    
    
    return true;
}

Matching_RightGetItems::Matching_RightGetItems(const Logger &logService) {
    log = logService;
}

void Matching_RightGetItems::setValue(const cv::Mat &mat) {
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

std::string Matching_RightGetItems::getValue() {
    std::lock_guard<std::mutex> lock(lm_output);
    auto ret=_out_value;
    return std::move(ret);
}

bool Matching_RightGetItems::getMatchOutput() {
    
    //TODO: 临时的加载资源方案
    //static cv::Mat UIDTemplate=BaiLanResource.UID.clone();
    //灰度化地图参考
    //static bool isGrayMap= false;
    //if(!isGrayMap)
    //{
    //    //cv::cvtColor(mapTemplate, mapTemplate, cv::COLOR_RGBA2RGB);
    //    isGrayMap=true;
    //}
    ////加载数组类型资源
    //static bool isLoadNumber= false;
    //if(!isLoadNumber)
    //{
    //}
    //TODO: 如何匹配的细节
    
    // 判断入参为空的情况
    if (_mat.empty())
    {
        log->warning("Matching_RightGetItems::getMatchOutput() _mat is empty");
        return false;
    }
    
    // 入参赋予有意义引用变量名
    //cv::Mat giUIDRef=_mat;
    
    return true;
}
