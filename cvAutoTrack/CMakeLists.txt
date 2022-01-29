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

#add_library(cvAutoTrack STATIC ${cvAutoTrack_src})

add_library(cvAutoTrack SHARED ${cvAutoTrack_src} Task/TaskFacility.h Task/TaskInstance.h Task/Task_HookHandle.h GenshinFunction/HookHandle/HookHandle.h GenshinFunction/HookHandle/HookHandle.cpp Task/Task_HookHandle.cpp GenshinFunction/Screenshot/Screenshot.cpp GenshinFunction/Screenshot/Screenshot.h GenshinFunction/Flow/FlowFacility.cpp GenshinFunction/Flow/FlowFacility.h GenshinFunction/HookHandle/HookerFacility.h GenshinFunction/HookHandle/Hooker_TengXunYunYouXi.h GenshinFunction/HookHandle/Hooker_YuanShen.h Task/Task_Screenshot.h GenshinFunction/WorkFacility.h "Data/DataFacility.h" "Data/Data_GenshinImpact.h" "Data/DataTypeBase.h" "GenshinFunction/WorkNode.h" Logger/LoggerError.h Task/Task_Screenshot.cpp GenshinFunction/SplitDispense/SplitDispense.cpp GenshinFunction/SplitDispense/SplitDispense.h Task/Task_SplitDispense.cpp Task/Task_SplitDispense.h Task/Task_Spilt.cpp Task/Task_Spilt.h)

# add_library(cvAutoTrack_static STATIC ${cvAutoTrack_src})
#
# set_target_properties(cvAutoTrack_static PROPERTIES OUTPUT_NAME "cvAutoTrack")
#
# set_target_properties(cvAutoTrack PROPERTIES CLEAN_DIRECT_OUTPUT 1)
# set_target_properties(cvAutoTrack_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

target_link_libraries(cvAutoTrack C:/tmp/opencv_455/install/install_build_Shared_World/x64/vc16/lib/opencv_world455d.lib)
