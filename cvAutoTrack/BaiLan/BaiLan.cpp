//
// Created by GengG on 2022/1/30.
//

#include "BaiLan.h"
// 引入标准输出日志
#include "../Logger/StandardOutputLogger.h"
//static Logger logger = std::make_shared<StandardOutputLogger>();

//引入摆烂相关的函数
#include "BaiLan_Util.h"

BaiLan::BaiLan() {
    log = std::make_shared<StandardOutputLogger>();

    log->Log("开始摆烂... ...");
    _thread = new std::thread(&BaiLan::service, this);
    _isRunService = true;
    createTasks();
    beginTasks();
}

BaiLan::~BaiLan() {
    endTasks();
    _isRunService = false;
    _isStartService = false;
    _thread->join();
    delete _thread;
    log->Log("管理主线程删除成功");
    log->Log("结束摆烂... ...");
}

bool BaiLan::startService() {
    log->Log("开启服务");
    _isStartService = true;
    return true;
}

bool BaiLan::stopService() {
    log->Log("停止服务");
    _isStartService = false;
    return true;
}

void BaiLan::service() {
    log->Log("进入服务主线程");
    // 任务循环是否继续
    while (_isRunService) {
        // 是否执行任务
        if (_isStartService) {
            // //计算帧率
            // static int64_t _lastTime=0;
            // int64_t now=cv::getTickCount();

            // 获取原神句柄
            {
                // 触发获取原神句柄
                if (_is_need_update_giHandle) {
                    //改用新的信号
                    _cv_get_handle.notify_one();
                    _is_need_update_giHandle = false;
                }

                //获取原神句柄部分
                if (_result_giHandle) {
                    _is_need_update_giHandle = false;

                    // 触发获取原神截图
                    _is_need_update_giFrame = true;
                } else {
                    _is_need_update_giHandle = true;
                    //等待 3s 后再次尝试
                    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
                }
            }

            // 获取原神截图
            {
                // 是否触发获取原神截图
                if (_is_need_update_giFrame) {
                    _gi_getFrame_signal = true;
                }

                //是否输出日志——关于原神截图
                static bool IsOutputInfo_giFrame = true;

                // 获取原神画面部分
                if (_result_giFrame) {
                    _gi_getFrame_signal = false;
                    cv::imshow("原神", gi_Frame);
                    // 触发画面拆分分发
                    _is_need_update_giSplitArea = true;
                } else {
                    _is_need_update_giFrame = true;
                }
            }

            // 获取原神画面拆分区域
            {
                // 是否触发获取画面拆分分发
                if (_is_need_update_giSplitArea) {
                    _gi_getSplitArea_signal = true;
                }

                //是否输出日志——关于画面拆分分发
                static bool IsOutputInfo_giSplitArea = true;

                //获取画面拆分分发部分
                if (_result_giSplitArea) {
                    _gi_getSplitArea_signal = false;

                    try {
                        cv::imshow("派蒙可能性区域", gi_SplitArea[0].clone());
                        cv::imshow("小地图可能性区域", gi_SplitArea[1].clone());
                        cv::imshow("UID可能性区域", gi_SplitArea[2].clone());
                        cv::imshow("左侧已获取物品可能性区域", gi_SplitArea[3].clone());
                        cv::imshow("右侧可捡取物品可能性区域", gi_SplitArea[4].clone());
                    }
                    catch (exception &e) {
                        //输出异常信息字符串到日志
                        log->Warning("SplitArea :" + std::string(e.what()));
                    }

                    // 触发获取分发指令
                    _is_need_update_giDispatch = true;
                } else {
                    //_is_need_update_giSplitArea = true;
                }
            }

            if (_is_need_update_giDispatch) {
                _is_need_update_giSplit_1_PaimonArea = true;
                _is_need_update_giSplit_2_MiniMapArea = true;
                _is_need_update_giSplit_3_UIDArea = true;
                _is_need_update_giSplit_4_LeftGetItemsArea = true;
                _is_need_update_giSplit_5_RightGetItemsArea = true;

                _is_need_update_giDispatch = false;
            }

            //获取分发1——派蒙识别区域
            {
                // 是否触发获取分发1——派蒙识别区域
                if (_is_need_update_giSplit_1_PaimonArea) {
                    _gi_getSplit_1_PaimonArea_signal = true;
                }

                //是否输出日志——关于分发1——派蒙识别区域
                static bool IsOutputInfo_giSplit_1_PaimonArea = true;

                //获取分发1——派蒙识别区域部分
                if (_result_giSplit_1_PaimonArea) {
                    _gi_getSplit_1_PaimonArea_signal = false;

                    log->Info("gi_Area_Paimon:" + std::to_string(gi_Area_Paimon.width) + "," +
                              std::to_string(gi_Area_Paimon.width));
                    // 触发获取派蒙识别结果
                    // _is_need_update_Matching_2_Paimon = true;
                } else {
                    //_is_need_update_giSplit_1_PaimonArea = true;
                }
            }
            //获取分发2——小地图识别区域
            {
                // 是否触发获取分发2——小地图识别区域
                if (_is_need_update_giSplit_2_MiniMapArea) {
                    _gi_getSplit_2_MiniMapArea_signal = true;
                }

                //是否输出日志——关于分发2——小地图识别区域
                static bool IsOutputInfo_giSplit_2_MiniMapArea = true;

                //获取分发2——小地图识别区域部分
                if (_result_giSplit_2_MiniMapArea) {
                    _gi_getSplit_2_MiniMapArea_signal = false;
                    // 触发获取小地图识别区域
                    // _is_need_update_Matching_2_MiniMap = true;
                } else {
                    //_is_need_update_giSplit_2_MiniMapArea = true;
                }
            }
            //获取分发3——UID识别结果
            {
                // 是否触发获取分发3——UID识别区域
                if (_is_need_update_giSplit_3_UIDArea) {
                    _gi_getSplit_3_UIDArea_signal = true;
                }

                //是否输出日志——关于分发3——UID识别区域
                static bool IsOutputInfo_giSplit_3_UIDArea = true;

                //获取分发3——UID识别区域部分
                if (_result_giSplit_3_UIDArea) {
                    _gi_getSplit_3_UIDArea_signal = false;
                    // 触发获取UID识别区域
                    // _is_need_update_Matching_2_UID = true;
                } else {
                    //_is_need_update_giSplit_3_UIDArea = true;
                }
            }
            //获取分发4——左侧已获取物品的识别区域
            {
                // 是否触发获取分发4——左侧已获取物品的识别区域
                if (_is_need_update_giSplit_4_LeftGetItemsArea) {
                    _gi_getSplit_4_LeftGetItemsArea_signal = true;
                }

                //是否输出日志——关于分发4——左侧已获取物品的识别区域
                static bool IsOutputInfo_giSplit_4_LeftGetItemsArea = true;

                //获取分发4——左侧已获取物品的识别区域
                if (_result_giSplit_4_LeftGetItemsArea) {
                    _gi_getSplit_4_LeftGetItemsArea_signal = false;
                    if (IsOutputInfo_giSplit_4_LeftGetItemsArea)log->Info("更新gi_Split_4_LeftGetItemsArea成功");
                    IsOutputInfo_giSplit_4_LeftGetItemsArea = false;

                    // 触发获取左侧已获取物品的识别区域
                    // _is_need_update_Matching_2_LeftGetItems = true;
                } else {
                    //_is_need_update_giSplit_4_LeftGetItemsArea = true;
                    //if(IsOutputInfo_giSplit_4_LeftGetItemsArea)log->Info("更新gi_Split_4_LeftGetItemsArea失败，再次尝试");
                    IsOutputInfo_giSplit_4_LeftGetItemsArea = true;
                }
            }
            //获取分发5——右侧已捡取物品的识别区域
            {
                // 是否触发获取分发5——右侧已捡取物品的识别区域
                if (_is_need_update_giSplit_5_RightGetItemsArea) {
                    _gi_getSplit_5_RightGetItemsArea_signal = true;
                }

                //是否输出日志——关于分发5——右侧已捡取物品的识别区域
                static bool IsOutputInfo_giSplit_5_RightGetItemsArea = true;

                //获取分发5——右侧已捡取物品的识别区域
                if (_result_giSplit_5_RightGetItemsArea) {
                    _gi_getSplit_5_RightGetItemsArea_signal = false;
                    if (IsOutputInfo_giSplit_5_RightGetItemsArea)log->Info("更新gi_Split_5_RightGetItemsArea成功");
                    IsOutputInfo_giSplit_5_RightGetItemsArea = false;

                    // 触发获取右侧已捡取物品的识别区域
                    // _is_need_update_Matching_2_LeftGetItems = true;
                } else {
                    //_is_need_update_giSplit_5_RightGetItemsArea = true;
                    //if(IsOutputInfo_giSplit_5_RightGetItemsArea)log->Info("更新gi_Split_5_RightGetItemsArea失败，再次尝试");
                    IsOutputInfo_giSplit_5_RightGetItemsArea = true;
                }
            }


            cv::waitKey(3);
        } else {

            //TODO: 线程休眠
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
    log->Log("结束服务主线程");
}


bool BaiLan::GetDirection(double &a) {
    a = _direction.a;
    return _direction.result;
}

bool BaiLan::GetRotation(double &a2) {
    a2 = _rotation.a2;
    return _rotation.result;
}

bool BaiLan::GetStarJson(char *string) {
    return false;
}

bool BaiLan::GetLastError() {
    return false;
}

void BaiLan::createTasks() {
}

void BaiLan::beginTasks() {
    //TODO: 添加任务后需要添加代码的位置

    //创建所有子服务线程
    _t_get_GI_Handle = new std::thread(&BaiLan::get_GI_Handle_server, this);
    _t_get_GI_Frame = new std::thread(&BaiLan::get_GI_Frame_server, this);
    _t_get_GI_SplitArea = new std::thread(&BaiLan::get_GI_SplitArea_server, this);
    _t_get_GI_Split_1_PaimonArea = new std::thread(&BaiLan::get_GI_Split_1_PaimonArea_server, this);
    _t_get_GI_Split_2_MiniMapArea = new std::thread(&BaiLan::get_GI_Split_2_MiniMapArea_server, this);
    _t_get_GI_Split_3_UIDArea = new std::thread(&BaiLan::get_GI_Split_3_UIDArea_server, this);
    _t_get_GI_Split_4_LeftGetItemsArea = new std::thread(&BaiLan::get_GI_Split_4_LeftGetItemsArea_server, this);
    _t_get_GI_Split_5_RightGetItemsArea = new std::thread(&BaiLan::get_GI_Split_5_RightGetItemsArea_server, this);


}

void BaiLan::endTasks() {
    //TODO: 添加任务后需要添加代码的位置

    // 获取所有带锁变量的锁
    std::lock_guard<std::mutex> lock_output_giHandle(_mutex_GI_Handle);
    std::lock_guard<std::mutex> lock_output_giFrame(_mutex_GI_Frame);
    std::lock_guard<std::mutex> lock_output_splitArea(_mutex_GI_SplitArea);
    std::lock_guard<std::mutex> lock_output_split_1_paimonArea(_mutex_GI_Split_1_PaimonArea);
    std::lock_guard<std::mutex> lock_output_split_2_miniMapArea(_mutex_GI_Split_2_MiniMapArea);
    std::lock_guard<std::mutex> lock_output_split_3_UIDArea(_mutex_GI_Split_3_UIDArea);
    std::lock_guard<std::mutex> lock_output_split_4_leftGetItemsArea(_mutex_GI_Split_4_LeftGetItemsArea);
    std::lock_guard<std::mutex> lock_output_split_5_rightGetItemsArea(_mutex_GI_Split_5_RightGetItemsArea);

    // join 所有线程
    if (_t_get_GI_Handle != nullptr) {
        _t_get_GI_Handle->join();
    }
    if (_t_get_GI_Frame != nullptr) {
        _t_get_GI_Frame->join();
    }
    if (_t_get_GI_SplitArea != nullptr) {
        _t_get_GI_SplitArea->join();
    }
    if (_t_get_GI_Split_1_PaimonArea != nullptr) {
        _t_get_GI_Split_1_PaimonArea->join();
    }
    if (_t_get_GI_Split_2_MiniMapArea != nullptr) {
        _t_get_GI_Split_2_MiniMapArea->join();
    }
    if (_t_get_GI_Split_3_UIDArea != nullptr) {
        _t_get_GI_Split_3_UIDArea->join();
    }
    if (_t_get_GI_Split_4_LeftGetItemsArea != nullptr) {
        _t_get_GI_Split_4_LeftGetItemsArea->join();
    }
    if (_t_get_GI_Split_5_RightGetItemsArea != nullptr) {
        _t_get_GI_Split_5_RightGetItemsArea->join();
    }
}


void BaiLan::get_GI_Handle_server() {
    log->Info("获取原神句柄线程启动");


    while (_isRunService) {
        std:unique_lock<std::mutex> uniqueLock(_mutex_GI_Handle);
        if (_isStartService)// 是否工作
        {
            _result_giHandle = get_GI_Handle(gi_Handle);
        }
        //触发信号
        _cv_get_handle.wait(uniqueLock);
    }
}

bool BaiLan::get_GI_Handle(HWND &giHandle) {
    _result_giHandle = false;
    return Get_GI_Handle(giHandle);
}

void BaiLan::get_GI_Frame_server() {
    log->Info("获取原神截图线程启动");
    while (_isRunService)
    {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_Handle);
        _cv_get_frame.wait(uniqueLock);
        if (_isStartService ) {
            std::lock_guard<std::mutex> lock_output(_mutex_GI_Frame);
            _result_giFrame = get_GI_Frame(gi_Handle, gi_Frame);
        }
    }
}

bool BaiLan::get_GI_Frame(HWND &giHandle, cv::Mat &giFrame) {
    _result_giFrame = false;
    return Get_GI_Frame(giHandle, giFrame);
}

void BaiLan::get_GI_SplitArea_server() {
    log->Info("获取截图拆分线程启动");
    while (_isRunService) {
        if (_isStartService && _gi_getSplitArea_signal) {
            std::lock_guard<std::mutex> lock_input(_mutex_GI_Frame);
            std::lock_guard<std::mutex> lock_output(_mutex_GI_SplitArea);

            _result_giSplitArea = get_GI_SplitArea(gi_Frame, gi_SplitArea);
            _gi_getSplitArea_signal = false;
        } else {
            //TODO: 线程休眠
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

bool BaiLan::get_GI_SplitArea(Mat &giFrame, vector<cv::Mat> &giSplitArea) {
    _result_giSplitArea = false;
    return Get_GI_SplitArea(giFrame, giSplitArea);
}

void BaiLan::get_GI_Split_1_PaimonArea_server() {
    log->Info("获取分发1——派蒙的识别区域线程启动");
    while (_isRunService) {
        if (_isStartService && _gi_getSplit_1_PaimonArea_signal) {
            std::lock_guard<std::mutex> lock_input(_mutex_GI_SplitArea);
            std::lock_guard<std::mutex> lock_output(_mutex_GI_Split_1_PaimonArea);

            _result_giSplit_1_PaimonArea = get_GI_Split_1_PaimonArea(gi_SplitArea[0], gi_Area_Paimon);
            _gi_getSplit_1_PaimonArea_signal = false;

        } else {
            //TODO: 线程休眠
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

bool BaiLan::get_GI_Split_1_PaimonArea(Mat &giSplitArea, Rect &giArea_Paimon) {
    _result_giSplit_1_PaimonArea = false;
    return Get_GI_SplitMatching_1_PaimonArea(giSplitArea, giArea_Paimon);
}

void BaiLan::get_GI_Split_2_MiniMapArea_server() {
    log->Info("获取分发2——小地图的识别区域线程启动");
    while (_isRunService) {
        if (_isStartService && _gi_getSplit_2_MiniMapArea_signal) {
            std::lock_guard<std::mutex> lock_input(_mutex_GI_SplitArea);
            std::lock_guard<std::mutex> lock_output(_mutex_GI_Split_2_MiniMapArea);

            _result_giSplit_2_MiniMapArea = get_GI_Split_2_MiniMapArea(gi_SplitArea[1], gi_Area_MiniMap);
            _gi_getSplit_2_MiniMapArea_signal = false;
        } else {
            //TODO: 线程休眠
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

}

bool BaiLan::get_GI_Split_2_MiniMapArea(Mat &giSplitArea_MiniMap, Rect &giArea_MiniMap) {
    _result_giSplit_2_MiniMapArea = false;
    return Get_GI_SplitMatching_2_MiniMapArea(giSplitArea_MiniMap, giArea_MiniMap);
}

void BaiLan::get_GI_Split_3_UIDArea_server() {
    log->Info("获取分发3——UID的识别区域线程启动");
    while (_isRunService) {
        if (_isStartService && _gi_getSplit_3_UIDArea_signal) {
            std::lock_guard<std::mutex> lock_input(_mutex_GI_SplitArea);
            std::lock_guard<std::mutex> lock_output(_mutex_GI_Split_3_UIDArea);

            _result_giSplit_3_UIDArea = get_GI_Split_3_UIDArea(gi_SplitArea[2], gi_Area_UID);
            _gi_getSplit_3_UIDArea_signal = false;

        } else {
            //TODO: 线程休眠
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

bool BaiLan::get_GI_Split_3_UIDArea(Mat &giSplitArea_UID, Rect &giArea_UID) {
    _result_giSplit_3_UIDArea = false;
    return Get_GI_SplitMatching_3_UIDArea(giSplitArea_UID, giArea_UID);
}

void BaiLan::get_GI_Split_4_LeftGetItemsArea_server() {
    log->Info("获取分发4——左侧获取物品区域线程启动");
    while (_isRunService) {
        if (_isStartService && _gi_getSplit_4_LeftGetItemsArea_signal) {
            std::lock_guard<std::mutex> lock_input(_mutex_GI_SplitArea);
            std::lock_guard<std::mutex> lock_output(_mutex_GI_Split_4_LeftGetItemsArea);

            _result_giSplit_4_LeftGetItemsArea = get_GI_Split_4_LeftGetItemsArea(gi_SplitArea[3], gi_Area_LeftGetItems);
            _gi_getSplit_4_LeftGetItemsArea_signal = false;
        } else {
            //TODO: 线程休眠
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

bool BaiLan::get_GI_Split_4_LeftGetItemsArea(Mat &giSplitArea_LeftGetItems, Rect &giArea_LeftGetItems) {
    _result_giSplit_4_LeftGetItemsArea = false;
    return Get_GI_SplitMatching_4_LeftGetItemsArea(giSplitArea_LeftGetItems, giArea_LeftGetItems);
}

void BaiLan::get_GI_Split_5_RightGetItemsArea_server() {
    log->Info("获取分发5——右侧获取物品区域线程启动");
    while (_isRunService) {
        if (_isStartService && _gi_getSplit_5_RightGetItemsArea_signal) {
            std::lock_guard<std::mutex> lock_input(_mutex_GI_SplitArea);
            std::lock_guard<std::mutex> lock_output(_mutex_GI_Split_5_RightGetItemsArea);

            _result_giSplit_5_RightGetItemsArea = get_GI_Split_5_RightGetItemsArea(gi_SplitArea[4],
                                                                                   gi_Area_RightGetItems);
            _gi_getSplit_5_RightGetItemsArea_signal = false;
        } else {
            //TODO: 线程休眠
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

bool BaiLan::get_GI_Split_5_RightGetItemsArea(Mat &giSplitArea_RightGetItems, Rect &giArea_RightGetItems) {
    _result_giSplit_5_RightGetItemsArea = false;
    return Get_GI_SplitMatching_5_RightGetItemsArea(giSplitArea_RightGetItems, giArea_RightGetItems);
}


