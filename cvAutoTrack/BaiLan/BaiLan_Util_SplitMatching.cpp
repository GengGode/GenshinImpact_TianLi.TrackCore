//
// Created by GengG on 2022/1/30.
//

#include "BaiLan_Util_SplitMatching.h"
#include "BaiLan_Util_SplitMatching_InstanceWork.h"

SplitMatching_Paimon::SplitMatching_Paimon(const Logger& logService)
{
    log=logService;
}

void SplitMatching_Paimon::setValue(const cv::Mat &mat) {
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

cv::Mat SplitMatching_Paimon::getValue() {
    std::lock_guard<std::mutex> lock(lm_output);
    cv::Mat ret=_out_mat;
    return std::move(ret);
}

bool SplitMatching_Paimon::getSplitMatching() {
    return getSplitMatching_Paimon(_mat,_out_mat);
}


SplitMatching_MiniMap::SplitMatching_MiniMap(const Logger& logService)
{
    log=logService;
}

void SplitMatching_MiniMap::setValue(const cv::Mat &mat) {
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

cv::Mat SplitMatching_MiniMap::getValue() {
    std::lock_guard<std::mutex> lock(lm_output);
    cv::Mat ret=_out_mat;
    return std::move(ret);
}

bool SplitMatching_MiniMap::getSplitMatching() {
    return getSplitMatching_MiniMap(_mat,_out_mat);
}


SplitMatching_UID::SplitMatching_UID(const Logger& logService)
{
    log=logService;
}

void SplitMatching_UID::setValue(const cv::Mat &mat) {
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

cv::Mat SplitMatching_UID::getValue() {
    std::lock_guard<std::mutex> lock(lm_output);
    cv::Mat ret=_out_mat;
    return std::move(ret);
}

bool SplitMatching_UID::getSplitMatching() {
    return getSplitMatching_UID(_mat,_out_mat);
}

SplitMatching_LeftGetItems::SplitMatching_LeftGetItems(const Logger& logService)
{
    log=logService;
}

void SplitMatching_LeftGetItems::setValue(const cv::Mat &mat) {
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

cv::Mat SplitMatching_LeftGetItems::getValue() {
    std::lock_guard<std::mutex> lock(lm_output);
    cv::Mat ret=_out_mat;
    return std::move(ret);
}

bool SplitMatching_LeftGetItems::getSplitMatching() {
    return getSplitMatching_LeftGetItems(_mat,_out_mat);
}

SplitMatching_RightGetItems::SplitMatching_RightGetItems(const Logger& logService)
{
    log=logService;
}

void SplitMatching_RightGetItems::setValue(const cv::Mat &mat) {
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

cv::Mat SplitMatching_RightGetItems::getValue() {
    std::lock_guard<std::mutex> lock(lm_output);
    cv::Mat ret=_out_mat;
    return std::move(ret);
}

bool SplitMatching_RightGetItems::getSplitMatching() {
    return getSplitMatching_RightGetItems(_mat, _out_mat);
}
