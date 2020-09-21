cmake_minimum_required (VERSION 2.8)
project(readGPIO)

#auto scan the file of this directory
aux_source_directory(. DIR_SRCS)

include_directories( "./includes" )
# 添加math子目录
add_subdirectory(lib)
# 指定生成目标
add_executable(readGPIO ${DIR_SRCS})
# 添加链接库
target_link_libraries(readGPIO gpiolibs)


message(${PROJECT_SOURCE_DIR})
