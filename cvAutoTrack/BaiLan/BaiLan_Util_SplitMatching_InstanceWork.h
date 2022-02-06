//
// Created by GengG on 2022/1/30.
//

#ifndef CVAUTOTRACKDLL_BAILAN_UTIL_SPLITMATCHING_INSTANCEWORK_H
#define CVAUTOTRACKDLL_BAILAN_UTIL_SPLITMATCHING_INSTANCEWORK_H
#include <opencv2/opencv.hpp>

bool            getSplitMatching_Paimon(cv::Mat& mat,cv::Mat& _out_mat) {
    if(mat.empty())
    {
        return false;
    }
    // TODO: 暂时只根据比例计算
    
    /* 测试画面
    static bool isSave = false;
    if(!isSave)
    {
        cv::Mat mat_temp=_out_mat;
        cv::imwrite("C:\\Users\\GengG\\CLionProjects\\cvAutoTrack\\cvAutoTrack\\Resource\\output\\mat_temp.png",mat_temp);
        isSave = true;
    }
    */
    
    /*
			PaimonRect = Rect(cvCeil(giSize.width*0.0135), cvCeil(giSize.width*0.006075), cvCeil(giSize.width*0.035), cvCeil(giSize.width*0.0406));
			resIdPaimon = 0;
			// 适配带鱼屏
			if (giSize.width / giSize.height == 64 / 27)
			{
				PaimonRect = Rect(cvCeil(giSize.width*0.038), cvCeil(giSize.height*0.012), cvCeil(giSize.height / 9.0 * 16.0 *0.035), cvCeil(giSize.height / 9.0 * 16.0 *0.0406));
			}
			break;
     */
    // 计算比例 			PaimonRect = Rect(26, 12, 68, 77);
    int split_x =26;
    int split_y =12;
    int split_width = 68;
    int split_height = 77;
    cv::Rect ROI(split_x,split_y,split_width,split_height);
    
    _out_mat=mat(ROI);

    return true;
}
bool           getSplitMatching_MiniMap(cv::Mat& mat,cv::Mat& _out_mat) {
    if(mat.empty())
    {
        return false;
    }
    // TODO: 暂时只根据比例计算
    
    /* 测试画面
    static bool isSave = false;
    
    if(!isSave)
    {
        cv::Mat mat_temp=mat;
        cv::imwrite("C:\\Users\\GengG\\CLionProjects\\cvAutoTrack\\cvAutoTrack\\Resource\\output\\mat_temp_2.png",mat_temp);
        isSave = true;
    }
     */
    
    /*
			mapRect = Rect(cvCeil(giSize.width*0.032), cvCeil(giSize.width*0.01), cvCeil(giSize.width *0.11), cvCeil(giSize.width*0.11));
			if (giSize.width / giSize.height == 64 / 27)
			{
				mapRect = Rect(cvCeil(giSize.width*0.051), cvCeil(giSize.height / 9.0 * 16.0 *0.01), cvCeil(giSize.height/9.0*16.0 *0.11), cvCeil(giSize.height / 9.0 * 16.0 *0.11));
			}
			break;
     */
    
    // 计算比例 mapRect = Rect(62, 19, 212, 212);
    int split_x = 62;
    int split_y = 19;
    int split_width = 212;
    int split_height = 212;
    cv::Rect ROI(split_x,split_y,split_width,split_height);
    
    _out_mat=mat(ROI);

    return true;
}
bool               getSplitMatching_UID(cv::Mat& mat,cv::Mat& _out_mat) {
    if(mat.empty())
    {
        return false;
    }
    // TODO: 暂时不做处理原样输出
    _out_mat=mat;
    return true;
}
bool      getSplitMatching_LeftGetItems(cv::Mat& mat,cv::Mat& _out_mat) {
    if(mat.empty())
    {
        return false;
    }
    // TODO: 暂时不做处理原样输出
    _out_mat=mat;
    return true;
}
bool     getSplitMatching_RightGetItems(cv::Mat& mat,cv::Mat& _out_mat) {
    if(mat.empty())
    {
        return false;
    }
    // TODO: 暂时不做处理原样输出
    _out_mat=mat;
    return true;
}

#endif //CVAUTOTRACKDLL_BAILAN_UTIL_SPLITMATCHING_INSTANCEWORK_H
