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
    
    log->setLogLevel(LogLevel::Info);
    // 初始化摆烂
    log->log("开始摆烂... ...");
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
    log->log("管理主线程删除成功");
    log->log("结束摆烂... ...");
}

bool BaiLan::startService() {
    log->log("开启服务");
    _isStartService = true;
    return true;
}

bool BaiLan::stopService() {
    log->log("停止服务");
    _isStartService = false;
    return true;
}
bool BaiLan::init(){
    return true;
}
bool BaiLan::uninit() {
    return true;
}
bool BaiLan::load() {
    return true;
}
bool BaiLan::release() {
    return true;
}
bool BaiLan::SetHandle(long long int handle) {
    return true;
}
bool BaiLan::SetWorldCenter(double x, double y) {
    return true;
}
bool BaiLan::SetWorldScale(double scale) {
    return true;
}

bool BaiLan::GetTransform(double& x, double& y, double& a)
{
    if(_transform.result)
    {
        x = _transform.x;
        y = _transform.y;
        a = _transform.a;
    }

    return _transform.result;
}
bool BaiLan::GetPosition(double &x,double &y){
    if(_position.result)
    {
        x = _position.x;
        y = _position.y;
    }
    log->log("结果"+std::to_string(_position.result)+"  获取位置: "+std::to_string(_position.x)+"  "+std::to_string(_position.y));
    return _position.result;
}
bool BaiLan::GetDirection(double &a) {
    if(_direction.result)
    {
        a = _direction.a;
    }
    log->log("结果: " + std::to_string(_direction.result) + "  获取角度: " + std::to_string(_direction.a));
    return _direction.result;
}

bool BaiLan::GetRotation(double &a2) {
    if(_rotation.result)
    {
        a2 = _rotation.a2;
    }
    log->log("结果: " + std::to_string(_rotation.result) + "  获取角度: " + std::to_string(a2));
    return _rotation.result;
}

bool BaiLan::GetStar(double& x, double& y, bool& isEnd)
{
    return false;
}

bool BaiLan::GetStarJson(char *string) {
    return false;
}

bool BaiLan::GetLastError() {
    return false;
}

bool BaiLan::GetUID(int &uid) {
    if(_uid.result)
    {
        uid = _uid.uid;
    }
    log->log("结果: " + std::to_string(_uid.result) + "  获取UID: " + std::to_string(_uid.uid));
    return _uid.result;
}

