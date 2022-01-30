//
// Created by GengG on 2022/1/30.
//

#ifndef CVAUTOTRACKDLL_BAILAN_H
#define CVAUTOTRACKDLL_BAILAN_H

#include "../Logger/LoggingFacility.h"
#include "../Data/DataTypeBase.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <windows.h>

#include <opencv2/opencv.hpp>

class BaiLan {

    Logger log;

    std::thread *_thread = nullptr;
    std::atomic<bool> _isRunService;
    std::atomic<bool> _isStartService;

private:
    // 用到的接口数据类型
    Transform _transform;
    Position _position;
    Direction _direction;
    Rotation _rotation;
    Star _star;
    StarJson _starJson;
    UID _uid;
    InfoLoadPicture _infoLoadPicture;
    InfoLoadVideo _infoLoadVideo;


public:
    BaiLan();

    ~BaiLan();

    bool startService();

    bool stopService();

    void service();

    /// 获取角色朝向
    /// \param a 输出角色朝向
    /// \return 是否成功获取
    bool GetDirection(double &a);

    bool GetRotation(double &a2);

    bool GetStarJson(char *string);

    bool GetLastError();

private:
    void createTasks();

    void beginTasks();

    void endTasks();

private:
    //获取原神句柄

    //触发条件变量
    std::condition_variable _cv_get_handle;
    std::atomic<bool> _is_need_update_giHandle = true; //是否需要更新原神句柄
    std::atomic<bool> _result_giHandle = false; //是否成功获取原神句柄
    HWND gi_Handle;
    std::thread *_t_get_GI_Handle;
    std::mutex _mutex_GI_Handle;

    //获取原神句柄线程服务
    void get_GI_Handle_server();

    bool get_GI_Handle(HWND &giHandle);

private:
    //获取原神截图

    std::condition_variable _cv_get_frame;
    //是否需要更新原神截图
    std::atomic<bool> _is_need_update_giFrame = false;
    //是否成功获取原神截图
    std::atomic<bool> _result_giFrame = false;
    cv::Mat gi_Frame;
    std::thread *_t_get_GI_Frame;
    std::mutex _mutex_GI_Frame;

    //获取原神截图线程服务
    void get_GI_Frame_server();

    bool get_GI_Frame(HWND &giHandle, cv::Mat &giFrame);

private:
    //获取拆分分发

    //是否需要更新拆分分发
    std::atomic<bool> _is_need_update_giSplitArea = false;
    //是否成功拆分原神截图
    std::atomic<bool> _result_giSplitArea = false;
    std::vector<cv::Mat> gi_SplitArea;
    std::thread *_t_get_GI_SplitArea;
    std::mutex _mutex_GI_SplitArea;
    std::atomic<bool> _gi_getSplitArea_signal;

    //获取截图拆分线程服务
    void get_GI_SplitArea_server();

    bool get_GI_SplitArea(cv::Mat &giFrame, std::vector<cv::Mat> &giSplitArea);

private:
    //所有分发的共同激活信号
    std::atomic<bool> _is_need_update_giDispatch = false;
private:
    //获取分发1——派蒙的识别区域

    //是否需要更新分发1——派蒙的识别区域
    std::atomic<bool> _is_need_update_giSplit_1_PaimonArea = false;
    //是否成功获取分发1——派蒙的识别区域
    std::atomic<bool> _result_giSplit_1_PaimonArea = false;
    cv::Rect gi_Area_Paimon;
    std::thread *_t_get_GI_Split_1_PaimonArea;
    std::mutex _mutex_GI_Split_1_PaimonArea;
    std::atomic<bool> _gi_getSplit_1_PaimonArea_signal;

    //获取分发1——派蒙的识别区域线程服务
    void get_GI_Split_1_PaimonArea_server();

    bool get_GI_Split_1_PaimonArea(cv::Mat &giSplitArea_Paimon, cv::Rect &giArea_Paimon);

private:
    //获取分发2——小地图的识别区域

    //是否需要更新分发2——小地图的识别区域
    std::atomic<bool> _is_need_update_giSplit_2_MiniMapArea = false;
    //是否成功获取分发2——小地图的识别区域
    std::atomic<bool> _result_giSplit_2_MiniMapArea = false;
    cv::Rect gi_Area_MiniMap;
    std::thread *_t_get_GI_Split_2_MiniMapArea;
    std::mutex _mutex_GI_Split_2_MiniMapArea;
    std::atomic<bool> _gi_getSplit_2_MiniMapArea_signal;

    //获取分发2——小地图的识别区域线程服务
    void get_GI_Split_2_MiniMapArea_server();

    bool get_GI_Split_2_MiniMapArea(cv::Mat &giSplitArea_MiniMap, cv::Rect &giArea_MiniMap);

private:
    //获取分发3——UID的识别区域

    //是否需要更新分发3——UID的识别区域
    std::atomic<bool> _is_need_update_giSplit_3_UIDArea = false;
    //是否成功获取分发3——UID的识别区域
    std::atomic<bool> _result_giSplit_3_UIDArea = false;
    cv::Rect gi_Area_UID;
    std::thread *_t_get_GI_Split_3_UIDArea;
    std::mutex _mutex_GI_Split_3_UIDArea;
    std::atomic<bool> _gi_getSplit_3_UIDArea_signal;

    //获取分发3——UID的识别区域线程服务
    void get_GI_Split_3_UIDArea_server();

    bool get_GI_Split_3_UIDArea(cv::Mat &giSplitArea_UID, cv::Rect &giArea_UID);

private:
    //获取分发4——左侧已获取物品的识别区域

    //是否需要更新分发4——左侧已获取物品的识别区域
    std::atomic<bool> _is_need_update_giSplit_4_LeftGetItemsArea = false;
    //是否成功获取分发4——左侧已获取物品的识别区域
    std::atomic<bool> _result_giSplit_4_LeftGetItemsArea = false;
    cv::Rect gi_Area_LeftGetItems;
    std::thread *_t_get_GI_Split_4_LeftGetItemsArea;
    std::mutex _mutex_GI_Split_4_LeftGetItemsArea;
    std::atomic<bool> _gi_getSplit_4_LeftGetItemsArea_signal;

    //获取分发4——左侧已获取物品的识别区域线程服务
    void get_GI_Split_4_LeftGetItemsArea_server();

    bool get_GI_Split_4_LeftGetItemsArea(cv::Mat &giSplitArea_LeftGetItems, cv::Rect &giArea_LeftGetItems);

private:
    //获取分发5——右侧已捡取物品的识别区域

    //是否需要更新分发5——右侧已捡取物品的识别区域
    std::atomic<bool> _is_need_update_giSplit_5_RightGetItemsArea = false;
    //是否成功获取分发5——右侧已捡取物品的识别区域
    std::atomic<bool> _result_giSplit_5_RightGetItemsArea = false;
    cv::Rect gi_Area_RightGetItems;
    std::thread *_t_get_GI_Split_5_RightGetItemsArea;
    std::mutex _mutex_GI_Split_5_RightGetItemsArea;
    std::atomic<bool> _gi_getSplit_5_RightGetItemsArea_signal;

    //获取分发5——右侧已捡取物品的识别区域
    void get_GI_Split_5_RightGetItemsArea_server();

    bool get_GI_Split_5_RightGetItemsArea(cv::Mat &giSplitArea_RightGetItems, cv::Rect &giArea_RightGetItems);

};

#endif //CVAUTOTRACKDLL_BAILAN_H/
