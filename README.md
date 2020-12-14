# RTK以及倾角传感器驱动安装和位姿计算
=======================================================

## RTK驱动安装和配置

安装步骤

```bash
mkdir & cd ros_rtk_driver
mkdir src
cd ..
catkin init
catkin config --cmake-args -DCMAKE_BUILD_TYPE=RelWithDebInfo
cd src
git clone https://github.com/swri-robotics/novatel_gps_driver
rosdep install . --from-paths -i
catkin build
```
如果出现未识别的消息格式，可以安装2进制驱动

```bash
sudo apt-get install ros-${ROS_DISTRO}-novatel-gps-driver
```
创建启动文件.launch

```bash
<?xml version="1.0"?>
<launch>
  <node name="novatel"
        pkg="nodelet" type="nodelet"
        args="standalone novatel_gps_driver/novatel_gps_nodelet">
    <rosparam>
      connection_type: serial
      device: /dev/ttyS0
      imu_sample_rate: 100
      publish_imu_messages: true
      frame_id: /gps
    </rosparam>
  </node>
</launch>
```





## bewis_ros_driver
北微传感器的 ros驱动，包括串口读取，解析，发布tf，订阅rtk topic等功能

安装步骤

```bash
mkdir & cd ros_angle_driver
mkdir src
cd src
gitclone https://github.com/Loricon/bewis_ros_driver.git
cd ..
catkin_make
source /build/setup.bash
```
运行
```bash
roscore
rosrun serialtest serialtest
```



