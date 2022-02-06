//
// Created by GengG on 2022/1/30.
//

#include "BaiLan.h"
// �����׼�����־
#include "../Logger/StandardOutputLogger.h"
//static Logger logger = std::make_shared<StandardOutputLogger>();

//���������صĺ���
#include "BaiLan_Util.h"

BaiLan::BaiLan() {
    log = std::make_shared<StandardOutputLogger>();

    // ��ʼ������
    log->Log("��ʼ����... ...");
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
    log->Log("�������߳�ɾ���ɹ�");
    log->Log("��������... ...");
}

bool BaiLan::startService() {
    log->Log("��������");
    _isStartService = true;
    return true;
}

bool BaiLan::stopService() {
    log->Log("ֹͣ����");
    _isStartService = false;
    return true;
}

bool BaiLan::GetDirection(double &a) {
    if(_direction.result)
    {
        a = _direction.a;
    }
    log->Log("���: "+std::to_string(_direction.result)+"  ��ȡ�Ƕ�: "+std::to_string(_direction.a));
    return _direction.result;
}

bool BaiLan::GetRotation(double &a2) {
    if(_rotation.result)
    {
        a2 = _rotation.a2;
    }
    log->Log("���: "+ std::to_string(_rotation.result) +"  ��ȡ�Ƕ�: "+std::to_string(a2));
    return _rotation.result;
}

bool BaiLan::GetStarJson(char *string) {
    return false;
}

bool BaiLan::GetLastError() {
    return false;
}

