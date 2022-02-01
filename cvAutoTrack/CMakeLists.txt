cmake_minimum_required(VERSION 3.21)
project(cvAutoTrack)
# set(CMAKE_CXX_STANDARD 20)
# add_library(cvAutoTrack SHARED cvAutoTrack/library.cpp)

set(CMAKE_CXX_STANDARD 20)

# find_package(opencv455_shared NAMES install_build_Static_World PATHS C:/tmp/opencv_455/install)

include_directories(${CMAKE_SOURCE_DIR}/cvAutoTrack)

include_directories(include C:/tmp/opencv_455/install/install_build_Static_World/include)

# include_directories(${OpenCV_INCLUDE_DIRS})
# link_directories(${OpenCV_LIBRARY_DIRS})
# target_link_libraries(cvAutoTrack ${OpenCV_LIBRARIES})

include_directories(C:/tmp/opencv_455/install/install_build_Shared_World/include)
link_directories(C:/tmp/opencv_455/install/install_build_Shared_World/x64/vc16/lib)

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


# add_library(cvAutoTrack STATIC ${cvAutoTrack_src})

 add_library(cvAutoTrack SHARED ${cvAutoTrack_src} BaiLan/BaiLan_Util.h BaiLan/BaiLan_Util_SplitArea.h BaiLan/BaiLan_Util_SplitMatching.cpp BaiLan/BaiLan_Util_SplitMatching.h BaiLan/BaiLan_Util_SplitMatching_InstanceWork.h)

# add_library(cvAutoTrack_static STATIC ${cvAutoTrack_src})
#
# set_target_properties(cvAutoTrack_static PROPERTIES OUTPUT_NAME "cvAutoTrack")
#
# set_target_properties(cvAutoTrack PROPERTIES CLEAN_DIRECT_OUTPUT 1)
# set_target_properties(cvAutoTrack_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

#if( CMAKE_BUILD_TYPE STREQUAL Debug)
 target_link_libraries(cvAutoTrack C:/tmp/opencv_455/install/install_build_Shared_World/x64/vc16/lib/opencv_world455d.lib)
#endif()
#if(CMAKE_BUILD_TYPE STREQUAL Release)
  #target_link_libraries(cvAutoTrack C:/tmp/opencv_455/install/install_build_Shared_World/x64/vc16/lib/opencv_world455.lib)
#endif()