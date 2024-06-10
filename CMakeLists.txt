cmake_minimum_required(VERSION 3.1)
project(mypvz)             #项目名程

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_SUB)   #子目录
# aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_CUR)     #当前目录
# file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)


# 添加编译选项，如-g -std=c++17 -O2
add_compile_options(-Wno-conversion-null)    #忽略警告


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)   #包含头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/easyx)     #easyx头文件目录

set(CMAKE_CXX_STANDARD 11)

enable_language(C)
enable_language(CXX)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

#链接库路径
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)


#链接静态库
#link_libraries(EasyXa)
#link_libraries(EasyXw)
link_libraries(Uuid)

# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/output)
add_executable(mypvz ${SRC_SUB} ${SRC_CUR} main.cpp)            #生成可执行文件

#链接动态库
target_link_libraries(mypvz winmm msimg32 gdi32 imm32 oleaut32 ole32 GdiPlus easyx)