void BaiLan::service() {
    log->Log("����������߳�");
    // ����ѭ���Ƿ����
    while (_isRunService) {
        std::unique_lock<std::mutex> unlck_get_signal(_mutex_get_signal);
        // �ȴ��ź�, ���û���ź�, ��ȴ�30��
        _cv_get_signal.wait_for(unlck_get_signal, std::chrono::seconds(1), [&]()->bool{return false;});

        // �Ƿ�ִ�����������ִ������, ������ȴ��ź�
        if (_isStartService) {
            // ������ȡԭ����
            //log->Log("������ȡԭ����");
            _cv_get_handle.notify_all();
            // ���ʹ�����ȡԭ����������һ��ʱ�䣬��ֹ��ȡʧ�ܵ�ʱ��ѭ������Ƶ�ʹ��ߡ�
            // ��������ʱ33����
            std::this_thread::sleep_for(std::chrono::milliseconds(33));
            //std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
    }
    log->Log("�����������߳�");
}

void BaiLan::createTasks() {
}

void BaiLan::beginTasks() {
    //TODO: ����������Ҫ��Ӵ����λ��

    //���������ӷ����߳�
    _t_get_GI_Handle = new std::thread(&BaiLan::get_GI_Handle_server, this);
    _t_get_GI_Frame = new std::thread(&BaiLan::get_GI_Frame_server, this);
    _t_get_GI_SplitArea = new std::thread(&BaiLan::get_GI_SplitArea_server, this);
    
    _t_get_GI_Split_1_PaimonArea = new std::thread(&BaiLan::get_GI_Split_1_PaimonArea_server, this);
    _t_get_GI_Split_2_MiniMapArea = new std::thread(&BaiLan::get_GI_Split_2_MiniMapArea_server, this);
    _t_get_GI_Split_3_UIDArea = new std::thread(&BaiLan::get_GI_Split_3_UIDArea_server, this);
    _t_get_GI_Split_4_LeftGetItemsArea = new std::thread(&BaiLan::get_GI_Split_4_LeftGetItemsArea_server, this);
    _t_get_GI_Split_5_RightGetItemsArea = new std::thread(&BaiLan::get_GI_Split_5_RightGetItemsArea_server, this);

    //����ʶ�𲿷�
    _t_get_GI_Matching_1_Paimon = new std::thread(&BaiLan::get_GI_Matching_1_Paimon_server,this);
    _t_get_GI_Matching_2_MiniMap = new std::thread(&BaiLan::get_GI_Matching_2_MiniMap_Server,this);
}

void BaiLan::endTasks() {
    //TODO: ����������Ҫ��Ӵ����λ��

    _cv_get_handle.notify_all();
    _cv_get_frame.notify_all();
    _cv_get_splitarea.notify_all();
    _cv_get_split_matching.notify_all();

    // ��ȡ���д�����������
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
    
    // join �����߳�
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
    
    //����ʶ�𲿷�
    if(_t_get_GI_Matching_1_Paimon!=nullptr){
        _t_get_GI_Matching_1_Paimon->join();
    }
    if(_t_get_GI_Matching_2_MiniMap!= nullptr){
        _t_get_GI_Matching_2_MiniMap->join();
    }
    
}

//�����ȡԭ�����߳�
void BaiLan::get_GI_Handle_server() {
    log->Log("�����ȡԭ�����߳�");
    while (_isRunService) {
        std:unique_lock<std::mutex> unlck_get_handle(_mutex_GI_Handle);
        // �ȴ��ź�, ���û���ź�, ��ȴ�1��
        _cv_get_handle.wait_for(unlck_get_handle, 1s, []()->bool {return false;});
        if (_isStartService)// �Ƿ���
        {
            if( [&] () ->bool{return Get_GI_Handle(gi_Handle);}())
            {//��ȡԭ�������ɹ�
                
                //������ȡԭ���ͼ
                //log->Log("��þ����������ȡԭ���ͼ");
                _cv_get_frame.notify_all();
                //���á�һֱ��ͼ����־λΪ��
                _is_always_get_GI_Frame=true;
            }
            else
            {//��ȡԭ������ʧ��
                
                //��ȡʧ�ܺ���Ҫ����������һ��
                //������ȡԭ����
                log->Info("��ȡԭ������ʧ�ܣ�������ȡԭ����");
                //_cv_get_handle.notify_all();
            }
        }
    }
}
//�����ȡԭ���ͼ�߳�
void BaiLan::get_GI_Frame_server() {
    log->Info("�����ȡԭ���ͼ�߳�");
    while (_isRunService)
    {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_Handle);
        // �ȴ��ź�, ���û���ź�, ��ȴ�1���롣
        // ��һֱ��ͼ����־λΪ���һֱִ�У����������ִ����һ��
        //TODO: ע�������߽�ͼ�ٶ�
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
                // ������һ����֪ͨ��ַַ�
                //log->Log("��ý�ͼ��������һ����֪ͨ��ַַ�");
                _cv_get_splitarea.notify_all();
    
                // ����Ӧ��ͼƵ�ʵ� 30 fps
                // ����ȴ�ʱ��
                // t= (static_cast<double>(cv::getTickCount())-t)/cv::getTickFrequency();
                // if(t<0.033)
                // {
                //     int need_wait_ms=(0.033-t>0?0.033-t:0)*1000;
                //     std::this_thread::sleep_for(std::chrono::milliseconds(need_wait_ms));
                //     log->Log("need_wait_ms: "+std::to_string(static_cast<int>(need_wait_ms)));
                // }
                //std::this_thread::sleep_for(33ms);
            }
            else
            {
                // ��ͼʧ�ܣ����»�ȡ������ȴ�100����
                _cv_get_handle.notify_all();
                log->Info("��ͼʧ�ܣ����»�ȡ���");
                // �ȴ�100����
                std::this_thread::sleep_for(1ms);
                break;
            }
            
            //log->Log("fps: "+std::to_string(static_cast<int>(1.0/t)));
        }
    }
}
//�����ȡ��ͼ����߳�
void BaiLan::get_GI_SplitArea_server() {
    log->Info("�����ȡ��ͼ����߳�");
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
//�����ȡ�ַ�1�������ɵ�ʶ�������߳�
void BaiLan::get_GI_Split_1_PaimonArea_server() {
    log->Info("�����ȡ�ַ�1�������ɵ�ʶ�������߳�");
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
                //TODO: ��ͼ֮���ʶ��
                _cv_matching_paimon.notify_all();
            }
            else
            {
                //_cv_get_splitarea.notify_all();
                log->Info("back splitarea.notify_all() form Get_GI_SplitMatching_1_PaimonArea");
            }
        }
    }
}
//�����ȡ�ַ�2����С��ͼ��ʶ�������߳�
void BaiLan::get_GI_Split_2_MiniMapArea_server() {
    log->Info("�����ȡ�ַ�2����С��ͼ��ʶ�������߳�");
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
               //TODO: ��ͼ֮���ʶ��
               _cv_matching_minimap.notify_all();
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
//�����ȡ�ַ�3����UID��ʶ�������߳�
void BaiLan::get_GI_Split_3_UIDArea_server() {
    log->Info("�����ȡ�ַ�3����UID��ʶ�������߳�");
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
                //TODO: ��ͼ֮���ʶ��
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
//�����ȡ�ַ�4�������������ʶ�������߳�
void BaiLan::get_GI_Split_4_LeftGetItemsArea_server() {
    log->Info("�����ȡ�ַ�4��������ȡ��Ʒ�����߳�");
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
                //TODO: ��ͼ֮���ʶ��
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
//�����ȡ�ַ�5�����Ҳ��ȡ��Ʒ�����߳�
void BaiLan::get_GI_Split_5_RightGetItemsArea_server() {
    log->Info("�����ȡ�ַ�5�����Ҳ��ȡ��Ʒ�����߳�");
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
                //TODO: ��ͼ֮���ʶ��
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

void BaiLan::foo_test_show_Frame() const {
    Mat         gray_paimonArea;
    vector<Mat> list;
    // static_cast<int>(giFrame.cols / 5.0*3.0),
// static_cast<int>(giFrame.rows / 4.0),
// static_cast<int>(giFrame.cols / 6.0),
// static_cast<int>(giFrame.rows / 2.0)
//���β���
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
void BaiLan::foo_test_show_PaimonArea() {// ����֡��
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
        /// double minDist = 10;  //����Բ��֮�����С����
        /// double  param1 = 100;  //Canny��Ե���Ľϴ���ֵ
        /// double  param2 = 100;  //�ۼ�����ֵ
        /// int min_radius = 20;  //Բ�ΰ뾶����Сֵ
        /// int max_radius = 100;  //Բ�ΰ뾶�����ֵ
        /// HoughCircles(abs_dst, circles, HOUGH_GRADIENT, dp, minDist, param1, param2,
        ///              min_radius, max_radius);
        /// //�Ҷ�ͼ��Ϊ��ɫ
        /// cvtColor(abs_dst,abs_dst,cv::COLOR_GRAY2BGR);
///
        /// std::sort( circles.begin(), circles.end(),[](const Vec3f& a,const Vec3f& b)->bool{return a[2]>b[2];});
        /// //ͼ���б�ǳ�Բ��
        /// for (size_t i = 0; i < circles.size()>1?1:0; i++)
        /// {
        ///     //��ȡԲ��
        ///     Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        ///     //��ȡ�뾶
        ///     int radius = cvRound(circles[i][2]);
        ///     //����Բ��
        ///     circle(abs_dst, center, 3, Scalar(0, 255, 0), -1, 8, 0);
        ///     //����Բ
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

//����ʶ��ַ�1�������ɵľ��������߳�
void BaiLan::get_GI_Matching_1_Paimon_server() {
    log->Info("����ʶ��ַ�1�������ɵľ��������߳�");
    while (_isRunService) {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_Split_1_PaimonArea);
        _cv_matching_paimon.wait(uniqueLock);
        if (_isStartService) {
            if( [&]()->bool{
                std::unique_lock<std::mutex> lock_output(_mutex_GI_Matching_1_Paimon);
                return Get_GI_Matching_1_Paimon(gi_Area_Paimon, gi_MatchOutput_Paimon);
            }())
            {
                //TODO:���ʶ�𵽵ı���
                isShowPaimon=gi_MatchOutput_Paimon;
                log->Info("ʶ�𵽵Ľ��: �����Ƿ���� "+std::to_string(gi_MatchOutput_Paimon));
                //foo_test_show_PaimonArea();
            }
            else
            {
                //_cv_get_splitarea.notify_all();
                isShowPaimon=false;
                log->Info("ʶ�𵽵Ľ��: �����Ƿ���� 0");
            }
        }
    }
}
//TODO: ת��Ϊ
//����ʶ��ַ�2����С��ͼ�ľ��������߳�
void BaiLan::get_GI_Matching_2_MiniMap_Server() {
    log->Info("����ʶ��ַ�2����С��ͼ�ľ��������߳�");
    while(_isRunService)
    {
        std::unique_lock<std::mutex> uniqueLock(_mutex_GI_Split_2_MiniMapArea);
        _cv_matching_minimap.wait(uniqueLock);
        if (_isStartService)
        {
            if([&]()->bool{
               std::unique_lock<std::mutex> lock_output(_mutex_GI_Matching_2_MiniMap) ;
               return Get_GI_Matching_2_MiniMap(gi_Area_MiniMap,gi_MatchOutput_MiniMap);
            }())
            {
                //TODO: 02.26-03:00 ���������������
            }
            else
            {
                //TODO: 02.26-03:00 ���������������
            }
        }
    }
}



