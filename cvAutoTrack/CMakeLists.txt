cmake_minimum_required(VERSION 3.21)
project(cvAutoTrack VERSION 7.0.0)
# 设置c++版本
set(CMAKE_CXX_STANDARD 20)
# 是否开启debug模式
# set(CMAKE_BUILD_TYPE RELEASE)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if (WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

set(build_mode Static) # 静态库
include_directories(${CMAKE_SOURCE_DIR}/cvAutoTrack)

# 设置opencv的库类型
set(opencv_is_world false)
set(opencv_is_static true)
# 生成opencv库目录路径字符串
# 关于是否world和static的设置
if (${opencv_is_world} STREQUAL true)
    set(opencv_world_str World)
else()
    set(opencv_world_str NoWorld)
endif ()
if(${opencv_is_static} STREQUAL true)
    set(opencv_static_str Static)
    set(opencv_lib_str staticlib)
else()
    set(opencv_static_str Shared)
    set(opencv_lib_str lib)
endif ()
# 引入opencv库，设置opencv的目录
set(opencv_install_dir C:/tmp/opencv_455/install/install_build_${opencv_static_str}_${opencv_world_str})
# 添加opencv库的include目录
set(opencv_include_dir ${opencv_install_dir}/include)
include_directories(${opencv_include_dir})
# 添加opencv库的lib目录
set(opencv_lib_dir ${opencv_install_dir}/x64/vc16/${opencv_lib_str})
link_directories(${opencv_lib_dir})

# 引入cuda库，设置cuda的目录
set(cuda_install_dir "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.6")
# 添加cuda库的include目录
set(cuda_include_dir ${cuda_install_dir}/include)
include_directories(${cuda_include_dir})
# 添加cuda库的lib目录
set(cuda_lib_dir ${cuda_install_dir}/lib/x64)
link_directories(${cuda_lib_dir})


# 添加源文件目录
file(GLOB_RECURSE SRC_FILES *.cpp)
foreach(dir in ${SRC_FILES})
    string(REGEX REPLACE "/([^/]*)[.]cpp" "" dir ${dir})
    aux_source_directory(${dir} cvAutoTrack_src)
endforeach()

# 添加版本号文件和资源文件
if(MSVC)
    set(MY_VERSIONINFO_RC "${CMAKE_BINARY_DIR}/resource.rc")
    configure_file("${CMAKE_SOURCE_DIR}/cvAutoTrack/resource.rc.in" "${MY_VERSIONINFO_RC}")
endif()

# 生成动态链接库和静态链接库

add_library(cvAutoTrack SHARED ${cvAutoTrack_src} ${MY_VERSIONINFO_RC})
set_target_properties(cvAutoTrack PROPERTIES CLEAN_DIRECT_OUTPUT 1)

#  ${opencv_lib_dir}/aded.lib
#  ${opencv_lib_dir}/IlmImfd.lib
#  ${opencv_lib_dir}/ippicvmt.lib
#  ${opencv_lib_dir}/ippiwd.lib
#  ${opencv_lib_dir}/ittnotifyd.lib
#  ${opencv_lib_dir}/libjpeg-turbod.lib
#  ${opencv_lib_dir}/libopenjp2d.lib
#  ${opencv_lib_dir}/libpngd.lib
#  ${opencv_lib_dir}/libprotobufd.lib
#  ${opencv_lib_dir}/libtiffd.lib
#  ${opencv_lib_dir}/libwebpd.lib
#  ${opencv_lib_dir}/opencv_img_hash455d.lib
#  ${opencv_lib_dir}/opencv_world455d.lib
#  ${opencv_lib_dir}/quircd.lib
#  ${opencv_lib_dir}/zlibd.lib
# target_link_libraries(cvAutoTrack ${opencv_lib_dir}/aded.lib)
# target_link_libraries(cvAutoTrack ${opencv_lib_dir}/IlmImfd.lib)
# target_link_libraries(cvAutoTrack ${opencv_lib_dir}/ippicvmt.lib)
# target_link_libraries(cvAutoTrack ${opencv_lib_dir}/ippiwd.lib)
# target_link_libraries(cvAutoTrack ${opencv_lib_dir}/ittnotifyd.lib)
# target_link_libraries(cvAutoTrack ${opencv_lib_dir}/libjpeg-turbod.lib)
# target_link_libraries(cvAutoTrack ${opencv_lib_dir}/libopenjp2d.lib)
# target_link_libraries(cvAutoTrack ${opencv_lib_dir}/libpngd.lib)
# target_link_libraries(cvAutoTrack ${opencv_lib_dir}/libprotobufd.lib)
# target_link_libraries(cvAutoTrack ${opencv_lib_dir}/libtiffd.lib)
# target_link_libraries(cvAutoTrack ${opencv_lib_dir}/libwebpd.lib)
# target_link_libraries(cvAutoTrack ${opencv_lib_dir}/opencv_img_hash455d.lib)
# target_link_libraries(cvAutoTrack ${opencv_lib_dir}/opencv_world455d.lib)
# target_link_libraries(cvAutoTrack ${opencv_lib_dir}/quircd.lib)
# target_link_libraries(cvAutoTrack ${opencv_lib_dir}/zlibd.lib)

