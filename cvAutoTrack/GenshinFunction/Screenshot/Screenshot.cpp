//
// Created by GengG on 2022/1/28.
//

#include "Screenshot.h"
Screenshot::Screenshot(const Logger& logService)
{
    log=logService;
    log->log("截图模块加载");
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
        err = 12;//窗口句柄失效
        return false;
    }
    if (!IsWindow(giHandle))
    {
        err = 12;//窗口句柄失效
        return false;
    }

    getGenshinImpactScale();

    //log -> Log("截图模块开始截图");
    //log ->Info ("giClientSize :" + std::to_string(giClientSize.width) + "," + std::to_string(giClientSize.height));

    BITMAP bmp;

    DeleteObject(hBmp);


//获取目标句柄的窗口大小RECT
    GetWindowRect(giHandle, &giRect);/* 对原神窗口的操作 */

//获取目标句柄的DC
    HDC hScreen = GetDC(giHandle);/* 对原神窗口的操作 */
    HDC hCompDC = CreateCompatibleDC(hScreen);

//获取目标句柄的宽度和高度
    int	nWidth = (int)((screen_scale) * (giRect.right - giRect.left));
    int	nHeight = (int)((screen_scale) * (giRect.bottom - giRect.top));

//创建Bitmap对象
    hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);//得到位图

    SelectObject(hCompDC, hBmp); //不写就全黑
    BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);

//释放对象
    DeleteDC(hScreen);
    DeleteDC(hCompDC);

//类型转换
//这里获取位图的大小信息,事实上也是兼容DC绘图输出的范围
    GetObject(hBmp, sizeof(BITMAP), &bmp);

    int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
    int depth = bmp.bmBitsPixel == 1 ? 1 : 8;

//mat操作

    //std::lock_guard<std::mutex> lock(lm_output);

    giFrame.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));

    GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, giFrame.data);

    giFrame = giFrame(cv::Rect(giClientRect.left, giClientRect.top, giClientSize.width, giClientSize.height));

    //log->Info("giFrame size:"+std::to_string(giFrame.size().width)+"*"+std::to_string(giFrame.size().height) );

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

//获取屏幕缩放比例
void Screenshot::getGenshinImpactScale() {

    GetClientRect(giHandle, &giClientRect);/* 对原神窗口的操作 */

    giClientSize.width = (int)(screen_scale * (giClientRect.right - giClientRect.left));
    giClientSize.height = (int)(screen_scale * (giClientRect.bottom - giClientRect.top));

    HWND hWnd = GetDesktopWindow();
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    // 获取监视器逻辑宽度与高度
    MONITORINFOEX miex;
    miex.cbSize = sizeof(miex);
    GetMonitorInfo(hMonitor, &miex);
    int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
    int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

    // 获取监视器物理宽度与高度
    DEVMODE dm;
    dm.dmSize = sizeof(dm);
    dm.dmDriverExtra = 0;
    EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
    int cxPhysical = dm.dmPelsWidth;
    int cyPhysical = dm.dmPelsHeight;

    double horzScale = ((double)cxPhysical / (double)cxLogical);
    screen_scale = horzScale;
}



