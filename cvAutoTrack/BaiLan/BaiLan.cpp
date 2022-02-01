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

    // 初始化摆烂
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

bool BaiLan::GetDirection(double &a) {
    a = _direction.a;
    log->Log("结果: "+std::to_string(_direction.result)+"  获取角度: "+std::to_string(_direction.a));
    return _direction.result;
}

bool BaiLan::GetRotation(double &a2) {
    a2 = _rotation.a2;
    log->Log("结果: "+ std::to_string(_rotation.result) +"  获取角度: "+std::to_string(a2));
    return _rotation.result;
}

bool BaiLan::GetStarJson(char *string) {
    return false;
}

bool BaiLan::GetLastError() {
    return false;
}

void BaiLan::service() {
    log->Log("进入服务主线程");
    // 任务循环是否继续
    while (_isRunService) {
        std::unique_lock<std::mutex> unlck_get_signal(_mutex_get_signal);
        // 等待信号, 如果没有信号, 则等待30秒
        _cv_get_signal.wait_for(unlck_get_signal, std::chrono::seconds(1), [&]()->bool{return false;});

        // 是否执行任务，如果不执行任务, 则继续等待信号
        if (_isStartService) {
            // 触发获取原神句柄
            log->Log("触发获取原神句柄");
            _cv_get_handle.notify_all();
            // 发送触发获取原神句柄后休眠一段时间，防止获取失败的时候循环触发频率过高。
            // 触发后延时33毫秒
            std::this_thread::sleep_for(std::chrono::milliseconds(33));
            //std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
    }
    log->Log("结束服务主线程");
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

    _cv_get_handle.notify_all();
    _cv_get_frame.notify_all();
    _cv_get_splitarea.notify_all();
    _cv_get_split_matching.notify_all();

    // 获取所有带锁变量的锁
    std::unique_lock<std::mutex> lock_output_giHandle(_mutex_GI_Handle);
    std::unique_lock<std::mutex> lock_output_giFrame(_mutex_GI_Frame);
    std::unique_lock<std::mutex> lock_output_splitArea(_mutex_GI_SplitArea);
    std::unique_lock<std::mutex> lock_output_split_1_paimonArea(_mutex_GI_Split_1_PaimonArea);
    std::unique_lock<std::mutex> lock_output_split_2_miniMapArea(_mutex_GI_Split_2_MiniMapArea);
    std::unique_lock<std::mutex> lock_output_split_3_UIDArea(_mutex_GI_Split_3_UIDArea);
    std::unique_lock<std::mutex> lock_output_split_4_leftGetItemsArea(_mutex_GI_Split_4_LeftGetItemsArea);
    std::unique_lock<std::mutex> lock_output_split_5_rightGetItemsArea(_mutex_GI_Split_5_RightGetItemsArea);

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
    log->Log("进入获取原神句柄线程");
    while (_isRunService) {
        std:unique_lock<std::mutex> unlck_get_handle(_mutex_GI_Handle);
        // 等待信号, 如果没有信号, 则等待1秒
        _cv_get_handle.wait_for(unlck_get_handle, 1s, []()->bool {return false;});
        if (_isStartService)// 是否工作
        {
            if( [&] () ->bool{_result_giHandle = false;return Get_GI_Handle(gi_Handle);}())
            {//获取原神句柄：成功
                
                //触发获取原神截图
                log->Log("获得句柄，触发获取原神截图");
                _cv_get_frame.notify_all();
                //设置【一直截图】标志位为真
                _is_always_get_GI_Frame=true;
            }
            else
            {//获取原神句柄：失败
                
                //获取失败后需要重新运行上一步
                //触发获取原神句柄
                log->Info("获取原神句柄：失败，触发获取原神句柄");
                //_cv_get_handle.notify_all();
            }
        }
    }
}

void BaiLan::get_GI_Frame_server() {
    log->Info("进入获取原神截图线程启动");
    while (_isRunService)
    {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_Handle);
        // 等待信号, 如果没有信号, 则等待1纳秒。
        // 【一直截图】标志位为真就一直执行，否则就重新执行上一步
        //TODO: 注掉后会提高截图速度
        _cv_get_frame.wait(uniqueLock);//,1s,[&](){return false;});
        double t=0;
        while(_isStartService)
        {
            t = static_cast<double>(cv::getTickCount());
            
            if(_is_always_get_GI_Frame && [&]()->bool{
                std::unique_lock<std::mutex> lock_output(_mutex_GI_Frame);
                return Get_GI_Frame(gi_Handle, gi_Frame);
            }())
            {
                // 触发下一步，通知拆分分发
                log->Log("获得截图，触发下一步，通知拆分分发");
                _cv_get_splitarea.notify_all();
    
                // 自适应截图频率到 30 fps
                // 计算等待时间
                t= (static_cast<double>(cv::getTickCount())-t)/cv::getTickFrequency();
                if(t<0.033)
                {
                    int need_wait_ms=(0.033-t>0?0.033-t:0)*1000;
                    std::this_thread::sleep_for(std::chrono::milliseconds(need_wait_ms));
                    log->Log("need_wait_ms: "+std::to_string(static_cast<int>(need_wait_ms)));
                }
                //std::this_thread::sleep_for(33ms);
            }
            else
            {
                // 截图失败，重新获取句柄并等待100毫秒
                _cv_get_handle.notify_all();
                log->Info("截图失败，重新获取句柄");
                // 等待100毫秒
                std::this_thread::sleep_for(1ms);
                break;
            }
            
            log->Log("fps: "+std::to_string(static_cast<int>(1.0/t)));
        }
    }
}


