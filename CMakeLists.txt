
cmake_minimum_required (VERSION 3.8)

project("Kinect_Accessor")

#Kinect
include_directories("$ENV{KINECTSDK20_DIR}/inc")
set(KINECT_LIB_DIR "$ENV{KINECTSDK20_DIR}/Lib/x86")
link_libraries(
"${KINECT_LIB_DIR}/Kinect20.Face.lib" 
"${KINECT_LIB_DIR}/Kinect20.Fusion.lib" 
"${KINECT_LIB_DIR}/Kinect20.lib" 
"${KINECT_LIB_DIR}/Kinect20.VisualGestureBuilder.lib"
)

# 将源添加到此项目的可执行文件。
add_executable (kinect_accessor "src/kinect_accessor.cpp" "src/kinect_accessor.h") 
