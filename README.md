# bewis_ros_driver
北微传感器的 ros驱动，包括串口读取，解析，发布tf，订阅rtk topic等功能

安装步骤

mkdir & cd catkin_ws
mkdir src
cd src
gitclone https://github.com/ros-drivers/nmea_comms.git
cd ..
catkin_make
source /build/setup.bash

运行

rosrun serialtest serialtest