void BaiLan::get_GI_SplitArea_server() {
    log->Info(" 02 获取截图拆分线程启动");
    while (_isRunService)
    {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_Frame);
        _cv_get_splitarea.wait(uniqueLock);
        //_cv_get_frame.notify_all();
        if (_isStartService) {
            if( [&]()->bool{
                std::unique_lock<std::mutex> lock_output(_mutex_GI_SplitArea);
                return Get_GI_SplitArea(gi_Frame, gi_SplitArea);
            }())
            {
                //foo_test_show_Frame();
                _cv_get_split_matching.notify_all();
                //log->Log("split_matching.notify_one()");
            }
            else
            {
                //_cv_get_frame.notify_all();
                log->Info("back frame.notify_all() form Get_GI_SplitArea");
            }
        }
    }
}

void BaiLan::foo_test_show_Frame() const {
    Mat         gray_paimonArea;
    vector<Mat> list;
    // static_cast<int>(giFrame.cols / 5.0*3.0),
// static_cast<int>(giFrame.rows / 4.0),
// static_cast<int>(giFrame.cols / 6.0),
// static_cast<int>(giFrame.rows / 2.0)
//矩形参数
    int         r_left=             static_cast<int>(gi_Frame.cols / 10.0 * 5.70);  // %57
    int         r_top=              static_cast<int>(gi_Frame.rows / 10.0 * 2.50);  // %25
    int         r_width=            static_cast<int>(gi_Frame.cols / 10.0 * 2.25);  // %16
    int         r_height=           static_cast<int>(gi_Frame.rows / 10.0 * 5.00);  // %50
    rectangle(gi_Frame,
              Rect(r_left,r_top,r_width,r_height),
              Scalar(255,255,255,255), 3);
    // static_cast<int>(giFrame.cols / 20.0),
// static_cast<int>(giFrame.rows / 2.0),
// static_cast<int>(giFrame.cols / 6.0),
// static_cast<int>(giFrame.rows / 3.0)
    r_left=             static_cast<int>(gi_Frame.cols / 10.0 * 0.50);             // %5
    r_top=              static_cast<int>(gi_Frame.rows / 10.0 * 4.60);             // %46
    r_width=            static_cast<int>(gi_Frame.cols / 10.0 * 1.60);             // %16
    r_height=           static_cast<int>(gi_Frame.rows / 10.0 * 4.80);             // %50
    
    rectangle(gi_Frame,
              Rect(r_left,r_top,r_width,r_height),
              Scalar(255,255,255,255), 3);
    split(gi_Frame, list);
    //cv::cvtColor(, gray_paimonArea, cv::COLOR_BGR2GRAY);
    imshow("gi_Frame", list[list.size()-1]);
    
    waitKey(1);
}

