include_directories( "../includes" )
aux_source_directory(. DIR_LIB_SRCS)
# 生成链接库
add_library(gpiolibs ${DIR_LIB_SRCS})