# ${opencv_lib_dir}/aded.lib
# ${opencv_lib_dir}/IlmImfd.lib
# ${opencv_lib_dir}/ippicvmt.lib
# ${opencv_lib_dir}/ippiwd.lib
# ${opencv_lib_dir}/ittnotifyd.lib
# ${opencv_lib_dir}/libjpeg-turbod.lib
# ${opencv_lib_dir}/libopenjp2d.lib
# ${opencv_lib_dir}/libpngd.lib
# ${opencv_lib_dir}/libprotobufd.lib
# ${opencv_lib_dir}/libtiffd.lib
# ${opencv_lib_dir}/libwebpd.lib
# ${opencv_lib_dir}/opencv_aruco455d.lib
# ${opencv_lib_dir}/opencv_barcode455d.lib
# ${opencv_lib_dir}/opencv_bgsegm455d.lib
# ${opencv_lib_dir}/opencv_bioinspired455d.lib
# ${opencv_lib_dir}/opencv_calib3d455d.lib
# ${opencv_lib_dir}/opencv_ccalib455d.lib
# ${opencv_lib_dir}/opencv_core455d.lib
# ${opencv_lib_dir}/opencv_cudaarithm455d.lib
# ${opencv_lib_dir}/opencv_cudabgsegm455d.lib
# ${opencv_lib_dir}/opencv_cudacodec455d.lib
# ${opencv_lib_dir}/opencv_cudafeatures2d455d.lib
# ${opencv_lib_dir}/opencv_cudafilters455d.lib
# ${opencv_lib_dir}/opencv_cudaimgproc455d.lib
# ${opencv_lib_dir}/opencv_cudalegacy455d.lib
# ${opencv_lib_dir}/opencv_cudaobjdetect455d.lib
# ${opencv_lib_dir}/opencv_cudaoptflow455d.lib
# ${opencv_lib_dir}/opencv_cudastereo455d.lib
# ${opencv_lib_dir}/opencv_cudawarping455d.lib
# ${opencv_lib_dir}/opencv_cudev455d.lib
# ${opencv_lib_dir}/opencv_datasets455d.lib
# ${opencv_lib_dir}/opencv_dnn455d.lib
# ${opencv_lib_dir}/opencv_dnn_objdetect455d.lib
# ${opencv_lib_dir}/opencv_dnn_superres455d.lib
# ${opencv_lib_dir}/opencv_dpm455d.lib
# ${opencv_lib_dir}/opencv_face455d.lib
# ${opencv_lib_dir}/opencv_features2d455d.lib
# ${opencv_lib_dir}/opencv_flann455d.lib
# ${opencv_lib_dir}/opencv_fuzzy455d.lib
# ${opencv_lib_dir}/opencv_gapi455d.lib
# ${opencv_lib_dir}/opencv_hfs455d.lib
# ${opencv_lib_dir}/opencv_highgui455d.lib
# ${opencv_lib_dir}/opencv_imgcodecs455d.lib
# ${opencv_lib_dir}/opencv_imgproc455d.lib
# ${opencv_lib_dir}/opencv_img_hash455d.lib
# ${opencv_lib_dir}/opencv_intensity_transform455d.lib
# ${opencv_lib_dir}/opencv_line_descriptor455d.lib
# ${opencv_lib_dir}/opencv_mcc455d.lib
# ${opencv_lib_dir}/opencv_ml455d.lib
# ${opencv_lib_dir}/opencv_objdetect455d.lib
# ${opencv_lib_dir}/opencv_optflow455d.lib
# ${opencv_lib_dir}/opencv_phase_unwrapping455d.lib
# ${opencv_lib_dir}/opencv_photo455d.lib
# ${opencv_lib_dir}/opencv_plot455d.lib
# ${opencv_lib_dir}/opencv_quality455d.lib
# ${opencv_lib_dir}/opencv_rapid455d.lib
# ${opencv_lib_dir}/opencv_reg455d.lib
# ${opencv_lib_dir}/opencv_rgbd455d.lib
# ${opencv_lib_dir}/opencv_saliency455d.lib
# ${opencv_lib_dir}/opencv_shape455d.lib
# ${opencv_lib_dir}/opencv_stereo455d.lib
# ${opencv_lib_dir}/opencv_stitching455d.lib
# ${opencv_lib_dir}/opencv_structured_light455d.lib
# ${opencv_lib_dir}/opencv_superres455d.lib
# ${opencv_lib_dir}/opencv_surface_matching455d.lib
# ${opencv_lib_dir}/opencv_text455d.lib
# ${opencv_lib_dir}/opencv_tracking455d.lib
# ${opencv_lib_dir}/opencv_video455d.lib
# ${opencv_lib_dir}/opencv_videoio455d.lib
# ${opencv_lib_dir}/opencv_videostab455d.lib
# ${opencv_lib_dir}/opencv_wechat_qrcode455d.lib
# ${opencv_lib_dir}/opencv_xfeatures2d455d.lib
# ${opencv_lib_dir}/opencv_ximgproc455d.lib
# ${opencv_lib_dir}/opencv_xobjdetect455d.lib
# ${opencv_lib_dir}/opencv_xphoto455d.lib
# ${opencv_lib_dir}/quircd.lib
# ${opencv_lib_dir}/zlibd.lib
target_link_libraries(cvAutoTrack
        aded.lib
        IlmImfd.lib
        ippicvmt.lib
        ippiwd.lib
        ittnotifyd.lib
        libjpeg-turbod.lib
        libopenjp2d.lib
        libpngd.lib
        libprotobufd.lib
        libtiffd.lib
        libwebpd.lib
        opencv_aruco455d.lib
        opencv_barcode455d.lib
        opencv_bgsegm455d.lib
        opencv_bioinspired455d.lib
        opencv_calib3d455d.lib
        opencv_ccalib455d.lib
        opencv_core455d.lib
        opencv_cudaarithm455d.lib
        opencv_cudabgsegm455d.lib
        opencv_cudacodec455d.lib
        opencv_cudafeatures2d455d.lib
        opencv_cudafilters455d.lib
        opencv_cudaimgproc455d.lib
        opencv_cudalegacy455d.lib
        opencv_cudaobjdetect455d.lib
        opencv_cudaoptflow455d.lib
        opencv_cudastereo455d.lib
        opencv_cudawarping455d.lib
        opencv_cudev455d.lib
        opencv_datasets455d.lib
        opencv_dnn455d.lib
        opencv_dnn_objdetect455d.lib
        opencv_dnn_superres455d.lib
        opencv_dpm455d.lib
        opencv_face455d.lib
        opencv_features2d455d.lib
        opencv_flann455d.lib
        opencv_fuzzy455d.lib
        opencv_gapi455d.lib
        opencv_hfs455d.lib
        opencv_highgui455d.lib
        opencv_imgcodecs455d.lib
        opencv_imgproc455d.lib
        opencv_img_hash455d.lib
        opencv_intensity_transform455d.lib
        opencv_line_descriptor455d.lib
        opencv_mcc455d.lib
        opencv_ml455d.lib
        opencv_objdetect455d.lib
        opencv_optflow455d.lib
        opencv_phase_unwrapping455d.lib
        opencv_photo455d.lib
        opencv_plot455d.lib
        opencv_quality455d.lib
        opencv_rapid455d.lib
        opencv_reg455d.lib
        opencv_rgbd455d.lib
        opencv_saliency455d.lib
        opencv_shape455d.lib
        opencv_stereo455d.lib
        opencv_stitching455d.lib
        opencv_structured_light455d.lib
        opencv_superres455d.lib
        opencv_surface_matching455d.lib
        opencv_text455d.lib
        opencv_tracking455d.lib
        opencv_video455d.lib
        opencv_videoio455d.lib
        opencv_videostab455d.lib
        opencv_wechat_qrcode455d.lib
        opencv_xfeatures2d455d.lib
        opencv_ximgproc455d.lib
        opencv_xobjdetect455d.lib
        opencv_xphoto455d.lib
        quircd.lib
        zlibd.lib
        )