void BaiLan::get_GI_Split_1_PaimonArea_server() {
    log->Info(" 03 01 获取分发1——派蒙的识别区域线程启动");
    while (_isRunService)
    {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_SplitArea);
        _cv_get_split_matching.wait(uniqueLock);
        if (_isStartService )
        {
           
            
            if([&]() -> bool {
                std::unique_lock<std::mutex> lock_output(_mutex_GI_Split_1_PaimonArea);
                return Get_GI_SplitMatching_1_PaimonArea(gi_SplitArea[0], gi_Area_Paimon);
            }())
            {
                //foo_test_show_PaimonArea();

                //TODO: 分图之后的识别
                //_cv_get_split_Rect_.notify_all();
                //log->Log("split_Rect.notify_one() TODO:");
            }
            else
            {
                //_cv_get_splitarea.notify_all();
                log->Info("back splitarea.notify_all() form Get_GI_SplitMatching_1_PaimonArea");
            }
        }
    }
}

void BaiLan::foo_test_show_PaimonArea() {// 计算帧率
    double      t =static_cast<double > (getTickCount());
    Mat         gray_paimonArea;
    vector<Mat> list;
    split(gi_SplitArea[0], list);
    //cv::cvtColor(, gray_paimonArea, cv::COLOR_BGR2GRAY);
    imshow("PaimonArea", list[list.size()-1]);
    
    waitKey(1);
    
    //Clion
    
    t= (static_cast<double>(getTickCount()) - t) / getTickFrequency();
    int fps= static_cast<int>(1.0/t);
    //log->Info("fps:"+std::to_string(fps));
}

void BaiLan::get_GI_Split_2_MiniMapArea_server() {
    log->Info(" 03 02 获取分发2——小地图的识别区域线程启动");
    while (_isRunService) {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_SplitArea);
        _cv_get_split_matching.wait(uniqueLock);
        if (_isStartService) {
           if([&]()->bool{
               std::unique_lock<std::mutex> lock_output(_mutex_GI_Split_2_MiniMapArea);
                return Get_GI_SplitMatching_2_MiniMapArea(gi_SplitArea[1], gi_Area_MiniMap);
            }())
           {
               //foo_test_show_MiniMapArea();
               //TODO: 分图之后的识别
               //_cv_get_split_Rect_.notify_all();
               //log->Log("split_Rect.notify_one() TODO:");
           }
           else
           {
               //_cv_get_splitarea.notify_all();
               log->Info("back splitarea.notify_all() form Get_GI_SplitMatching_2_MiniMapArea");
           }
        }
    }
}

void BaiLan::foo_test_show_MiniMapArea() {
    try {
        //cv::Mat gray_miniMapArea;
        /// std::vector<cv::Mat> list;
        /// cv::split(gi_SplitArea[1],list);
        /// //cv::cvtColor(, gray_paimonArea, cv::COLOR_BGR2GRAY);
        /// int kernel_size =11;
        /// int scale = 1;
        /// int delta = 0;
        /// int ddepth = CV_16S;
        /// cv::Mat abs_dst,dst;
        /// Laplacian( list[list.size()-1], dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
        /// convertScaleAbs( dst, abs_dst );
///
///
        /// kernel_size =3;
///
        /// Laplacian( abs_dst, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
        /// convertScaleAbs( dst, abs_dst );
///
        /// vector<Vec3f> circles;
        /// double dp = 2; //
        /// double minDist = 10;  //两个圆心之间的最小距离
        /// double  param1 = 100;  //Canny边缘检测的较大阈值
        /// double  param2 = 100;  //累加器阈值
        /// int min_radius = 20;  //圆形半径的最小值
        /// int max_radius = 100;  //圆形半径的最大值
        /// HoughCircles(abs_dst, circles, HOUGH_GRADIENT, dp, minDist, param1, param2,
        ///              min_radius, max_radius);
        /// //灰度图变为彩色
        /// cvtColor(abs_dst,abs_dst,cv::COLOR_GRAY2BGR);
///
        /// std::sort( circles.begin(), circles.end(),[](const Vec3f& a,const Vec3f& b)->bool{return a[2]>b[2];});
        /// //图像中标记出圆形
        /// for (size_t i = 0; i < circles.size()>1?1:0; i++)
        /// {
        ///     //读取圆心
        ///     Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        ///     //读取半径
        ///     int radius = cvRound(circles[i][2]);
        ///     //绘制圆心
        ///     circle(abs_dst, center, 3, Scalar(0, 255, 0), -1, 8, 0);
        ///     //绘制圆
        ///     circle(abs_dst, center, radius, Scalar(0, 0, 255), 3, 8, 0);
        /// }
        //cv::Mat gray_miniMapArea;
        //cv::cvtColor(gi_SplitArea[1], gray_miniMapArea, cv::COLOR_RGBA2GRAY);
        //cv::imshow("miniMap",gray_miniMapArea );


       // cv::waitKey(1);
    }
    catch (exception &e)
    {
        log->Error("get_GI_Split_2_MiniMapArea_server error:" + string(e.what()));
    }
    vector<Mat> list;
    split(gi_SplitArea[1], list);
    imshow("MiniMapArea", list[3]);
    waitKey(1);
}

