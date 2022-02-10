cmake_minimum_required(VERSION 3.21)
project(cvAutoTrack  VERSION 7.0.0)
# 设置c++版本
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
# 是否开启debug模式
# set(CMAKE_BUILD_TYPE RELEASE)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -fPIC")
 # // # 设置静态链接
 # // set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} /MT")
 # // set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
 # //
 # // set(CMAKE_CXX_FLAGS_RELEASE /MT)
 # // set(CMAKE_CXX_FLAGS_DEBUG /MTd)
 # // set(CMAKE_CXX_FLAGS /MT)
 # // #if(MSVC)
 # //     foreach(var
 # //             CMAKE_C_FLAGS CMAKE_CXX_FLAGS
 # //             CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELEASE
 # //             CMAKE_C_FLAGS_DEBUG CMAKE_CXX_FLAGS_DEBUG
 # //             CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS_RELWITHDEBINFO
 # //             CMAKE_C_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_MINSIZEREL
 # //             )
 # //         if (${var} MATCHES "/MD")
 # //             string(REGEX REPLACE "/MD" "/MT" ${var} "${${var}}")
 # //         endif ()
 # //     endforeach()
 # // #endif()

# find_package(opencv455_shared NAMES install_build_Static_World PATHS C:/tmp/opencv_455/install)

include_directories(${CMAKE_SOURCE_DIR}/cvAutoTrack)

include_directories(include C:/tmp/opencv_455/install/install_build_Static_World/include)

# include_directories(${OpenCV_INCLUDE_DIRS})
# link_directories(${OpenCV_LIBRARY_DIRS})
# target_link_libraries(cvAutoTrack ${OpenCV_LIBRARIES})

include_directories(C:/tmp/opencv_455/install/install_build_Static_World/include)
link_directories(C:/tmp/opencv_455/install/install_build_Static_World/x64/vc16/staticlib)

# target_link_libraries(cvAutoTrack ${opencv455_shared})

#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
aux_source_directory(. cvAutoTrack_src)
aux_source_directory(./BaiLan cvAutoTrack_src)
aux_source_directory(./Data cvAutoTrack_src)
aux_source_directory(./GenshinFunction cvAutoTrack_src)
aux_source_directory(./GenshinFunction/Flow cvAutoTrack_src)
aux_source_directory(./GenshinFunction/HookHandle cvAutoTrack_src)
aux_source_directory(./GenshinFunction/Screenshot cvAutoTrack_src)
aux_source_directory(./GenshinFunction/SplitDispense cvAutoTrack_src)
aux_source_directory(./Logger cvAutoTrack_src)
aux_source_directory(./Task cvAutoTrack_src)
aux_source_directory(./Timer cvAutoTrack_src)

# 调试输出cmake路径


if(MSVC)
 set(MY_VERSIONINFO_RC "${CMAKE_BINARY_DIR}/resource.rc"
         BaiLan/resource/cvAutoTrack.rc
         BaiLan/BaiLan_Util_SplitArea.h
         BaiLan/BaiLan_Util_SplitMatching.cpp
         BaiLan/BaiLan_Util_SplitMatching.h
         BaiLan/BaiLan_Util_SplitMatching_InstanceWork.h
         BaiLan/BaiLan_Util.h
         BaiLan/BaiLan_Util_Matching.cpp
         BaiLan/BaiLan_Util_Matching.h
         BaiLan/BaiLan_Util_Resource.cpp
         BaiLan/BaiLan_Util_Resource.h
         BaiLan/BaiLan_Util_SplitMatchingII.cpp
         BaiLan/BaiLan_Util_SplitMatchingII.h
         BaiLan/SURF_Map.cpp
         BaiLan/SURF_Map.h
         )
 configure_file("${CMAKE_SOURCE_DIR}/cvAutoTrack/resource.rc.in" "${MY_VERSIONINFO_RC}")
endif()

# 生成动态链接库和静态链接库
add_library(cvAutoTrack SHARED ${cvAutoTrack_src} ${MY_VERSIONINFO_RC})
add_library(cvAutoTrack_static STATIC ${cvAutoTrack_src})
# 重命名静态链接库文件
set_target_properties(cvAutoTrack_static PROPERTIES OUTPUT_NAME "cvAutoTrack")

set_target_properties(cvAutoTrack PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(cvAutoTrack_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

if( CMAKE_BUILD_TYPE STREQUAL Debug)
 # target_link_libraries(cvAutoTrack C:/tmp/opencv_455/install/install_build_Shared_World/x64/vc16/lib/opencv_world455d.lib)
endif()
if(CMAKE_BUILD_TYPE STREQUAL Release)
  #target_link_libraries(cvAutoTrack C:/tmp/opencv_455/install/install_build_Shared_World/x64/vc16/lib/opencv_world455.lib)
endif()

# target_link_libraries(cvAutoTrack C:/tmp/opencv_455/install/install_build_Shared_World/x64/vc16/lib/opencv_world455d.lib)
target_link_libraries(cvAutoTrack C:/tmp/opencv_455/install/install_build_Static_World/x64/vc16/staticlib/opencv_world455d.lib )
