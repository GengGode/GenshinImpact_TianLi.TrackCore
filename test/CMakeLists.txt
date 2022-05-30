cmake_minimum_required(VERSION 3.21)
project(cvAutoTrackTest)

# Add the cvAutoTrack library
include_directories(${CMAKE_SOURCE_DIR}/cvAutoTrack)
# 添加源文件目录
aux_source_directory(. test_src)

add_executable(cvAutoTrackTest ${test_src})

 target_link_libraries(cvAutoTrackTest PRIVATE
         debug ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug/cvAutoTrack.lib
         optimized ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release/cvAutoTrack.lib
         )