void BaiLan::get_GI_Split_3_UIDArea_server() {
    log->Info(" 03 03 获取分发3——UID的识别区域线程启动");
    while (_isRunService) {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_SplitArea);
        _cv_get_split_matching.wait(uniqueLock);
        if (_isStartService) {
            if( [&]()->bool{
                std::unique_lock<std::mutex> lock_output(_mutex_GI_Split_3_UIDArea);
                return Get_GI_SplitMatching_3_UIDArea(gi_SplitArea[2], gi_Area_UID);
            }())
            {
                //foo_test_show_UIDArea();
                //TODO: 分图之后的识别
                //_cv_get_split_Rect_.notify_all();
                //log->Log("split_Rect.notify_one() TODO:");
            }
            else
            {
                //_cv_get_splitarea.notify_all();
                log->Info("back splitarea.notify_all() form Get_GI_SplitMatching_3_UIDArea");
            }
        }
    }
}

void BaiLan::foo_test_show_UIDArea() {
    imshow("UIDArea", gi_SplitArea[2]);
    waitKey(1);
}

void BaiLan::get_GI_Split_4_LeftGetItemsArea_server() {
    log->Info(" 03 04 获取分发4——左侧获取物品区域线程启动");
    while (_isRunService) {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_SplitArea);
        _cv_get_split_matching.wait(uniqueLock);
        if (_isStartService ) {
            if ( [&]()->bool{
                std::unique_lock<std::mutex> lock_output(_mutex_GI_Split_4_LeftGetItemsArea);
                return Get_GI_SplitMatching_4_LeftGetItemsArea(gi_SplitArea[3], gi_Area_LeftGetItems);
            }())
            {
                //foo_test_show_LeftGetItemsArea();
                //TODO: 分图之后的识别
                //_cv_get_split_Rect_.notify_all();
                //log->Log("split_Rect.notify_one() TODO:");
            }
            else
            {
                //_cv_get_splitarea.notify_all();
                log->Info("back splitarea.notify_all() form Get_GI_SplitMatching_4_LeftGetItemsArea");
            }
        }
    }
}

void BaiLan::foo_test_show_LeftGetItemsArea() {
    imshow("LeftGetItemsArea", gi_SplitArea[3]);
    waitKey(1);
}

void BaiLan::get_GI_Split_5_RightGetItemsArea_server() {
    log->Info(" 03 04 获取分发5——右侧获取物品区域线程启动");
    while (_isRunService) {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_SplitArea);
        _cv_get_split_matching.wait(uniqueLock);
        if (_isStartService) {
            if([&]()->bool{
                    std::unique_lock<std::mutex> lock_output(_mutex_GI_Split_5_RightGetItemsArea);
                return Get_GI_SplitMatching_5_RightGetItemsArea(gi_SplitArea[4], gi_Area_RightGetItems);
            }())
            {
                //foo_test_show_RightGetItemsArea();
                //TODO: 分图之后的识别
                //_cv_get_split_Rect_.notify_all();
                //log->Log("split_Rect.notify_one() TODO:");
            }
            else
            {
                //_cv_get_splitarea.notify_all();
                log->Info("back splitarea.notify_all() form Get_GI_SplitMatching_5_RightGetItemsArea");
            }
        }
    }
}

void BaiLan::foo_test_show_RightGetItemsArea() {
    imshow("RightGetItemsArea", gi_SplitArea[4]);
    //	SetWindowPos(thisHandle, HWND_TOP, GIS.giRect.left + offGiMinMap.x, GIS.giRect.top + offGiMinMap.y, 0, 0, SWP_NOSIZE);
    waitKey(1);
}


