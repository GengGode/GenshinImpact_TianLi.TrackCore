//
// Created by GengG on 2022/2/6.
//

#include "BaiLan_Util_Resource.h"
#include "resource/resource.h"
#include <wincodec.h>


bool HBitmap2Mat(HBITMAP& _hBmp, cv::Mat& _mat)
{
    BITMAP bmp;
    GetObject(_hBmp, sizeof(BITMAP), &bmp);
    int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
    int depth = bmp.bmBitsPixel == 1 ? 1 : 8;
    cv::Mat v_mat;
    v_mat.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8UC3, nChannels));
    GetBitmapBits(_hBmp, bmp.bmHeight*bmp.bmWidth*nChannels, v_mat.data);
    _mat = v_mat;
    return false;
}

bool LoadPNG2Mat(cv::Mat& _mat)
{
    //HRESULT hr = S_OK;
    HMODULE hModu = NULL;
    IWICStream *pIWICStream = NULL;
    IWICBitmapDecoder *pIDecoder = NULL;
    IWICBitmapFrameDecode *pIDecoderFrame = NULL;
    IWICImagingFactory *m_pIWICFactory = NULL;
    IWICBitmapSource *bitmap_source = NULL; //
    HRSRC imageResHandle = NULL;
    HGLOBAL imageResDataHandle = NULL;
    void *pImageFile = NULL;
    DWORD imageFileSize = 0;
    
    hModu = GetModuleHandleW(L"CVAUTOTRACK.dll");
    
    if (hModu == NULL) throw "Get Dll Instance Fail!";
    
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    
    CoCreateInstance(
            CLSID_WICImagingFactory,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&m_pIWICFactory)
    );
    
    imageResHandle = ::FindResource(hModu, MAKEINTRESOURCE(IDB_PNG_GIMAP), "PNG");
    imageResDataHandle = LoadResource(hModu, imageResHandle);
    pImageFile = LockResource(imageResDataHandle);
    imageFileSize = SizeofResource(hModu, imageResHandle);
    m_pIWICFactory->CreateStream(&pIWICStream);
    
    pIWICStream->InitializeFromMemory(
            reinterpret_cast<BYTE*>(pImageFile),
            imageFileSize);
    m_pIWICFactory->CreateDecoderFromStream(
            pIWICStream,                   // The stream to use to create the decoder
            NULL,                          // Do not prefer a particular vendor
            WICDecodeMetadataCacheOnLoad,  // Cache metadata when needed
            &pIDecoder);                   // Pointer to the decoder
    pIDecoder->GetFrame(0, &pIDecoderFrame);
    
    bitmap_source = pIDecoderFrame;
    
    UINT width = 0, height = 0, depht = 4;
    //UINT width = 0, height = 0, depht = 3;
    bitmap_source->GetSize(&width, &height);
    
    {
        std::vector<BYTE> buffer(width * height * depht);
        bitmap_source->CopyPixels(NULL, width * depht, buffer.size(), buffer.data());
        
        HBITMAP bitmap = CreateBitmap(width, height, 1, 8 * depht, buffer.data());
        
        HBitmap2Mat(bitmap, _mat);
        
        // buffer.resize(0);
        // buffer.reserve(0);
        
        DeleteObject(bitmap);
    }
    
    DeleteObject(bitmap_source);
    
    CoFreeUnusedLibraries();
    CoUninitialize();
    
    
    if (_mat.empty())
    {
        return false;
    }
    cv::cvtColor(_mat, _mat, cv::COLOR_RGBA2RGB);
    if (_mat.empty())
    {
        return false;
    }
    return true;
}

BaiLan_Util_Resource::BaiLan_Util_Resource()
{
    install();
}

BaiLan_Util_Resource::~BaiLan_Util_Resource()
{
    release();
}

/*
HMODULE GetSelfModuleHandle()
{
        MEMORY_BASIC_INFORMATION mbi;

        return ((::VirtualQuery(GetSelfModuleHandle, &mbi, sizeof(mbi)) != 0)
                ? (HMODULE) mbi.AllocationBase : NULL);
}
*/


