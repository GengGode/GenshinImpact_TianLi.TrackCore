cmake_minimum_required(VERSION 3.21)
project(cvAutoTrackTest)

aux_source_directory(. test_src)

add_executable(cvAutoTrackTest ${test_src})

target_link_libraries(cvAutoTrackTest cvAutoTrack)


include_directories(${CMAKE_SOURCE_DIR}/cvAutoTrack)