void BaiLan::service() {
    log->log("进入服务主线程");
    // 任务循环是否继续
    while (_isRunService) {
        std::unique_lock<std::mutex> unlck_get_signal(_mutex_get_signal);
        // 等待信号, 如果没有信号, 则等待30秒
        _cv_get_signal.wait_for(unlck_get_signal, std::chrono::seconds(1), [&]()->bool{return false;});

        // 是否执行任务，如果不执行任务, 则继续等待信号
        if (_isStartService) {
            // 触发获取原神句柄
            //log->Log("触发获取原神句柄");
            _cv_get_handle.notify_all();
            // 发送触发获取原神句柄后休眠一段时间，防止获取失败的时候循环触发频率过高。
            // 触发后延时33毫秒
            std::this_thread::sleep_for(std::chrono::milliseconds(33));
            //std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
    }
    log->log("结束服务主线程");
}

void BaiLan::createTasks() {
}

void BaiLan::beginTasks() {
    //TODO: 添加任务后需要添加代码的位置

    //创建所有子服务线程
    _t_get_GI_Handle    = new std::thread(&BaiLan::get_GI_Handle_server, this);
    _t_get_GI_Frame     = new std::thread(&BaiLan::get_GI_Frame_server, this);
    _t_get_GI_SplitArea = new std::thread(&BaiLan::get_GI_SplitArea_server, this);
    
    _t_get_GI_Split_1_PaimonArea        = new std::thread(&BaiLan::get_GI_Split_1_PaimonArea_server, this);
    _t_get_GI_Split_2_MiniMapArea       = new std::thread(&BaiLan::get_GI_Split_2_MiniMapArea_server, this);
    _t_get_GI_Split_3_UIDArea           = new std::thread(&BaiLan::get_GI_Split_3_UIDArea_server, this);
    _t_get_GI_Split_4_LeftGetItemsArea  = new std::thread(&BaiLan::get_GI_Split_4_LeftGetItemsArea_server, this);
    _t_get_GI_Split_5_RightGetItemsArea = new std::thread(&BaiLan::get_GI_Split_5_RightGetItemsArea_server, this);

    //初步识别部分
    _t_get_GI_Matching_1_Paimon        = new std::thread(&BaiLan::get_GI_Matching_1_Paimon_server,this);
    _t_get_GI_Matching_2_MiniMap       = new std::thread(&BaiLan::get_GI_Matching_2_MiniMap_Server,this);
    _t_get_GI_Matching_3_UID           = new std::thread(&BaiLan::get_GI_Matching_3_UID_Server,this);
    _t_get_GI_Matching_4_LeftGetItems  = new std::thread(&BaiLan::get_GI_Matching_4_LeftGetItems_Server, this);
    _t_get_GI_Matching_5_RightGetItems = new std::thread(&BaiLan::get_GI_Matching_5_RightGetItems_Server, this);
    
    //二级分发部分
    _t_II_1_Position    = new std::thread(&BaiLan::get_GI_II_1_Position_Server,this);
    _t_II_2_Direction   = new std::thread(&BaiLan::get_GI_II_2_Direction_Server,this);
    _t_II_3_Rotation    = new std::thread(&BaiLan::get_GI_II_3_Rotation_Server,this);
    _t_II_4_StarJson    = new std::thread(&BaiLan::get_II_4_StarJson_Server,this);
}

void BaiLan::endTasks() {
    //TODO: 添加任务后需要添加代码的位置

    _cv_get_handle.notify_all();
    _cv_get_frame.notify_all();
    _cv_get_splitarea.notify_all();
    
    _cv_get_split_matching.notify_all();
    
    _cv_matching_paimon.notify_all();
    _cv_matching_minimap.notify_all();
    _cv_matching_uid.notify_all();
    _cv_matching_leftgetitems.notify_all();
    _cv_matching_rightgetitems.notify_all();
    
    _cv_get_II_split_matching.notify_all();
    
    // _cv_II_1_position.notify_all();
    // _cv_II_2_direction.notify_all();
    // _cv_II_3_rotation.notify_all();
    // _cv_II_4_starjson.notify_all();
    
    // 获取所有带锁变量的锁
    std::unique_lock<std::mutex> lock_output_giHandle(_mutex_GI_Handle);
    std::unique_lock<std::mutex> lock_output_giFrame(_mutex_GI_Frame);
    std::unique_lock<std::mutex> lock_output_splitArea(_mutex_GI_SplitArea);
    std::unique_lock<std::mutex> lock_output_split_1_paimonArea(_mutex_GI_Split_1_PaimonArea);
    std::unique_lock<std::mutex> lock_output_split_2_miniMapArea(_mutex_GI_Split_2_MiniMapArea);
    std::unique_lock<std::mutex> lock_output_split_3_UIDArea(_mutex_GI_Split_3_UIDArea);
    std::unique_lock<std::mutex> lock_output_split_4_leftGetItemsArea(_mutex_GI_Split_4_LeftGetItemsArea);
    std::unique_lock<std::mutex> lock_output_split_5_rightGetItemsArea(_mutex_GI_Split_5_RightGetItemsArea);
    std::unique_lock<std::mutex> lock_output_matchoutput_1_paimon(_mutex_GI_Matching_1_Paimon);
    std::unique_lock<std::mutex> lock_output_matchoutput_2_minimap(_mutex_GI_Matching_2_MiniMap);
    std::unique_lock<std::mutex> lock_output_matchoutput_3_uid(_mutex_GI_Matching_3_UID);
    std::unique_lock<std::mutex> lock_output_matchoutput_4_leftgetitems(_mutex_Matching_4_LeftGetItems);
    std::unique_lock<std::mutex> lock_output_matchoutput_5_rightgetitems(_mutex_Matching_5_RightGetItems);
    std::unique_lock<std::mutex> lock_output_II_1_position(_mutex_II_1_Position);
    std::unique_lock<std::mutex> lock_output_II_2_direction(_mutex_II_2_Direction);
    std::unique_lock<std::mutex> lock_output_II_3_rotation(_mutex_II_3_Rotation);
    std::unique_lock<std::mutex> lock_output_II_4_starjson(_mutex_II_4_StarJson);
    
    
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
    
    //初步识别部分
    if(_t_get_GI_Matching_1_Paimon!=nullptr){
        _t_get_GI_Matching_1_Paimon->join();
    }
    if(_t_get_GI_Matching_2_MiniMap!= nullptr){
        _t_get_GI_Matching_2_MiniMap->join();
    }
    if(_t_get_GI_Matching_3_UID!= nullptr){
        _t_get_GI_Matching_3_UID->join();
    }
    if(_t_get_GI_Matching_4_LeftGetItems != nullptr){
        _t_get_GI_Matching_4_LeftGetItems->join();
    }
    if(_t_get_GI_Matching_5_RightGetItems != nullptr){
        _t_get_GI_Matching_5_RightGetItems->join();
    }
    
    //二级分发
    if(_t_II_1_Position!= nullptr)
    {
        _t_II_1_Position->join();
    }
    if(_t_II_2_Direction!= nullptr)
    {
        _t_II_2_Direction->join();
    }
    if(_t_II_3_Rotation!= nullptr)
    {
        _t_II_3_Rotation->join();
    }
    if(_t_II_4_StarJson!= nullptr)
    {
        _t_II_4_StarJson->join();
    }
}

//进入获取原神句柄线程
void BaiLan::get_GI_Handle_server() {
    log->log("进入获取原神句柄线程");
    while (_isRunService) {
        std::unique_lock<std::mutex> unlck_get_handle(_mutex_GI_Handle);
        // 等待信号, 如果没有信号, 则等待1秒
        _cv_get_handle.wait_for(unlck_get_handle, 1s, []()->bool {return false;});
        if (_isStartService)// 是否工作
        {
            if( [&] () ->bool{return Get_GI_Handle(log,gi_Handle);}())
            {//获取原神句柄：成功
                
                //触发获取原神截图
                //log->Log("获得句柄，触发获取原神截图");
                _cv_get_frame.notify_all();
                //设置【一直截图】标志位为真
                _is_always_get_GI_Frame=true;
            }
            else
            {//获取原神句柄：失败
                
                //获取失败后需要重新运行上一步
                //触发获取原神句柄
                log->info("获取原神句柄：失败，触发获取原神句柄");
                //_cv_get_handle.notify_all();
            }
        }
    }
}
//进入获取原神截图线程
void BaiLan::get_GI_Frame_server() {
    log->info("进入获取原神截图线程");
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
                return Get_GI_Frame(log, gi_Handle, gi_Frame);
            }())
            {
                // 触发下一步，通知拆分分发
                //log->Log("获得截图，触发下一步，通知拆分分发");
                _cv_get_splitarea.notify_all();
    
                // 自适应截图频率到 30 fps
                // 计算等待时间
                // t= (static_cast<double>(cv::getTickCount())-t)/cv::getTickFrequency();
                // if(t<0.033)
                // {
                //     int need_wait_ms=(0.033-t>0?0.033-t:0)*1000;
                //     std::this_thread::sleep_for(std::chrono::milliseconds(need_wait_ms));
                //     log->Log("need_wait_ms: "+std::to_string(static_cast<int>(need_wait_ms)));
                // }
                
                //TODO: 需要测试是否真的需要加延时，不加延时为什么会卡
                std::this_thread::sleep_for(1ms);
            }
            else
            {
                // 截图失败，重新获取句柄并等待100毫秒
                _cv_get_handle.notify_all();
                log->info("截图失败，重新获取句柄");
                // 等待100毫秒
                std::this_thread::sleep_for(1ms);
                break;
            }
            
            //log->Log("fps: "+std::to_string(static_cast<int>(1.0/t)));
        }
    }
}
//进入获取截图拆分线程
void BaiLan::get_GI_SplitArea_server() {
    log->info("进入获取截图拆分线程");
    while (_isRunService)
    {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_Frame);
        _cv_get_splitarea.wait(uniqueLock);
        //_cv_get_frame.notify_all();
        if (_isStartService) {
            if( [&]()->bool{
                std::unique_lock<std::mutex> lock_output(_mutex_GI_SplitArea);
                return Get_GI_SplitArea(log, gi_Frame, gi_SplitArea);
            }())
            {
                //foo_test_show_Frame();
                _cv_get_split_matching.notify_all();
                //log->Log("split_matching.notify_one()");
            }
            else
            {
                //_cv_get_frame.notify_all();
                log->info("back frame.notify_all() form Get_GI_SplitArea");
            }
        }
    }
}
//进入获取分发1——派蒙的识别区域线程
void BaiLan::get_GI_Split_1_PaimonArea_server() {
    log->info("进入获取分发1——派蒙的识别区域线程");
    while (_isRunService)
    {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_SplitArea);
        _cv_get_split_matching.wait(uniqueLock);
        if (_isStartService )
        {
            if([&]() -> bool {
                std::unique_lock<std::mutex> lock_output(_mutex_GI_Split_1_PaimonArea);
                return Get_GI_SplitMatching_1_PaimonArea(log, gi_SplitArea[0], gi_Area_Paimon);
            }())
            {
                //foo_test_show_PaimonArea();
                //TODO: 分图之后的识别
                _cv_matching_paimon.notify_all();
            }
            else
            {
                //_cv_get_splitarea.notify_all();
                log->info("back splitarea.notify_all() form Get_GI_SplitMatching_1_PaimonArea");
            }
        }
    }
}
//进入获取分发2——小地图的识别区域线程
void BaiLan::get_GI_Split_2_MiniMapArea_server() {
    log->info("进入获取分发2——小地图的识别区域线程");
    while (_isRunService) {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_SplitArea);
        _cv_get_split_matching.wait(uniqueLock);
        if (_isStartService) {
           if([&]()->bool{
               std::unique_lock<std::mutex> lock_output(_mutex_GI_Split_2_MiniMapArea);
                return Get_GI_SplitMatching_2_MiniMapArea(log, gi_SplitArea[1], gi_Area_MiniMap);
            }())
           {
               //foo_test_show_MiniMapArea();
               //TODO: 分图之后的识别
               _cv_matching_minimap.notify_all();
               //log->error("split_Rect.notify_one() TODO:");
           }
           else
           {
               //_cv_get_splitarea.notify_all();
               log->info("back splitarea.notify_all() form Get_GI_SplitMatching_2_MiniMapArea");
           }
        }
    }
}
//进入获取分发3——UID的识别区域线程
void BaiLan::get_GI_Split_3_UIDArea_server() {
    log->info("进入获取分发3——UID的识别区域线程");
    while (_isRunService) {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_SplitArea);
        _cv_get_split_matching.wait(uniqueLock);
        if (_isStartService) {
            if( [&]()->bool{
                std::unique_lock<std::mutex> lock_output(_mutex_GI_Split_3_UIDArea);
                return Get_GI_SplitMatching_3_UIDArea(log, gi_SplitArea[2], gi_Area_UID);
            }())
            {
                //foo_test_show_UIDArea();
                //TODO: 分图之后的识别
                //_cv_get_split_Rect_.notify_all();
                //log->Log("split_Rect.notify_one() TODO:");
    
                _cv_matching_uid.notify_all();
            }
            else
            {
                //_cv_get_splitarea.notify_all();
                log->info("back splitarea.notify_all() form Get_GI_SplitMatching_3_UIDArea");
            }
        }
    }
}
//进入获取分发4——矩形区域的识别区域线程
void BaiLan::get_GI_Split_4_LeftGetItemsArea_server() {
    log->info("进入获取分发4——左侧获取物品区域线程");
    while (_isRunService) {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_SplitArea);
        _cv_get_split_matching.wait(uniqueLock);
        if (_isStartService ) {
            if ( [&]()->bool{
                std::unique_lock<std::mutex> lock_output(_mutex_GI_Split_4_LeftGetItemsArea);
                return Get_GI_SplitMatching_4_LeftGetItemsArea(log, gi_SplitArea[3], gi_Area_LeftGetItems);
            }())
            {
                foo_test_show_LeftGetItemsArea();
                //TODO: 分图之后的识别
                //_cv_get_split_Rect_.notify_all();
                //log->Log("split_Rect.notify_one() TODO:");
            }
            else
            {
                //_cv_get_splitarea.notify_all();
                log->info("back splitarea.notify_all() form Get_GI_SplitMatching_4_LeftGetItemsArea");
            }
        }
    }
}
//进入获取分发5——右侧获取物品区域线程
void BaiLan::get_GI_Split_5_RightGetItemsArea_server() {
    log->info("进入获取分发5——右侧获取物品区域线程");
    while (_isRunService) {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_SplitArea);
        _cv_get_split_matching.wait(uniqueLock);
        if (_isStartService) {
            if([&]()->bool{
                    std::unique_lock<std::mutex> lock_output(_mutex_GI_Split_5_RightGetItemsArea);
                return Get_GI_SplitMatching_5_RightGetItemsArea(log, gi_SplitArea[4], gi_Area_RightGetItems);
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
                log->info("back splitarea.notify_all() form Get_GI_SplitMatching_5_RightGetItemsArea");
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
        log->error("get_GI_Split_2_MiniMapArea_server error:" + string(e.what()));
    }
    vector<Mat> list;
    split(gi_SplitArea[1], list);
    imshow("MiniMapArea", list[list.size()-1]);
    waitKey(1);
}
void BaiLan::foo_test_show_UIDArea() {
    vector<Mat> list;
    split(gi_SplitArea[2], list);
    imshow("UIDArea", list[list.size()-1]);
    waitKey(1);
}
void BaiLan::foo_test_show_LeftGetItemsArea() {
    vector<Mat> list;
    split(gi_SplitArea[3], list);
    imshow("LeftGetItemsArea", list[list.size()-1]);
    waitKey(1);
}
void BaiLan::foo_test_show_RightGetItemsArea() {
    vector<Mat> list;
    split(gi_SplitArea[4], list);
    imshow("RightGetItemsArea", list[list.size()-1]);
    //	SetWindowPos(thisHandle, HWND_TOP, GIS.giRect.left + offGiMinMap.x, GIS.giRect.top + offGiMinMap.y, 0, 0, SWP_NOSIZE);
    waitKey(1);
}

//进入识别分发1——派蒙的具体区域线程
void BaiLan::get_GI_Matching_1_Paimon_server() {
    log->info("进入识别分发1——派蒙的具体区域线程");
    while (_isRunService) {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_Split_1_PaimonArea);
        _cv_matching_paimon.wait(uniqueLock);
        if (_isStartService) {
            if( [&]()->bool{
                std::unique_lock<std::mutex> lock_output(_mutex_GI_Matching_1_Paimon);
                return Get_GI_Matching_1_Paimon(log, gi_Area_Paimon, gi_MatchOutput_Paimon);
            }())
            {
                //TODO:输出识别到的变量
                isShowPaimon=gi_MatchOutput_Paimon;
                //log->info("识别到的结果: 派蒙是否存在 " + std::to_string(gi_MatchOutput_Paimon));
                //foo_test_show_PaimonArea();
            }
            else
            {
                //_cv_get_splitarea.notify_all();
                isShowPaimon=false;
                //log->info("识别到的结果: 派蒙是否存在 0");
            }
        }
    }
}
//TODO: 转换为SplitMiniMap
//进入识别分发2——小地图的具体区域线程
void BaiLan::get_GI_Matching_2_MiniMap_Server() {
    log->info("进入识别分发2——小地图的具体区域线程");
    /*
    while(_isRunService)
    {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_Split_2_MiniMapArea);
        _cv_matching_minimap.wait(uniqueLock);
        if (_isStartService)
        {
            if([&]()->bool{
               std::unique_lock<std::mutex> lock_output(_mutex_GI_Matching_2_MiniMap) ;
               return Get_GI_Matching_2_MiniMap(log,gi_Area_MiniMap,gi_MatchOutput_MiniMap);
            }())
            {
                //TODO: 02.26-03:00 输出结果到缓存变量
            }
            else
            {
                //TODO: 02.26-03:00 输出结果到缓存变量
            }
        }
    }
    */
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

//进入识别分发3——UID的具体区域线程
void BaiLan::get_GI_Matching_3_UID_Server() {
    log->info("进入识别分发3——UID的具体区域线程");
    while(_isRunService){
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_Split_3_UIDArea);
        _cv_matching_uid.wait(uniqueLock);
        if(_isStartService){
            if([&]()->bool{
                std::unique_lock<std::mutex> lock_output(_mutex_GI_Matching_3_UID);
                return Get_GI_Matching_3_UID(log, gi_Area_UID,gi_MatchOutput_UID);
            }())
            {
                _uid.uid=gi_MatchOutput_UID;
                _uid.result= true;
            }
            else
            {
                _uid.result= false;
            }
        }
    }
}
//进入识别分发4——侧已获得物品列表的具体区域线程
void BaiLan::get_GI_Matching_4_LeftGetItems_Server() {
    log->info("进入识别分发4——侧已获得物品列表的具体区域线程");
    while (_isRunService){
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_Split_4_LeftGetItemsArea);
        _cv_matching_leftgetitems.wait(uniqueLock);
        if(_isStartService){
            if([&]()->bool{
                std::unique_lock<std::mutex> lock_output(_mutex_Matching_4_LeftGetItems);
                return Get_GI_Matching_4_LeftGetItems(log, gi_Area_LeftGetItems,gi_MatchOutput_LeftGetItems);
            }())
            {
                // strcpy_s(_starJson.jsonBuff,255,gi_MatchOutput_LeftGetItems.c_str());
                // _starJson.result= true;
            }
            else
            {
                //_starJson.result= false;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}
//进入识别分发5——右侧已获得物品列表的具体区域线程
void BaiLan::get_GI_Matching_5_RightGetItems_Server() {
    log->info("进入识别分发5——右侧已获得物品列表的具体区域线程");
    while (_isRunService){
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_Split_5_RightGetItemsArea);
        _cv_matching_leftgetitems.wait(uniqueLock);
        if(_isStartService){
            if([&]()->bool{
                std::unique_lock<std::mutex> lock_output(_mutex_Matching_5_RightGetItems);
                return Get_GI_Matching_5_RightGetItems(log, gi_Area_LeftGetItems,gi_MatchOutput_RightGetItems);
            }())
            {
                // strcpy_s(_starJson.jsonBuff,255,gi_MatchOutput_LeftGetItems.c_str());
                // _starJson.result= true;
            }
            else
            {
                //_starJson.result= false;
            }
            
            //等待1毫秒
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}
//-----------------------------------------------------------------------------------
//二级分发

//进入二级分发1——角色位置的计算
void BaiLan::get_GI_II_1_Position_Server() {
    log->info("进入二级分发1——角色位置的计算");
    while (_isRunService){
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_Split_2_MiniMapArea);
        _cv_matching_minimap.wait(uniqueLock);
        if(_isStartService){
            if([&]()->bool{
                std::unique_lock<std::mutex> lock_output(_mutex_II_1_Position);
                return Get_GI_II_1_Position(log, gi_Area_MiniMap,gi_II_Output_1_Position);
            }())
            {
                _position.x = gi_II_Output_1_Position.x;
                _position.y = gi_II_Output_1_Position.y;
                _position.result = true;
            }
            else
            {
                _position.result = false;
            }
        }
    }
}

//进入二级分发2——角色角度的计算
void BaiLan::get_GI_II_2_Direction_Server() {
    log->info("进入二级分发2——角色角度的计算");
    while (_isRunService){
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_Split_2_MiniMapArea);
        _cv_matching_minimap.wait(uniqueLock);
        if(_isStartService){
            if([&]()->bool{
                std::unique_lock<std::mutex> lock_output(_mutex_II_2_Direction);
                return Get_GI_II_2_Direction(log, gi_Area_MiniMap,gi_II_Output_2_Direction);
            }())
            {
                _direction.a=gi_II_Output_2_Direction;
                _direction.result = true;
            }
            else
            {
                _direction.result = false;
            }
        }
    }
}

//进入二级分发3——角色角度2的计算
void BaiLan::get_GI_II_3_Rotation_Server() {
    log->info("进入二级分发3——角色角度2的计算");
    while (_isRunService){
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_Split_2_MiniMapArea);
        _cv_matching_minimap.wait(uniqueLock);
        if(_isStartService){
            if([&]()->bool{
                std::unique_lock<std::mutex> lock_output(_mutex_II_3_Rotation);
                return Get_GI_II_3_Rotation(log, gi_Area_MiniMap,gi_II_Output_3_Rotation);
            }())
            {
                _rotation.a2=gi_II_Output_3_Rotation;
                _rotation.result = true;
            }
            else
            {
                _rotation.result = false;
            }
        }
    }
}

//进入二级分发4——神瞳的计算
void BaiLan::get_II_4_StarJson_Server() {
    log->info("进入二级分发4——神瞳的计算");
    while (_isRunService){
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_Split_2_MiniMapArea);
        _cv_matching_minimap.wait(uniqueLock);
        if(_isStartService){
            if([&]()->bool{
                std::unique_lock<std::mutex> lock_output(_mutex_II_4_StarJson);
                return Get_GI_II_4_StarJson(log, gi_Area_MiniMap,gi_II_Output_4_StarJson);
            }())
            {
                strcpy_s(_starJson.jsonBuff,255,gi_II_Output_4_StarJson.c_str());
                _starJson.result= true;
            }
            else
            {
                _starJson.result= false;
            }
        }
    }
}