void BaiLan_Util_Resource::install()
{
    HBITMAP gHmp;
    gHmp = LoadBitmap(GetModuleHandleW(L"CVAUTOTRACK.dll"), MAKEINTRESOURCE(IDB_BITMAP_PAIMON));
    if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
    HBitmap2Mat(gHmp, PaimonTemplate);
    
    gHmp = LoadBitmap(GetModuleHandleW(L"CVAUTOTRACK.dll"), MAKEINTRESOURCE(IDB_BITMAP_STAR));
    if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
    HBitmap2Mat(gHmp, StarTemplate);
    
    //gHmp = LoadBitmap(GetModuleHandleW(L"CVAUTOTRACK.dll"), MAKEINTRESOURCE(IDB_BITMAP_GIMAP));
    //if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
    //HBitmap2Mat(gHmp, MapTemplate);
    
    if (LoadPNG2Mat(MapTemplate) == false)throw"LoadSource Get Resource From Dll HBitmap faile";
    
    gHmp = LoadBitmap(GetModuleHandleW(L"CVAUTOTRACK.dll"), MAKEINTRESOURCE(IDB_BITMAP_UID_));
    if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
    HBitmap2Mat(gHmp, UID);
    gHmp = LoadBitmap(GetModuleHandleW(L"CVAUTOTRACK.dll"), MAKEINTRESOURCE(IDB_BITMAP_UID0));
    if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
    HBitmap2Mat(gHmp, UIDnumber[0]);
    gHmp = LoadBitmap(GetModuleHandleW(L"CVAUTOTRACK.dll"), MAKEINTRESOURCE(IDB_BITMAP_UID1));
    if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
    HBitmap2Mat(gHmp, UIDnumber[1]);
    gHmp = LoadBitmap(GetModuleHandleW(L"CVAUTOTRACK.dll"), MAKEINTRESOURCE(IDB_BITMAP_UID2));
    if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
    HBitmap2Mat(gHmp, UIDnumber[2]);
    gHmp = LoadBitmap(GetModuleHandleW(L"CVAUTOTRACK.dll"), MAKEINTRESOURCE(IDB_BITMAP_UID3));
    if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
    HBitmap2Mat(gHmp, UIDnumber[3]);
    gHmp = LoadBitmap(GetModuleHandleW(L"CVAUTOTRACK.dll"), MAKEINTRESOURCE(IDB_BITMAP_UID4));
    if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
    HBitmap2Mat(gHmp, UIDnumber[4]);
    gHmp = LoadBitmap(GetModuleHandleW(L"CVAUTOTRACK.dll"), MAKEINTRESOURCE(IDB_BITMAP_UID5));
    if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
    HBitmap2Mat(gHmp, UIDnumber[5]);
    gHmp = LoadBitmap(GetModuleHandleW(L"CVAUTOTRACK.dll"), MAKEINTRESOURCE(IDB_BITMAP_UID6));
    if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
    HBitmap2Mat(gHmp, UIDnumber[6]);
    gHmp = LoadBitmap(GetModuleHandleW(L"CVAUTOTRACK.dll"), MAKEINTRESOURCE(IDB_BITMAP_UID7));
    if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
    HBitmap2Mat(gHmp, UIDnumber[7]);
    gHmp = LoadBitmap(GetModuleHandleW(L"CVAUTOTRACK.dll"), MAKEINTRESOURCE(IDB_BITMAP_UID8));
    if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
    HBitmap2Mat(gHmp, UIDnumber[8]);
    gHmp = LoadBitmap(GetModuleHandleW(L"CVAUTOTRACK.dll"), MAKEINTRESOURCE(IDB_BITMAP_UID9));
    if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
    HBitmap2Mat(gHmp, UIDnumber[9]);
    
    StarRGBA2A();
    UIDnumberRGBA2A();
}

void BaiLan_Util_Resource::release()
{
    PaimonTemplate.release();
    StarTemplate.release();
    MapTemplate.release();
    UID.release();
    UIDnumber[0].release();
    UIDnumber[1].release();
    UIDnumber[2].release();
    UIDnumber[3].release();
    UIDnumber[4].release();
    UIDnumber[5].release();
    UIDnumber[6].release();
    UIDnumber[7].release();
    UIDnumber[8].release();
    UIDnumber[9].release();
}

void BaiLan_Util_Resource::StarRGBA2A()
{
    cv::cvtColor(StarTemplate, StarTemplate, cv::COLOR_RGBA2GRAY);
}

void BaiLan_Util_Resource::UIDnumberRGBA2A()
{
    cv::cvtColor(UID, UID, cv::COLOR_RGBA2GRAY);
    for (int i = 0; i < 10; i++)
    {
        cv::cvtColor(UIDnumber[i], UIDnumber[i], cv::COLOR_RGBA2GRAY);
    }
}
