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
    //隐式使用的通用参数
    bool isShowPaimon= false;

public:
    BaiLan();

    ~BaiLan();

    bool startService();

    bool stopService();

    /// 获取角色朝向
    /// \param a 输出角色朝向
    /// \return 是否成功获取
    bool GetDirection(double &a);

    bool GetRotation(double &a2);

    bool GetStarJson(char *string);

    bool GetLastError();

private:
    void service();
    
    void createTasks();

    void beginTasks();

    void endTasks();

private:
    // 获取触发信号

    std::condition_variable _cv_get_signal;
    std::mutex _mutex_get_signal;

private:
    //获取原神句柄

    //触发条件变量
    std::condition_variable _cv_get_handle;
    HWND gi_Handle;
    std::thread *_t_get_GI_Handle;
    std::mutex _mutex_GI_Handle;

    //获取原神句柄线程服务
    void get_GI_Handle_server();

private:
    //获取原神截图

    
    std::condition_variable _cv_get_frame;
    cv::Mat gi_Frame;
    std::thread *_t_get_GI_Frame;
    std::mutex _mutex_GI_Frame;
    //是否【一直截图】标志位
    std::atomic<bool> _is_always_get_GI_Frame = false;

    //获取原神截图线程服务
    void get_GI_Frame_server();

private:
    //获取拆分分发

    std::condition_variable _cv_get_splitarea;
    std::vector<cv::Mat> gi_SplitArea;
    std::thread *_t_get_GI_SplitArea;
    std::mutex _mutex_GI_SplitArea;
    //获取截图拆分线程服务
    void get_GI_SplitArea_server();

private:
    //所有分发的共同激活信号
    std::condition_variable _cv_get_split_matching;
private:
    //获取分发1——派蒙的识别区域

    cv::Mat gi_Area_Paimon;
    std::thread *_t_get_GI_Split_1_PaimonArea;
    std::mutex _mutex_GI_Split_1_PaimonArea;

    //获取分发1——派蒙的识别区域线程服务
    void get_GI_Split_1_PaimonArea_server();
    
private:
    //获取分发2——小地图的识别区域

    cv::Mat gi_Area_MiniMap;
    std::thread *_t_get_GI_Split_2_MiniMapArea;
    std::mutex _mutex_GI_Split_2_MiniMapArea;

    //获取分发2——小地图的识别区域线程服务
    void get_GI_Split_2_MiniMapArea_server();
    
private:
    //获取分发3——UID的识别区域

    cv::Mat gi_Area_UID;
    std::thread *_t_get_GI_Split_3_UIDArea;
    std::mutex _mutex_GI_Split_3_UIDArea;

    //获取分发3——UID的识别区域线程服务
    void get_GI_Split_3_UIDArea_server();


private:
    //获取分发4——左侧已获取物品的识别区域

    cv::Mat gi_Area_LeftGetItems;
    std::thread *_t_get_GI_Split_4_LeftGetItemsArea;
    std::mutex _mutex_GI_Split_4_LeftGetItemsArea;

    //获取分发4——左侧已获取物品的识别区域线程服务
    void get_GI_Split_4_LeftGetItemsArea_server();

private:
    //获取分发5——右侧已捡取物品的识别区域

    cv::Mat gi_Area_RightGetItems;
    std::thread *_t_get_GI_Split_5_RightGetItemsArea;
    std::mutex _mutex_GI_Split_5_RightGetItemsArea;

    //获取分发5——右侧已捡取物品的识别区域
    void get_GI_Split_5_RightGetItemsArea_server();

private:
    void foo_test_show_Frame() const;
    
    void foo_test_show_PaimonArea();
    
    void foo_test_show_MiniMapArea();
    
    void foo_test_show_UIDArea();
    
    void foo_test_show_LeftGetItemsArea();
    
    void foo_test_show_RightGetItemsArea();
    
//初步识别部分
private:
    //识别分发1——派蒙的具体区域
    std::condition_variable _cv_matching_paimon;
    bool gi_MatchOutput_Paimon;
    std::thread *_t_get_GI_Matching_1_Paimon;
    std::mutex _mutex_GI_Matching_1_Paimon;
    
    //识别分发1——派蒙的具体区域线程服务
    void get_GI_Matching_1_Paimon_server();

private:
    //识别分发2——小地图的具体区域
    std::condition_variable _cv_matching_minimap;
    cv::Point2d gi_MatchOutput_MiniMap;
    std::thread *_t_get_GI_Matching_2_MiniMap;
    std::mutex _mutex_GI_Matching_2_MiniMap;
    
    //识别分发2——小地图的具体区域线程服务
    void get_GI_Matching_2_MiniMap_Server();
    
private:
    //识别分发3——UID的具体区域
    std::condition_variable _cv_matching_uid;
    int gi_MatchOutput_UID;
    std::thread *_t_get_GI_Matching_3_UID;
    std::mutex _mutex_GI_Matching_3_UID;
    
    //识别分发3——UID的具体区域线程服务
    void get_GI_Matching_3_UID_Server();
};

#endif //CVAUTOTRACKDLL_BAILAN_H/