# 导入cuda库
target_link_libraries(cvAutoTrack
        cublas.lib
        cublasLt.lib
        cuda.lib
        cudadevrt.lib
        cudart.lib
        cudart_static.lib
        cudnn.lib
        cudnn64_8.lib
        cudnn_adv_infer.lib
        cudnn_adv_infer64_8.lib
        cudnn_adv_train.lib
        cudnn_adv_train64_8.lib
        cudnn_cnn_infer.lib
        cudnn_cnn_infer64_8.lib
        cudnn_cnn_train.lib
        cudnn_cnn_train64_8.lib
        cudnn_ops_infer.lib
        cudnn_ops_infer64_8.lib
        cudnn_ops_train.lib
        cudnn_ops_train64_8.lib
        cufft.lib
        cufftw.lib
        cufilt.lib
        curand.lib
        cusolver.lib
        cusolverMg.lib
        cusparse.lib
        nppc.lib
        nppial.lib
        nppicc.lib
        nppidei.lib
        nppif.lib
        nppig.lib
        nppim.lib
        nppist.lib
        nppisu.lib
        nppitc.lib
        npps.lib
        nvblas.lib
        nvjpeg.lib
        nvml.lib
        nvptxcompiler_static.lib
        nvrtc-builtins_static.lib
        nvrtc.lib
        nvrtc_static.lib
        OpenCL.lib
        )

