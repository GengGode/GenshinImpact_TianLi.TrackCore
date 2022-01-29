//
// Created by GengG on 2022/1/28.
//

#include "Screenshot.h"
Screenshot::Screenshot(const Logger& logService)
{
    log=logService;
    log->Log("��ͼģ�����");
}

Screenshot::~Screenshot() {
    std::lock_guard<std::mutex> lock_input(lm_input);
    std::lock_guard<std::mutex> lock_output(lm_output);
}

void Screenshot::work()
{
    std::lock_guard<std::mutex> lock_input(lm_input);
    std::lock_guard<std::mutex> lock_output(lm_output);

    result = screenshot();
    return;
}

bool Screenshot::screenshot()
{
    static HBITMAP	hBmp;

    if (giHandle == NULL)
    {
        err = 12;//���ھ��ʧЧ
        return false;
    }
    if (!IsWindow(giHandle))
    {
        err = 12;//���ھ��ʧЧ
        return false;
    }

    getGenshinImpactScale();

    BITMAP bmp;

    DeleteObject(hBmp);


//��ȡĿ�����Ĵ��ڴ�СRECT
    GetWindowRect(giHandle, &giRect);/* ��ԭ�񴰿ڵĲ��� */

//��ȡĿ������DC
    HDC hScreen = GetDC(giHandle);/* ��ԭ�񴰿ڵĲ��� */
    HDC hCompDC = CreateCompatibleDC(hScreen);

//��ȡĿ�����Ŀ�Ⱥ͸߶�
    int	nWidth = (int)((screen_scale) * (giRect.right - giRect.left));
    int	nHeight = (int)((screen_scale) * (giRect.bottom - giRect.top));

//����Bitmap����
    hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);//�õ�λͼ

    SelectObject(hCompDC, hBmp); //��д��ȫ��
    BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);

//�ͷŶ���
    DeleteDC(hScreen);
    DeleteDC(hCompDC);

//����ת��
//�����ȡλͼ�Ĵ�С��Ϣ,��ʵ��Ҳ�Ǽ���DC��ͼ����ķ�Χ
    GetObject(hBmp, sizeof(BITMAP), &bmp);

    int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
    int depth = bmp.bmBitsPixel == 1 ? 1 : 8;

//mat����

    //std::lock_guard<std::mutex> lock(lm_output);

    giFrame.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));

    GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, giFrame.data);

    giFrame = giFrame(cv::Rect(giClientRect.left, giClientRect.top, giClientSize.width, giClientSize.height));


    if (giFrame.empty())
    {
        err=102;
        return false;
    }

    if (giFrame.cols < 480 || giFrame.rows < 360)
    {
        err=103;
        return false;
    }
    return true;
}

//��ȡ��Ļ���ű���
void Screenshot::getGenshinImpactScale() {

    GetClientRect(giHandle, &giClientRect);/* ��ԭ�񴰿ڵĲ��� */

    giClientSize.width = (int)(screen_scale * (giClientRect.right - giClientRect.left));
    giClientSize.height = (int)(screen_scale * (giClientRect.bottom - giClientRect.top));

    HWND hWnd = GetDesktopWindow();
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    // ��ȡ�������߼������߶�
    MONITORINFOEX miex;
    miex.cbSize = sizeof(miex);
    GetMonitorInfo(hMonitor, &miex);
    int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
    int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

    // ��ȡ��������������߶�
    DEVMODE dm;
    dm.dmSize = sizeof(dm);
    dm.dmDriverExtra = 0;
    EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
    int cxPhysical = dm.dmPelsWidth;
    int cyPhysical = dm.dmPelsHeight;

    double horzScale = ((double)cxPhysical / (double)cxLogical);
    screen_scale = horzScale;
}



