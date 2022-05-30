#include "cvAutoTrack.h"
#ifdef 摆烂
// #include "AutoTrackManagement.h"
// static AutoTrackManagement _AutoTrackManagement;
#else
#include "BaiLan/BaiLan.h"
BaiLan _AutoTrackManagement;
//static BaiLan _AutoTrackManagement;
#endif

/// <summary>
/// 开启获取信息服务，开启之后才能获取到持续更新的数据。
/// </summary>
/// <returns></returns>
bool __stdcall startService()
{

//#if (defined(DEBUG) || defined(_DEBUG))
//#ifdef _MSC_VER
    //_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(2089);
//#endif
//#endif

    return _AutoTrackManagement.startService();
}
/// <summary>
/// 停止获取信息，停止后调用非阻塞API将只会获取到最后一次更新的数据。
/// </summary>
/// <returns></returns>
bool __stdcall stopService()
{
    return _AutoTrackManagement.stopService();
}
bool __stdcall init()
{
    return _AutoTrackManagement.init();
}
bool __stdcall uninit()
{
    return _AutoTrackManagement.uninit();
}
bool __stdcall SetHandle(long long int handle)
{
    return _AutoTrackManagement.SetHandle(handle);
}
bool __stdcall SetWorldCenter(double x, double y)
{
    return _AutoTrackManagement.SetWorldCenter(x, y);
}
bool __stdcall SetWorldScale(double scale)
{
    return _AutoTrackManagement.SetWorldScale(scale);
}

/// <summary>
/// 获取角色的定位，返回值为0表示失败，其他值表示对应的地图id
/// </summary>
/// <param name="x">传入x的引用，将会改变x的值为输出的角色x坐标</param>
/// <param name="y">传入y的引用，将会改变y的值为输出的角色y坐标</param>
/// <param name="a">传入a的引用，将会改变x的值为输出的角色朝向</param>
/// <returns>是否成功</returns>
bool __stdcall GetTransform(double &x, double&y, double&a)
{
    return _AutoTrackManagement.GetTransform(x, y, a);
}
/// <summary>
/// 获取角色定位
/// </summary>
/// <param name="x">传入x的引用，将会改变x的值为输出的角色x坐标</param>
/// <param name="y">传入y的引用，将会改变y的值为输出的角色y坐标</param>
/// <returns>是否成功</returns>
bool __stdcall GetPosition(double & x, double & y)
{
    return _AutoTrackManagement.GetPosition(x, y);
}
/// <summary>
/// 获取角色朝向
/// </summary>
/// <param name="a"></param>
/// <returns></returns>
bool __stdcall GetDirection(double & a)
{
    return _AutoTrackManagement.GetDirection(a);
}
/// <summary>
/// 获取视角朝向
/// </summary>
/// <param name="a2"></param>
/// <returns></returns>
bool __stdcall GetRotation(double & a2)
{
    return _AutoTrackManagement.GetRotation(a2);
}
bool __stdcall GetStar(double &x, double &y, bool &isEnd)
{
    return _AutoTrackManagement.GetStar(x, y, isEnd);
}
/// <summary>
/// 获取神瞳信息，以json字符串的形式
/// </summary>
/// <param name="jsonBuff"></param>
/// <returns></returns>
bool GetStarJson(char * jsonBuff)
{
    return _AutoTrackManagement.GetStarJson(jsonBuff);
}
/// <summary>
/// 获取游戏账号UID
/// </summary>
/// <param name="uid"></param>
/// <returns></returns>
bool __stdcall GetUID(int & uid)
{
    return _AutoTrackManagement.GetUID(uid);
}

/// <summary>
/// 以阻塞的方式进行对指定id的地图进行匹配定位，获取角色的定位，返回值为0表示失败，其他值表示对应的地图id
/// </summary>
/// <param name="x">传入x的引用，将会改变x的值为输出的角色x坐标</param>
/// <param name="y">传入y的引用，将会改变y的值为输出的角色y坐标</param>
/// <param name="a">传入a的引用，将会改变x的值为输出的角色朝向</param>
/// <param name="map_id">指定要匹配的地图，以id的方式</param>
/// <returns>是否成功</returns>
bool __stdcall GetTransformBlock(double& x, double& y, double& a, int map_index) {
    return false;
}
/// <summary>
/// 获取角色定位
/// </summary>
/// <param name="x">传入x的引用，将会改变x的值为输出的角色x坐标</param>
/// <param name="y">传入y的引用，将会改变y的值为输出的角色y坐标</param>
/// <returns>是否成功</returns>
bool __stdcall GetPositionBlock(double& x, double& y)
{
    return false;
}
/// <summary>
/// 获取角色朝向
/// </summary>
/// <param name="a"></param>
/// <returns></returns>
 bool __stdcall GetDirectionBlock(double& a)
{
     return false;
 }

 bool __stdcall GetInfoLoadPicture(char* path, int& uid, double& x, double& y, double& a)
 {
     return false;//_AutoTrackManagement.GetInfoLoadPicture(path, uid, x, y, a);
 }
 bool __stdcall GetInfoLoadVideo(char* path, char* pathOutFile)
 {
     return false;//_AutoTrackManagement.GetInfoLoadVideo(path, pathOutFile);
 }

 /// <summary>
/// 获取最后一个错误码
/// </summary>
/// <returns>错误码</returns>
int __stdcall GetLastErr()
{
    return _AutoTrackManagement.GetLastError();
}


// const char * __stdcall GetLastErrStr()
// {
//     return _AutoTrackManagement.GetLastErrorStr();
// }