# add_library(cvAutoTrack_opencv STATIC IMPORTED)
# set_property(TARGET cvAutoTrack_opencv PROPERTY IMPORTED_LOCATION ${opencv_lib_dir}/opencv_world455d.lib)
# target_link_libraries(cvAutoTrack cvAutoTrack_opencv)

# if(${build_mode} STREQUAL Static)
#     add_library(cvAutoTrack_static STATIC ${cvAutoTrack_src} ${MY_VERSIONINFO_RC})
#     set_target_properties(cvAutoTrack_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
#     target_link_libraries(cvAutoTrack_static ${opencv_lib_dir}/opencv_world455d.lib)
#     # 重命名静态链接库文件
#     set_target_properties(cvAutoTrack_static PROPERTIES OUTPUT_NAME "cvAutoTrack")
# endif()


# C:\tmp\opencv_455\install\install_build_Static_World\x64\vc16\staticlib\aded.lib
# C:\tmp\opencv_455\install\install_build_Static_World\x64\vc16\staticlib\IlmImfd.lib
# C:\tmp\opencv_455\install\install_build_Static_World\x64\vc16\staticlib\ippiwd.lib
# C:\tmp\opencv_455\install\install_build_Static_World\x64\vc16\staticlib\ittnotifyd.lib
# C:\tmp\opencv_455\install\install_build_Static_World\x64\vc16\staticlib\libjpeg-turbod.lib
# C:\tmp\opencv_455\install\install_build_Static_World\x64\vc16\staticlib\libopenjp2d.lib
# C:\tmp\opencv_455\install\install_build_Static_World\x64\vc16\staticlib\libpngd.lib
# C:\tmp\opencv_455\install\install_build_Static_World\x64\vc16\staticlib\libprotobufd.lib
# C:\tmp\opencv_455\install\install_build_Static_World\x64\vc16\staticlib\libtiffd.lib
# C:\tmp\opencv_455\install\install_build_Static_World\x64\vc16\staticlib\libwebpd.lib
# C:\tmp\opencv_455\install\install_build_Static_World\x64\vc16\staticlib\opencv_img_hash455d.lib
# C:\tmp\opencv_455\install\install_build_Static_World\x64\vc16\staticlib\opencv_world455d.lib
# C:\tmp\opencv_455\install\install_build_Static_World\x64\vc16\staticlib\quircd.lib
# C:\tmp\opencv_455\install\install_build_Static_World\x64\vc16\staticlib\zlibd.lib

