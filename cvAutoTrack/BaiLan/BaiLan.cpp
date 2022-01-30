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

void BaiLan::service() {
    log->Log("����������߳�");
    // ����ѭ���Ƿ����
    while (_isRunService) {
        // �Ƿ�ִ������
        if (_isStartService) {
            // //����֡��
            // static int64_t _lastTime=0;
            // int64_t now=cv::getTickCount();

            // ��ȡԭ����
            {
                // ������ȡԭ����
                if (_is_need_update_giHandle) {
                    //�����µ��ź�
                    _cv_get_handle.notify_one();
                    _is_need_update_giHandle = false;
                }

                //��ȡԭ��������
                if (_result_giHandle) {
                    _is_need_update_giHandle = false;

                    // ������ȡԭ���ͼ
                    _is_need_update_giFrame = true;
                } else {
                    _is_need_update_giHandle = true;
                    //�ȴ� 3s ���ٴγ���
                    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
                }
            }

            // ��ȡԭ���ͼ
            {
                // �Ƿ񴥷���ȡԭ���ͼ
                if (_is_need_update_giFrame) {
                    _gi_getFrame_signal = true;
                }

                //�Ƿ������־��������ԭ���ͼ
                static bool IsOutputInfo_giFrame = true;

                // ��ȡԭ���沿��
                if (_result_giFrame) {
                    _gi_getFrame_signal = false;
                    cv::imshow("ԭ��", gi_Frame);
                    // ���������ַַ�
                    _is_need_update_giSplitArea = true;
                } else {
                    _is_need_update_giFrame = true;
                }
            }

            // ��ȡԭ����������
            {
                // �Ƿ񴥷���ȡ�����ַַ�
                if (_is_need_update_giSplitArea) {
                    _gi_getSplitArea_signal = true;
                }

                //�Ƿ������־�������ڻ����ַַ�
                static bool IsOutputInfo_giSplitArea = true;

                //��ȡ�����ַַ�����
                if (_result_giSplitArea) {
                    _gi_getSplitArea_signal = false;

                    try {
                        cv::imshow("���ɿ���������", gi_SplitArea[0].clone());
                        cv::imshow("С��ͼ����������", gi_SplitArea[1].clone());
                        cv::imshow("UID����������", gi_SplitArea[2].clone());
                        cv::imshow("����ѻ�ȡ��Ʒ����������", gi_SplitArea[3].clone());
                        cv::imshow("�Ҳ�ɼ�ȡ��Ʒ����������", gi_SplitArea[4].clone());
                    }
                    catch (exception &e) {
                        //����쳣��Ϣ�ַ�������־
                        log->Warning("SplitArea :" + std::string(e.what()));
                    }

                    // ������ȡ�ַ�ָ��
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

            //��ȡ�ַ�1��������ʶ������
            {
                // �Ƿ񴥷���ȡ�ַ�1��������ʶ������
                if (_is_need_update_giSplit_1_PaimonArea) {
                    _gi_getSplit_1_PaimonArea_signal = true;
                }

                //�Ƿ������־�������ڷַ�1��������ʶ������
                static bool IsOutputInfo_giSplit_1_PaimonArea = true;

                //��ȡ�ַ�1��������ʶ�����򲿷�
                if (_result_giSplit_1_PaimonArea) {
                    _gi_getSplit_1_PaimonArea_signal = false;

                    log->Info("gi_Area_Paimon:" + std::to_string(gi_Area_Paimon.width) + "," +
                              std::to_string(gi_Area_Paimon.width));
                    // ������ȡ����ʶ����
                    // _is_need_update_Matching_2_Paimon = true;
                } else {
                    //_is_need_update_giSplit_1_PaimonArea = true;
                }
            }
            //��ȡ�ַ�2����С��ͼʶ������
            {
                // �Ƿ񴥷���ȡ�ַ�2����С��ͼʶ������
                if (_is_need_update_giSplit_2_MiniMapArea) {
                    _gi_getSplit_2_MiniMapArea_signal = true;
                }

                //�Ƿ������־�������ڷַ�2����С��ͼʶ������
                static bool IsOutputInfo_giSplit_2_MiniMapArea = true;

                //��ȡ�ַ�2����С��ͼʶ�����򲿷�
                if (_result_giSplit_2_MiniMapArea) {
                    _gi_getSplit_2_MiniMapArea_signal = false;
                    // ������ȡС��ͼʶ������
                    // _is_need_update_Matching_2_MiniMap = true;
                } else {
                    //_is_need_update_giSplit_2_MiniMapArea = true;
                }
            }
            //��ȡ�ַ�3����UIDʶ����
            {
                // �Ƿ񴥷���ȡ�ַ�3����UIDʶ������
                if (_is_need_update_giSplit_3_UIDArea) {
                    _gi_getSplit_3_UIDArea_signal = true;
                }

                //�Ƿ������־�������ڷַ�3����UIDʶ������
                static bool IsOutputInfo_giSplit_3_UIDArea = true;

                //��ȡ�ַ�3����UIDʶ�����򲿷�
                if (_result_giSplit_3_UIDArea) {
                    _gi_getSplit_3_UIDArea_signal = false;
                    // ������ȡUIDʶ������
                    // _is_need_update_Matching_2_UID = true;
                } else {
                    //_is_need_update_giSplit_3_UIDArea = true;
                }
            }
            //��ȡ�ַ�4��������ѻ�ȡ��Ʒ��ʶ������
            {
                // �Ƿ񴥷���ȡ�ַ�4��������ѻ�ȡ��Ʒ��ʶ������
                if (_is_need_update_giSplit_4_LeftGetItemsArea) {
                    _gi_getSplit_4_LeftGetItemsArea_signal = true;
                }

                //�Ƿ������־�������ڷַ�4��������ѻ�ȡ��Ʒ��ʶ������
                static bool IsOutputInfo_giSplit_4_LeftGetItemsArea = true;

                //��ȡ�ַ�4��������ѻ�ȡ��Ʒ��ʶ������
                if (_result_giSplit_4_LeftGetItemsArea) {
                    _gi_getSplit_4_LeftGetItemsArea_signal = false;
                    if (IsOutputInfo_giSplit_4_LeftGetItemsArea)log->Info("����gi_Split_4_LeftGetItemsArea�ɹ�");
                    IsOutputInfo_giSplit_4_LeftGetItemsArea = false;

                    // ������ȡ����ѻ�ȡ��Ʒ��ʶ������
                    // _is_need_update_Matching_2_LeftGetItems = true;
                } else {
                    //_is_need_update_giSplit_4_LeftGetItemsArea = true;
                    //if(IsOutputInfo_giSplit_4_LeftGetItemsArea)log->Info("����gi_Split_4_LeftGetItemsAreaʧ�ܣ��ٴγ���");
                    IsOutputInfo_giSplit_4_LeftGetItemsArea = true;
                }
            }
            //��ȡ�ַ�5�����Ҳ��Ѽ�ȡ��Ʒ��ʶ������
            {
                // �Ƿ񴥷���ȡ�ַ�5�����Ҳ��Ѽ�ȡ��Ʒ��ʶ������
                if (_is_need_update_giSplit_5_RightGetItemsArea) {
                    _gi_getSplit_5_RightGetItemsArea_signal = true;
                }

                //�Ƿ������־�������ڷַ�5�����Ҳ��Ѽ�ȡ��Ʒ��ʶ������
                static bool IsOutputInfo_giSplit_5_RightGetItemsArea = true;

                //��ȡ�ַ�5�����Ҳ��Ѽ�ȡ��Ʒ��ʶ������
                if (_result_giSplit_5_RightGetItemsArea) {
                    _gi_getSplit_5_RightGetItemsArea_signal = false;
                    if (IsOutputInfo_giSplit_5_RightGetItemsArea)log->Info("����gi_Split_5_RightGetItemsArea�ɹ�");
                    IsOutputInfo_giSplit_5_RightGetItemsArea = false;

                    // ������ȡ�Ҳ��Ѽ�ȡ��Ʒ��ʶ������
                    // _is_need_update_Matching_2_LeftGetItems = true;
                } else {
                    //_is_need_update_giSplit_5_RightGetItemsArea = true;
                    //if(IsOutputInfo_giSplit_5_RightGetItemsArea)log->Info("����gi_Split_5_RightGetItemsAreaʧ�ܣ��ٴγ���");
                    IsOutputInfo_giSplit_5_RightGetItemsArea = true;
                }
            }


            cv::waitKey(3);
        } else {

            //TODO: �߳�����
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
    log->Log("�����������߳�");
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


}

void BaiLan::endTasks() {
    //TODO: ����������Ҫ��Ӵ����λ��

    // ��ȡ���д�����������
    std::lock_guard<std::mutex> lock_output_giHandle(_mutex_GI_Handle);
    std::lock_guard<std::mutex> lock_output_giFrame(_mutex_GI_Frame);
    std::lock_guard<std::mutex> lock_output_splitArea(_mutex_GI_SplitArea);
    std::lock_guard<std::mutex> lock_output_split_1_paimonArea(_mutex_GI_Split_1_PaimonArea);
    std::lock_guard<std::mutex> lock_output_split_2_miniMapArea(_mutex_GI_Split_2_MiniMapArea);
    std::lock_guard<std::mutex> lock_output_split_3_UIDArea(_mutex_GI_Split_3_UIDArea);
    std::lock_guard<std::mutex> lock_output_split_4_leftGetItemsArea(_mutex_GI_Split_4_LeftGetItemsArea);
    std::lock_guard<std::mutex> lock_output_split_5_rightGetItemsArea(_mutex_GI_Split_5_RightGetItemsArea);

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
}


void BaiLan::get_GI_Handle_server() {
    log->Info("��ȡԭ�����߳�����");


    while (_isRunService) {
        std:unique_lock<std::mutex> uniqueLock(_mutex_GI_Handle);
        if (_isStartService)// �Ƿ���
        {
            _result_giHandle = get_GI_Handle(gi_Handle);
        }
        //�����ź�
        _cv_get_handle.wait(uniqueLock);
    }
}

bool BaiLan::get_GI_Handle(HWND &giHandle) {
    _result_giHandle = false;
    return Get_GI_Handle(giHandle);
}

void BaiLan::get_GI_Frame_server() {
    log->Info("��ȡԭ���ͼ�߳�����");
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
    log->Info("��ȡ��ͼ����߳�����");
    while (_isRunService) {
        if (_isStartService && _gi_getSplitArea_signal) {
            std::lock_guard<std::mutex> lock_input(_mutex_GI_Frame);
            std::lock_guard<std::mutex> lock_output(_mutex_GI_SplitArea);

            _result_giSplitArea = get_GI_SplitArea(gi_Frame, gi_SplitArea);
            _gi_getSplitArea_signal = false;
        } else {
            //TODO: �߳�����
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

bool BaiLan::get_GI_SplitArea(Mat &giFrame, vector<cv::Mat> &giSplitArea) {
    _result_giSplitArea = false;
    return Get_GI_SplitArea(giFrame, giSplitArea);
}

void BaiLan::get_GI_Split_1_PaimonArea_server() {
    log->Info("��ȡ�ַ�1�������ɵ�ʶ�������߳�����");
    while (_isRunService) {
        if (_isStartService && _gi_getSplit_1_PaimonArea_signal) {
            std::lock_guard<std::mutex> lock_input(_mutex_GI_SplitArea);
            std::lock_guard<std::mutex> lock_output(_mutex_GI_Split_1_PaimonArea);

            _result_giSplit_1_PaimonArea = get_GI_Split_1_PaimonArea(gi_SplitArea[0], gi_Area_Paimon);
            _gi_getSplit_1_PaimonArea_signal = false;

        } else {
            //TODO: �߳�����
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

bool BaiLan::get_GI_Split_1_PaimonArea(Mat &giSplitArea, Rect &giArea_Paimon) {
    _result_giSplit_1_PaimonArea = false;
    return Get_GI_SplitMatching_1_PaimonArea(giSplitArea, giArea_Paimon);
}

void BaiLan::get_GI_Split_2_MiniMapArea_server() {
    log->Info("��ȡ�ַ�2����С��ͼ��ʶ�������߳�����");
    while (_isRunService) {
        if (_isStartService && _gi_getSplit_2_MiniMapArea_signal) {
            std::lock_guard<std::mutex> lock_input(_mutex_GI_SplitArea);
            std::lock_guard<std::mutex> lock_output(_mutex_GI_Split_2_MiniMapArea);

            _result_giSplit_2_MiniMapArea = get_GI_Split_2_MiniMapArea(gi_SplitArea[1], gi_Area_MiniMap);
            _gi_getSplit_2_MiniMapArea_signal = false;
        } else {
            //TODO: �߳�����
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

}

bool BaiLan::get_GI_Split_2_MiniMapArea(Mat &giSplitArea_MiniMap, Rect &giArea_MiniMap) {
    _result_giSplit_2_MiniMapArea = false;
    return Get_GI_SplitMatching_2_MiniMapArea(giSplitArea_MiniMap, giArea_MiniMap);
}

void BaiLan::get_GI_Split_3_UIDArea_server() {
    log->Info("��ȡ�ַ�3����UID��ʶ�������߳�����");
    while (_isRunService) {
        if (_isStartService && _gi_getSplit_3_UIDArea_signal) {
            std::lock_guard<std::mutex> lock_input(_mutex_GI_SplitArea);
            std::lock_guard<std::mutex> lock_output(_mutex_GI_Split_3_UIDArea);

            _result_giSplit_3_UIDArea = get_GI_Split_3_UIDArea(gi_SplitArea[2], gi_Area_UID);
            _gi_getSplit_3_UIDArea_signal = false;

        } else {
            //TODO: �߳�����
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

bool BaiLan::get_GI_Split_3_UIDArea(Mat &giSplitArea_UID, Rect &giArea_UID) {
    _result_giSplit_3_UIDArea = false;
    return Get_GI_SplitMatching_3_UIDArea(giSplitArea_UID, giArea_UID);
}

void BaiLan::get_GI_Split_4_LeftGetItemsArea_server() {
    log->Info("��ȡ�ַ�4��������ȡ��Ʒ�����߳�����");
    while (_isRunService) {
        if (_isStartService && _gi_getSplit_4_LeftGetItemsArea_signal) {
            std::lock_guard<std::mutex> lock_input(_mutex_GI_SplitArea);
            std::lock_guard<std::mutex> lock_output(_mutex_GI_Split_4_LeftGetItemsArea);

            _result_giSplit_4_LeftGetItemsArea = get_GI_Split_4_LeftGetItemsArea(gi_SplitArea[3], gi_Area_LeftGetItems);
            _gi_getSplit_4_LeftGetItemsArea_signal = false;
        } else {
            //TODO: �߳�����
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

bool BaiLan::get_GI_Split_4_LeftGetItemsArea(Mat &giSplitArea_LeftGetItems, Rect &giArea_LeftGetItems) {
    _result_giSplit_4_LeftGetItemsArea = false;
    return Get_GI_SplitMatching_4_LeftGetItemsArea(giSplitArea_LeftGetItems, giArea_LeftGetItems);
}

void BaiLan::get_GI_Split_5_RightGetItemsArea_server() {
    log->Info("��ȡ�ַ�5�����Ҳ��ȡ��Ʒ�����߳�����");
    while (_isRunService) {
        if (_isStartService && _gi_getSplit_5_RightGetItemsArea_signal) {
            std::lock_guard<std::mutex> lock_input(_mutex_GI_SplitArea);
            std::lock_guard<std::mutex> lock_output(_mutex_GI_Split_5_RightGetItemsArea);

            _result_giSplit_5_RightGetItemsArea = get_GI_Split_5_RightGetItemsArea(gi_SplitArea[4],
                                                                                   gi_Area_RightGetItems);
            _gi_getSplit_5_RightGetItemsArea_signal = false;
        } else {
            //TODO: �߳�����
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

bool BaiLan::get_GI_Split_5_RightGetItemsArea(Mat &giSplitArea_RightGetItems, Rect &giArea_RightGetItems) {
    _result_giSplit_5_RightGetItemsArea = false;
    return Get_GI_SplitMatching_5_RightGetItemsArea(giSplitArea_RightGetItems, giArea_RightGetItems);
}


