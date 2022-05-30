//
// Created by GengG on 2022/2/8.
//

#include "BaiLan_Util_SplitMatchingII.h"

#include "BaiLan_Util_Resource.h"

#include <io.h>

II_1_Position::II_1_Position(const Logger &logService) {
    log= logService;
}

void II_1_Position::setValue(const cv::Mat &mat) {
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

cv::Point2d II_1_Position::getValue() {
    std::lock_guard<std::mutex> lock(lm_output);
    auto ret =_out_value;
    return std::move(ret);
}

bool II_1_Position::getMatchOutput() {
    //计算角色位置
    
    //TODO: 临时的加载资源方案
    static Matching_II_Position_Resource giMatchResource;//mapTemplate=BaiLanResource.MapTemplate.clone();
    //灰度化地图参考
    static bool isGrayMap= false;
    if(!isGrayMap)
    {
        cv::cvtColor(BaiLanResource.MapTemplate, giMatchResource.MapTemplate, cv::COLOR_RGBA2RGB);
        isGrayMap=true;
    }
    static bool isLoadSurfResource=false;
    if(!isLoadSurfResource)
    {
        init();
        _detectorAllMap = cv::xfeatures2d::SURF::create(minHessian);
        //_detectorAllMap->detectAndCompute(giMatchResource.MapTemplate, cv::noArray(), _KeyPointAllMap, _DataPointAllMap);
        
        cv::FileStorage test("", cv::FileStorage::WRITE);
        
        std::string path="C:/Users/GengG/CLionProjects/cvAutoTrack/cmake-build-debug-visual-studio-2019/bin/Debug/resource/surf_allmap.yml";
        if(_access( path.c_str(), 0 ) != -1){
            cv::FileStorage fs(path, cv::FileStorage::READ);
            fs["KeyPoint"] >> _KeyPointAllMap;
            _detectorAllMap->compute(giMatchResource.MapTemplate,_KeyPointAllMap, _DataPointAllMap);
            fs.release();
        
        }else{
            cv::FileStorage fs(path, cv::FileStorage::WRITE);
            //_detectorAllMap->detect(giMatchResource.MapTemplate, _KeyPointAllMap);
            _detectorAllMap->detectAndCompute(giMatchResource.MapTemplate, cv::noArray(), _KeyPointAllMap, _DataPointAllMap);
            fs << "KeyPoint" << _KeyPointAllMap;
            fs.release();
        }
        
        isLoadSurfResource=true;
    }
    //TODO: 如何匹配的细节
    
    // 判断入参为空的情况
    if (_mat.empty())
    {
        log->warning("Matching_RightGetItems::getMatchOutput() _mat is empty");
        return false;
    }
    // 判断入参是否为4通道图
    if (_mat.type() != CV_8UC4)
    {
        log->warning("Matching_RightGetItems::getMatchOutput() _mat is not CV_8UC4");
        return false;
    }
    
    // 入参赋予有意义引用变量名
    cv::Mat giMiniMapRef =_mat;
    
    cv::Mat img_scene(giMatchResource.MapTemplate);
    cv::Mat img_object(giMiniMapRef(cv::Rect(30, 30, giMiniMapRef.cols - 60, giMiniMapRef.rows - 60)));
    

    
    if (img_object.empty())
    {
        //err = 5;//原神小地图区域为空或者区域长宽小于60px
        return false;
    }
    
    isContinuity = false;
    
    cv::Point2d* hisP = _TransformHistory;
    
    cv::Point2d pos;
    
    if (dis(hisP[2] - hisP[1]) < 1000)
    {
        if (hisP[2].x > someSizeR && hisP[2].x < img_scene.cols - someSizeR && hisP[2].y>someSizeR && hisP[2].y < img_scene.rows - someSizeR)
        {
            isContinuity = true;
            if (isContinuity)
            {
                if (isOnCity == false)
                {
                    cv::Mat someMap(img_scene(cv::Rect(cvRound(hisP[2].x - someSizeR), cvRound(hisP[2].y - someSizeR), cvRound(someSizeR * 2), cvRound(someSizeR * 2))));
                    cv::Mat minMap(img_object);
                    
                    //resize(someMap, someMap, Size(), MatchMatScale, MatchMatScale, 1);
                    //resize(minMap, minMap, Size(), MatchMatScale, MatchMatScale, 1);
                    
                    //cv::Ptr<cv::xfeatures2d::SURF>& detectorSomeMap = *(cv::Ptr<cv::xfeatures2d::SURF>*)_detectorSomeMap;
                    //std::vector<cv::KeyPoint>& KeyPointSomeMap = *(std::vector<cv::KeyPoint>*)_KeyPointSomeMap;
                    //cv::Mat& DataPointSomeMap = *(cv::Mat*)_DataPointSomeMap;
                    //std::vector<cv::KeyPoint>& KeyPointMiniMap = *(std::vector<cv::KeyPoint>*)_KeyPointMiniMap;
                    //cv::Mat& DataPointMiniMap = *(cv::Mat*)_DataPointMiniMap;
                    
                    cv::Ptr<cv::xfeatures2d::SURF>& detectorSomeMap = _detectorSomeMap;
                    std::vector<cv::KeyPoint>& KeyPointSomeMap = _KeyPointSomeMap;
                    cv::Mat& DataPointSomeMap = _DataPointSomeMap;
                    std::vector<cv::KeyPoint>& KeyPointMiniMap = _KeyPointMiniMap;
                    cv::Mat& DataPointMiniMap = _DataPointMiniMap;
                    
                    detectorSomeMap = cv::xfeatures2d::SURF::create(minHessian);
                    detectorSomeMap->detectAndCompute(someMap, cv::noArray(), KeyPointSomeMap, DataPointSomeMap);
                    detectorSomeMap->detectAndCompute(minMap, cv::noArray(), KeyPointMiniMap, DataPointMiniMap);
                    
                    std::vector<double> lisx;
                    std::vector<double> lisy;
                    double sumx = 0;
                    double sumy = 0;
                    
                    if (KeyPointMiniMap.size() == 0 || KeyPointSomeMap.size() == 0)
                    {
                        isContinuity = false;
                    }
                    else
                    {
                        cv::Ptr<cv::DescriptorMatcher> matcherTmp = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
                        std::vector< std::vector<cv::DMatch> > KNN_mTmp;
#ifdef _DEBUG
                        std::vector<cv::DMatch> good_matchesTmp;
#endif
                        matcherTmp->knnMatch(DataPointMiniMap, DataPointSomeMap, KNN_mTmp, 2);
                        
                        //长时间运行概率触发 有未经处理的异常: Microsoft C++ 异常: cv::Exception，位于内存位置
                        for (size_t i = 0; i < KNN_mTmp.size(); i++)
                        {
                            if (KNN_mTmp[i][0].distance < ratio_thresh * KNN_mTmp[i][1].distance)
                            {
#ifdef _DEBUG
                                good_matchesTmp.push_back(KNN_mTmp[i][0]);
#endif
                                if (KNN_mTmp[i][0].queryIdx >= KeyPointMiniMap.size())
                                {
                                    continue;
                                }
                                // 这里有个bug回卡进来，进入副本或者切换放大招时偶尔触发
                                lisx.push_back(((minMap.cols / 2.0 - KeyPointMiniMap[KNN_mTmp[i][0].queryIdx].pt.x) * mapScale + KeyPointSomeMap[KNN_mTmp[i][0].trainIdx].pt.x));
                                lisy.push_back(((minMap.rows / 2.0 - KeyPointMiniMap[KNN_mTmp[i][0].queryIdx].pt.y) * mapScale + KeyPointSomeMap[KNN_mTmp[i][0].trainIdx].pt.y));
                                
                                sumx += lisx.back();
                                sumy += lisy.back();
                            }
                        }
#ifdef _DEBUG
                        cv::Mat img_matches, imgmap, imgminmap;
						drawKeypoints(someMap, KeyPointSomeMap, imgmap, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
						drawKeypoints(img_object, KeyPointMiniMap, imgminmap, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
						drawMatches(img_object, KeyPointMiniMap, someMap, KeyPointSomeMap, good_matchesTmp, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
#endif
                        if (min(lisx.size(), lisy.size()) <= 4)
                        {
                            //有可能处于城镇中
                            
                            /***********************/
                            //重新从完整中地图取出角色周围部分地图
                            img_scene(cv::Rect(cvCeil(hisP[2].x - someSizeR), cvCeil(hisP[2].y - someSizeR), someSizeR * 2, someSizeR * 2)).copyTo(someMap);
                            //Mat minMap(img_object);
                            
                            resize(someMap, someMap, cv::Size(someSizeR * 4, someSizeR * 4));
                            //resize(minMap, minMap, Size(), MatchMatScale, MatchMatScale, 1);
                            
                            detectorSomeMap = cv::xfeatures2d::SURF::create(minHessian);
                            detectorSomeMap->detectAndCompute(someMap, cv::noArray(), KeyPointSomeMap, DataPointSomeMap);
                            //detectorSomeMap->detectAndCompute(minMap, noArray(), KeyPointMinMap, DataPointMinMap);
                            if (KeyPointSomeMap.size() == 0 || KeyPointMiniMap.size() == 0)
                            {
                                isContinuity = false;
                            }
                            else
                            {
                                cv::Ptr<cv::DescriptorMatcher> matcherTmp = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
                                std::vector< std::vector<cv::DMatch> > KNN_mTmp;
#ifdef _DEBUG
                                std::vector<cv::DMatch> good_matchesTmp;
#endif
                                matcherTmp->knnMatch(DataPointMiniMap, DataPointSomeMap, KNN_mTmp, 2);
                                //std::vector<double> lisx;
                                //std::vector<double> lisy;
                                lisx.clear();
                                lisy.clear();
                                //double sumx = 0;
                                //double sumy = 0;
                                sumx = 0;
                                sumy = 0;
                                
                                for (size_t i = 0; i < KNN_mTmp.size(); i++)
                                {
                                    if (KNN_mTmp[i][0].distance < ratio_thresh * KNN_mTmp[i][1].distance)
                                    {
#ifdef _DEBUG
                                        good_matchesTmp.push_back(KNN_mTmp[i][0]);
#endif
                                        // 这里有个bug回卡进来，进入副本或者切换放大招时偶尔触发
                                        if (KNN_mTmp[i][0].queryIdx >= KeyPointMiniMap.size())
                                        {
                                            continue;
                                        }
                                        
                                        lisx.push_back(((minMap.cols / 2.0 - KeyPointMiniMap[KNN_mTmp[i][0].queryIdx].pt.x) * 0.8667 + KeyPointSomeMap[KNN_mTmp[i][0].trainIdx].pt.x));
                                        lisy.push_back(((minMap.rows / 2.0 - KeyPointMiniMap[KNN_mTmp[i][0].queryIdx].pt.y) * 0.8667 + KeyPointSomeMap[KNN_mTmp[i][0].trainIdx].pt.y));
                                        sumx += lisx.back();
                                        sumy += lisy.back();
                                    }
                                }
#ifdef _DEBUG
                                //Mat img_matches, imgmap, imgminmap;
								drawKeypoints(someMap, KeyPointSomeMap, imgmap, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
								drawKeypoints(img_object, KeyPointMiniMap, imgminmap, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

								drawMatches(img_object, KeyPointMiniMap, someMap, KeyPointSomeMap, good_matchesTmp, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
#endif
                                if (min(lisx.size(), lisy.size()) <= 4)
                                {
                                    isContinuity = false;
                                }
                                else
                                {
                                    if (min(lisx.size(), lisy.size()) >= 10)
                                    {
                                        isOnCity = true;
                                    }
                                    else
                                    {
                                        isOnCity = false;
                                    }
                                    
                                    double meanx = sumx / lisx.size(); //均值
                                    double meany = sumy / lisy.size(); //均值
                                    cv::Point2d p = SPC(lisx, sumx, lisy, sumy);
                                    
                                    //int x = (int)meanx;
                                    //int y = (int)meany;
                                    
                                    double x = (p.x - someMap.cols / 2.0) / 2.0;
                                    double y = (p.y - someMap.rows / 2.0) / 2.0;
                                    
                                    pos = cv::Point2d(x + hisP[2].x, y + hisP[2].y);
                                }
                                
                            }
                        }
                        else
                        {
                            isOnCity = false;
                            
                            double meanx = sumx / lisx.size(); //均值
                            double meany = sumy / lisy.size(); //均值
                            cv::Point2d p = SPC(lisx, sumx, lisy, sumy);
                            
                            
                            double x = p.x;
                            double y = p.y;
                            
                            pos = cv::Point2d(x + hisP[2].x - someSizeR, y + hisP[2].y - someSizeR);
                        }
                    }
                }
                else
                {
                    //在城镇中
                    /***********************/
                    //重新从完整中地图取出角色周围部分地图
                    cv::Mat someMap(img_scene(cv::Rect(cvCeil(hisP[2].x - someSizeR), cvCeil(hisP[2].y - someSizeR), someSizeR * 2, someSizeR * 2)));
                    cv::Mat minMap(img_object);
                    
                    //cv::Ptr<cv::xfeatures2d::SURF>& detectorSomeMap = *(cv::Ptr<cv::xfeatures2d::SURF>*)_detectorSomeMap;
                    //std::vector<cv::KeyPoint>& KeyPointSomeMap = *(std::vector<cv::KeyPoint>*)_KeyPointSomeMap;
                    //cv::Mat& DataPointSomeMap = *(cv::Mat*)_DataPointSomeMap;
                    //std::vector<cv::KeyPoint>& KeyPointMiniMap = *(std::vector<cv::KeyPoint>*)_KeyPointMiniMap;
                    //cv::Mat& DataPointMiniMap = *(cv::Mat*)_DataPointMiniMap;
                    
                    cv::Ptr<cv::xfeatures2d::SURF>& detectorSomeMap = _detectorSomeMap;
                    std::vector<cv::KeyPoint>& KeyPointSomeMap = _KeyPointSomeMap;
                    cv::Mat& DataPointSomeMap = _DataPointSomeMap;
                    std::vector<cv::KeyPoint>& KeyPointMiniMap = _KeyPointMiniMap;
                    cv::Mat& DataPointMiniMap = _DataPointMiniMap;
                    
                    
                    resize(someMap, someMap, cv::Size(someSizeR * 4, someSizeR * 4));
                    //resize(minMap, minMap, Size(), MatchMatScale, MatchMatScale, 1);
                    
                    detectorSomeMap = cv::xfeatures2d::SURF::create(minHessian);
                    detectorSomeMap->detectAndCompute(someMap, cv::noArray(), KeyPointSomeMap, DataPointSomeMap);
                    detectorSomeMap->detectAndCompute(minMap, cv::noArray(), KeyPointMiniMap, DataPointMiniMap);
                    
                    if (KeyPointSomeMap.size() != 0 && KeyPointMiniMap.size() != 0)
                    {
                        cv::Ptr<cv::DescriptorMatcher> matcherTmp = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
                        std::vector< std::vector<cv::DMatch> > KNN_mTmp;
#ifdef _DEBUG
                        std::vector<cv::DMatch> good_matchesTmp;
#endif
                        KNN_mTmp.clear();
                        matcherTmp->knnMatch(DataPointMiniMap, DataPointSomeMap, KNN_mTmp, 2);
                        std::vector<double> lisx;
                        std::vector<double> lisy;
                        double sumx = 0;
                        double sumy = 0;
                        
                        for (size_t i = 0; i < KNN_mTmp.size(); i++)
                        {
                            if (KNN_mTmp[i][0].distance < ratio_thresh * KNN_mTmp[i][1].distance)
                            {
#ifdef _DEBUG
                                good_matchesTmp.push_back(KNN_mTmp[i][0]);
#endif
                                // 这里有个bug回卡进来，进入副本或者切换放大招时偶尔触发
                                if (KNN_mTmp[i][0].queryIdx >= KeyPointMiniMap.size())
                                {
                                    continue;
                                }
                                
                                lisx.push_back(((minMap.cols / 2.0 - KeyPointMiniMap[KNN_mTmp[i][0].queryIdx].pt.x) * 0.8667 + KeyPointSomeMap[KNN_mTmp[i][0].trainIdx].pt.x));
                                lisy.push_back(((minMap.rows / 2.0 - KeyPointMiniMap[KNN_mTmp[i][0].queryIdx].pt.y) * 0.8667 + KeyPointSomeMap[KNN_mTmp[i][0].trainIdx].pt.y));
                                sumx += lisx.back();
                                sumy += lisy.back();
                            }
                        }
#ifdef _DEBUG
                        cv::Mat img_matches, imgmap, imgminmap;
						drawKeypoints(someMap, KeyPointSomeMap, imgmap, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
						drawKeypoints(img_object, KeyPointMiniMap, imgminmap, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

						drawMatches(img_object, KeyPointMiniMap, someMap, KeyPointSomeMap, good_matchesTmp, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
#endif
                        if (max(lisx.size(), lisy.size()) > 4)
                        {
                            if (min(lisx.size(), lisy.size()) >= 10)
                            {
                                isOnCity = true;
                            }
                            else
                            {
                                isOnCity = false;
                            }
                            //if(max(lisx.size(), lisy.size()) <=10 )
                            //{
                            //	isOnCity = false;
                            //}
                            
                            double meanx = sumx / lisx.size(); //均值
                            double meany = sumy / lisy.size(); //均值
                            cv::Point2d p = SPC(lisx, sumx, lisy, sumy);
                            
                            double x = (p.x - someMap.cols / 2.0) / 2.0;
                            double y = (p.y - someMap.rows / 2.0) / 2.0;
                            
                            pos = cv::Point2d(x + hisP[2].x, y + hisP[2].y);
                        }
                        else
                        {
                            isContinuity = false;
                        }//if (max(lisx.size(), lisy.size()) > 4)
                    }
                    else
                    {
                        isContinuity = false;
                    }//if (KeyPointSomeMap.size() != 0 && KeyPointMinMap.size() != 0)
                }
            }
        }
    }
    else
    {
        isConveying = true;
    }
    if (!isContinuity)
    {
        //cv::Ptr<cv::xfeatures2d::SURF>& detectorAllMap = *(cv::Ptr<cv::xfeatures2d::SURF>*)_detectorAllMap;
        //std::vector<cv::KeyPoint>& KeyPointAllMap = *(std::vector<cv::KeyPoint>*)_KeyPointAllMap;
        //cv::Mat& DataPointAllMap = *(cv::Mat*)_DataPointAllMap;
        //std::vector<cv::KeyPoint>& KeyPointMiniMap = *(std::vector<cv::KeyPoint>*)_KeyPointMiniMap;
        //cv::Mat& DataPointMiniMap = *(cv::Mat*)_DataPointMiniMap;
        
        cv::Ptr<cv::xfeatures2d::SURF>& detectorAllMap = _detectorAllMap;
        std::vector<cv::KeyPoint>& KeyPointAllMap = _KeyPointAllMap;
        cv::Mat& DataPointAllMap = _DataPointAllMap;
        std::vector<cv::KeyPoint>& KeyPointMiniMap = _KeyPointMiniMap;
        cv::Mat& DataPointMiniMap = _DataPointMiniMap;
        
        detectorAllMap->detectAndCompute(img_object, cv::noArray(), KeyPointMiniMap, DataPointMiniMap);
        
        if (KeyPointMiniMap.size() == 0)
        {
            //err = 4;//未能匹配到特征点
            return false;
        }
        else
        {
            cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
            std::vector< std::vector<cv::DMatch> > KNN_m;
#ifdef _DEBUG
            std::vector<cv::DMatch> good_matches;
#endif
            matcher->knnMatch(DataPointMiniMap, DataPointAllMap, KNN_m, 2);
            
            std::vector<double> lisx;
            std::vector<double> lisy;
            double sumx = 0;
            double sumy = 0;
            for (size_t i = 0; i < KNN_m.size(); i++)
            {
                if (KNN_m[i][0].distance < ratio_thresh * KNN_m[i][1].distance)
                {
#ifdef _DEBUG
                    good_matches.push_back(KNN_m[i][0]);
#endif
                    if (KNN_m[i][0].queryIdx >= KeyPointMiniMap.size())
                    {
                        continue;
                    }
                    lisx.push_back(((img_object.cols / 2.0 - KeyPointMiniMap[KNN_m[i][0].queryIdx].pt.x) * mapScale + KeyPointAllMap[KNN_m[i][0].trainIdx].pt.x));
                    lisy.push_back(((img_object.rows / 2.0 - KeyPointMiniMap[KNN_m[i][0].queryIdx].pt.y) * mapScale + KeyPointAllMap[KNN_m[i][0].trainIdx].pt.y));
                    sumx += lisx.back();
                    sumy += lisy.back();
                }
            }
#ifdef _DEBUG
            cv::Mat img_matches, imgmap, imgminmap;
			drawKeypoints(img_scene, KeyPointAllMap, imgmap, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
			drawKeypoints(img_object, KeyPointMiniMap, imgminmap, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

			drawMatches(img_object, KeyPointMiniMap, img_scene, KeyPointAllMap, good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
#endif
            
            if (lisx.size() == 0 || lisy.size() == 0)
            {
                //err = 4;//未能匹配到特征点
                log->error("lisx.size() == 0 || lisy.size() == 0");
                return false;
            }
            else
            {
                pos = SPC(lisx, sumx, lisy, sumy);
            }
        }
    }
    
    if (isConveying)
    {
        pos = posFilter.reinitfilterting(pos);
        //isConveying = false;
    }
    else
    {
        pos = posFilter.filterting(pos);
    }
    
    hisP[0] = hisP[1];
    hisP[1] = hisP[2];
    hisP[2] = pos;
    
    pos = TransferTianLiAxes(pos * MapAbsScale, MapWorldOffset, MapWorldScale);
    
    pos = TransferUserAxes(pos, UserWorldOrigin_X, UserWorldOrigin_Y, UserWorldScale);
    
    _out_value=pos;
    return true;
}

II_2_Direction::II_2_Direction(const Logger &logService) {
    log= logService;
}

void II_2_Direction::setValue(const cv::Mat &mat) {
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

double II_2_Direction::getValue() {
    std::lock_guard<std::mutex> lock(lm_output);
    auto ret=_out_value;
    return std::move(ret);
}

bool II_2_Direction::getMatchOutput() {
    //计算角色方向
    //TODO: 临时的加载资源方案
    
    //TODO: 如何匹配的细节
    
    // 判断入参为空的情况
    if (_mat.empty())
    {
        log->warning("Matching_RightGetItems::getMatchOutput() _mat is empty");
        return false;
    }
    // 判断入参是否为4通道图
    if (_mat.type() != CV_8UC4)
    {
        log->warning("Matching_RightGetItems::getMatchOutput() _mat is not CV_8UC4");
        return false;
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
    
    // 入参赋予有意义引用变量名
    cv::Mat obj = _mat; 
    cv::Mat giAvatarRef;

    int Avatar_Rect_x = cvRound(obj.cols * 0.4);
    int Avatar_Rect_y = cvRound(obj.rows * 0.4);
    int Avatar_Rect_w = cvRound(obj.cols * 0.2);
    int Avatar_Rect_h = cvRound(obj.rows * 0.2);

    giAvatarRef=obj(cv::Rect(Avatar_Rect_x, Avatar_Rect_y, Avatar_Rect_w, Avatar_Rect_h));

    cv::resize(giAvatarRef, giAvatarRef, cv::Size(), 2, 2);
    std::vector<cv::Mat> lis;
    cv::split(giAvatarRef, lis);
    
    cv::Mat gray0;
    cv::Mat gray1;
    cv::Mat gray2;
    
    cv::threshold(lis[0], gray0, 240, 255, cv::THRESH_BINARY);
    cv::threshold(lis[1], gray1, 212, 255, cv::THRESH_BINARY);
    cv::threshold(lis[2], gray2, 25, 255, cv::THRESH_BINARY_INV);
    
    cv::Mat and12;
    cv::bitwise_and(gray1, gray2, and12);// , gray0);
    cv::resize(and12, and12, cv::Size(), 2, 2, 3);
    cv::Canny(and12, and12, 20, 3 * 20, 3);
    cv::circle(and12, cv::Point(cvCeil(and12.cols / 2), cvCeil(and12.rows / 2)), cvCeil(and12.cols / 4.5), cv::Scalar(0, 0, 0), -1);
    cv::Mat dilate_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    cv::dilate(and12, and12, dilate_element);
    cv::Mat erode_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    cv::erode(and12, and12, erode_element);
    
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarcy;
    
    cv::findContours(and12, contours, hierarcy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    
    std::vector<cv::Rect> boundRect(contours.size());  //定义外接矩形集合
    cv::Point2f rect[4];
    
    std::vector<cv::Point2d> AvatarKeyPoint;
    double AvatarKeyPointLine[3] = { 0 };
    std::vector<cv::Point2f> AvatarKeyLine;
    cv::Point2f KeyLine;
    
    if (contours.size() != 3) {
        err = 205;//提取小箭头特征误差过大
        return false;
    }
        for (int i = 0; i < 3; i++)
        {
            boundRect[i] = cv::boundingRect(cv::Mat(contours[i]));
            AvatarKeyPoint.push_back(cv::Point(cvRound(boundRect[i].x + boundRect[i].width / 2), cvRound(boundRect[i].y + boundRect[i].height / 2)));
        }
        
        AvatarKeyPointLine[0] = dis(AvatarKeyPoint[2] - AvatarKeyPoint[1]);
        AvatarKeyPointLine[1] = dis(AvatarKeyPoint[2] - AvatarKeyPoint[0]);
        AvatarKeyPointLine[2] = dis(AvatarKeyPoint[1] - AvatarKeyPoint[0]);
        
        if (AvatarKeyPointLine[0] >= AvatarKeyPointLine[2] && AvatarKeyPointLine[1] >= AvatarKeyPointLine[2])
        {
            AvatarKeyLine.push_back(AvatarKeyPoint[2] - AvatarKeyPoint[1]);
            AvatarKeyLine.push_back(AvatarKeyPoint[2] - AvatarKeyPoint[0]);
        }
        if (AvatarKeyPointLine[0] >= AvatarKeyPointLine[1] && AvatarKeyPointLine[2] >= AvatarKeyPointLine[1])
        {
            AvatarKeyLine.push_back(AvatarKeyPoint[1] - AvatarKeyPoint[0]);
            AvatarKeyLine.push_back(AvatarKeyPoint[1] - AvatarKeyPoint[2]);
        }
        if (AvatarKeyPointLine[1] >= AvatarKeyPointLine[0] && AvatarKeyPointLine[2] >= AvatarKeyPointLine[0])
        {
            AvatarKeyLine.push_back(AvatarKeyPoint[0] - AvatarKeyPoint[1]);
            AvatarKeyLine.push_back(AvatarKeyPoint[0] - AvatarKeyPoint[2]);
        }
        
        AvatarKeyLine = Vector2UnitVector(AvatarKeyLine);
        KeyLine = AvatarKeyLine[0] + AvatarKeyLine[1];
        
    _out_value = Line2Angle(KeyLine);
    return true;
}

II_3_Rotation::II_3_Rotation(const Logger &logService) {
    log= logService;
}

void II_3_Rotation::setValue(const cv::Mat &mat) {
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

double II_3_Rotation::getValue() {
    std::lock_guard<std::mutex> lock(lm_output);
    auto ret=_out_value;
    return std::move(ret);
}

bool II_3_Rotation::getMatchOutput() {
    //计算角色旋转角度
    //TODO: 临时的加载资源方案
    
    //TODO: 如何匹配的细节
    
    // 判断入参为空的情况
    if (_mat.empty())
    {
        log->warning("Matching_RightGetItems::getMatchOutput() _mat is empty");
        return false;
    }
    
    // 入参赋予有意义引用变量名
    cv::Mat giMiniMapRef=_mat;
    
    cv::Mat img_object(giMiniMapRef(cv::Rect(40, 40, giMiniMapRef.cols - 80, giMiniMapRef.rows - 80)));
    
    if (img_object.channels() != 4)
    {
        err = 401;
        return false;
    }
    
    std::vector<cv::Mat>scr_channels;
    
    split(img_object, scr_channels);
    
    cv::Mat Alpha = scr_channels[3];
    
    Alpha = 255.0 - Alpha;
    
    Alpha = Alpha * 2;
    
    cv::threshold(Alpha, Alpha, 150, 0, cv::THRESH_TOZERO_INV);
    cv::threshold(Alpha, Alpha, 50, 0, cv::THRESH_TOZERO);
    cv::threshold(Alpha, Alpha, 50, 255, cv::THRESH_BINARY);
    
    cv::circle(Alpha, cv::Point(Alpha.cols / 2, Alpha.rows / 2), min(Alpha.cols / 2, Alpha.rows / 2) * 1.21, cv::Scalar(0, 0, 0), min(Alpha.cols / 2, Alpha.rows / 2) * 0.42);
    cv::circle(Alpha, cv::Point(Alpha.cols / 2, Alpha.rows / 2), min(Alpha.cols / 2, Alpha.rows / 2) * 0.3, cv::Scalar(0, 0, 0), -1);
    
    
    cv::Mat dilate_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4));
    cv::dilate(Alpha, Alpha, dilate_element);
    cv::Mat erode_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4));
    cv::erode(Alpha, Alpha, erode_element);
    
    erode_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4));
    cv::erode(Alpha, Alpha, erode_element);
    dilate_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4));
    cv::dilate(Alpha, Alpha, dilate_element);
    
    
    //传入黑白图
    //根据白块部分计算视角中心坐标
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarcy;
    
    cv::findContours(Alpha, contours, hierarcy, 0, 1);
    
    std::vector<cv::Rect> boundRect(contours.size());  //定义外接矩形集合
    
    if (contours.size() == 0)
    {
        err = 402;//
        return false;
    }
    
    cv::Point p;
    int maxBlack = 0;
    int maxId = 0;
    
    for (int i = 0; i < contours.size(); i++)
    {
        if (contours[i].size() > maxBlack)
        {
            maxBlack = contours[i].size();
            maxId = i;
        }
        boundRect[i] = cv::boundingRect(cv::Mat(contours[i]));
        
    }
    
    p = cv::Point(boundRect[maxId].x + boundRect[maxId].width / 2, boundRect[maxId].y + boundRect[maxId].height / 2);
    
// #ifdef _DEBUG
//     circle(Alpha, p, 3, cv::Scalar(255, 0, 0));
//     line(Alpha, p, cv::Point(img_object.cols / 2, img_object.rows / 2), cv::Scalar(0, 255, 0));
//     cv::imshow("Img", Alpha);
//     cv::waitKey(1);
// #endif
    p = p - cv::Point(img_object.cols / 2, img_object.rows / 2);
    
    _out_value = Line2Angle(p);
    return true;
}

II_4_StarJson::II_4_StarJson(const Logger &logService) {
    log= logService;
}

void II_4_StarJson::setValue(const cv::Mat &mat) {
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

std::string II_4_StarJson::getValue() {
    std::lock_guard<std::mutex> lock(lm_output);
    auto ret=_out_value;
    return std::move(ret);
}

bool II_4_StarJson::getMatchOutput() {
    //计算神瞳Json
    
    return false;
}