# ippicvmt.lib


# cvAutoTrack
#     -CMakeLists.txt
#     -cvAutoTrack
#          -xxx.h
#          -xxx.cpp
#          -xxx.rc
#          -CMakeLists.txt
#     -test
#          -test.cpp
#          -CMakeLists.txt

# ade.lib
# IlmImf.lib
# ippiw.lib
# ittnotify.lib
# libjpeg-turbo.lib
# libopenjp2.lib
# libpng.lib
# libprotobuf.lib
# libtiff.lib
# libwebp.lib
# opencv_img_hash455.lib
# opencv_world455.lib
# quirc.lib
# zlib.lib


# ade.lib
# IlmImf.lib
# ippicvmt.lib
# ippiw.lib 
# ittnotify.lib 
# libjpeg-turbo.lib 
# libopenjp2.lib 
# libpng.lib 
# libprotobuf.lib 
# libtiff.lib 
# libwebp.lib 
# opencv_aruco455.lib 
# opencv_barcode455.lib 
# opencv_bgsegm455.lib 
# opencv_bioinspired455.lib 
# opencv_calib3d455.lib 
# opencv_ccalib455.lib 
# opencv_core455.lib 
# opencv_cudaarithm455.lib 
# opencv_cudabgsegm455.lib 
# opencv_cudacodec455.lib 
# opencv_cudafeatures2d455.lib 
# opencv_cudafilters455.lib 
# opencv_cudaimgproc455.lib 
# opencv_cudalegacy455.lib 
# opencv_cudaobjdetect455.lib 
# opencv_cudaoptflow455.lib 
# opencv_cudastereo455.lib 
# opencv_cudawarping455.lib 
# opencv_cudev455.lib 
# opencv_datasets455.lib 
# opencv_dnn455.lib 
# opencv_dnn_objdetect455.lib 
# opencv_dnn_superres455.lib 
# opencv_dpm455.lib 
# opencv_face455.lib 
# opencv_features2d455.lib 
# opencv_flann455.lib 
# opencv_fuzzy455.lib 
# opencv_gapi455.lib 
# opencv_hfs455.lib 
# opencv_highgui455.lib 
# opencv_imgcodecs455.lib 
# opencv_imgproc455.lib 
# opencv_img_hash455.lib 
# opencv_intensity_transform455.lib 
# opencv_line_descriptor455.lib 
# opencv_mcc455.lib 
# opencv_ml455.lib 
# opencv_objdetect455.lib 
# opencv_optflow455.lib 
# opencv_phase_unwrapping455.lib 
# opencv_photo455.lib 
# opencv_plot455.lib 
# opencv_quality455.lib 
# opencv_rapid455.lib 
# opencv_reg455.lib 
# opencv_rgbd455.lib
# opencv_saliency455.lib
# opencv_shape455.lib
# opencv_stereo455.lib
# opencv_stitching455.lib
# opencv_structured_light455.lib
# opencv_superres455.lib
# opencv_surface_matching455.lib
# opencv_text455.lib
# opencv_tracking455.lib
# opencv_video455.lib
# opencv_videoio455.lib
# opencv_videostab455.lib
# opencv_wechat_qrcode455.lib
# opencv_xfeatures2d455.lib
# opencv_ximgproc455.lib
# opencv_xobjdetect455.lib
# opencv_xphoto455.lib
# quirc.lib
# zlib.lib






# aded.lib
# IlmImfd.lib
# ippicvmt.lib
# ippiwd.lib
# ittnotifyd.lib
# libjpeg-turbod.lib
# libopenjp2d.lib
# libpngd.lib
# libprotobufd.lib
# libtiffd.lib
# libwebpd.lib
# opencv_aruco455d.lib
# opencv_barcode455d.lib
# opencv_bgsegm455d.lib
# opencv_bioinspired455d.lib
# opencv_calib3d455d.lib
# opencv_ccalib455d.lib
# opencv_core455d.lib
# opencv_cudaarithm455d.lib
# opencv_cudabgsegm455d.lib
# opencv_cudacodec455d.lib
# opencv_cudafeatures2d455d.lib
# opencv_cudafilters455d.lib
# opencv_cudaimgproc455d.lib
# opencv_cudalegacy455d.lib
# opencv_cudaobjdetect455d.lib
# opencv_cudaoptflow455d.lib
# opencv_cudastereo455d.lib
# opencv_cudawarping455d.lib
# opencv_cudev455d.lib
# opencv_datasets455d.lib
# opencv_dnn455d.lib
# opencv_dnn_objdetect455d.lib
# opencv_dnn_superres455d.lib
# opencv_dpm455d.lib
# opencv_face455d.lib
# opencv_features2d455d.lib
# opencv_flann455d.lib
# opencv_fuzzy455d.lib
# opencv_gapi455d.lib
# opencv_hfs455d.lib
# opencv_highgui455d.lib
# opencv_imgcodecs455d.lib
# opencv_imgproc455d.lib
# opencv_img_hash455d.lib
# opencv_intensity_transform455d.lib
# opencv_line_descriptor455d.lib
# opencv_mcc455d.lib
# opencv_ml455d.lib
# opencv_objdetect455d.lib
# opencv_optflow455d.lib
# opencv_phase_unwrapping455d.lib
# opencv_photo455d.lib
# opencv_plot455d.lib
# opencv_quality455d.lib
# opencv_rapid455d.lib
# opencv_reg455d.lib
# opencv_rgbd455d.lib
# opencv_saliency455d.lib
# opencv_shape455d.lib
# opencv_stereo455d.lib
# opencv_stitching455d.lib
# opencv_structured_light455d.lib
# opencv_superres455d.lib
# opencv_surface_matching455d.lib
# opencv_text455d.lib
# opencv_tracking455d.lib
# opencv_video455d.lib
# opencv_videoio455d.lib
# opencv_videostab455d.lib
# opencv_wechat_qrcode455d.lib
# opencv_xfeatures2d455d.lib
# opencv_ximgproc455d.lib
# opencv_xobjdetect455d.lib
# opencv_xphoto455d.lib
# quircd.lib
# zlibd.lib
#



# cublas.lib
# cublasLt.lib
# cuda.lib
# cudadevrt.lib
# cudart.lib
# cudart_static.lib
# cudnn.lib
# cudnn64_8.lib
# cudnn_adv_infer.lib
# cudnn_adv_infer64_8.lib
# cudnn_adv_train.lib
# cudnn_adv_train64_8.lib
# cudnn_cnn_infer.lib
# cudnn_cnn_infer64_8.lib
# cudnn_cnn_train.lib
# cudnn_cnn_train64_8.lib
# cudnn_ops_infer.lib
# cudnn_ops_infer64_8.lib
# cudnn_ops_train.lib
# cudnn_ops_train64_8.lib
# cufft.lib
# cufftw.lib
# cufilt.lib
# curand.lib
# cusolver.lib
# cusolverMg.lib
# cusparse.lib
# nppc.lib
# nppial.lib
# nppicc.lib
# nppidei.lib
# nppif.lib
# nppig.lib
# nppim.lib
# nppist.lib
# nppisu.lib
# nppitc.lib
# npps.lib
# nvblas.lib
# nvjpeg.lib
# nvml.lib
# nvptxcompiler_static.lib
# nvrtc-builtins_static.lib
# nvrtc.lib
# nvrtc_static.lib
# OpenCL.lib