# RTK以及倾角传感器驱动安装和位姿计算
=======================================================

## 前置操作

获取串口权限(USB虚拟串口)

创建文件/etc/udev/rules.d/70-ttyusb.rules
```bash
sudo gedit /etc/udev/rules.d/70-ttyusb.rules
```
把以下内容复制到文件并保存(非虚拟串口修改对应内容即可获取权限)
```bash
KERNEL==”ttyUSB[0-9]*”, MODE=”0666”
```
插入串口设备，自动获取权限


## 安装UTM WGS84坐标系转换

ROS - Python and C++ interfaces for manipulating geodetic coordinates

```bash
sudo apt-get install ros-melodic-geodesy
```



## RTK驱动安装和配置

安装步骤

```bash
mkdir & cd ros_rtk_driver
mkdir src
cd ..
catkin init
catkin config --cmake-args -DCMAKE_BUILD_TYPE=RelWithDebInfo
cd src
git clone https://github.com/Loricon/bewis_ros_driver.git
rosdep install . --from-paths -i
catkin build
```
如果出现未识别的消息格式，可以安装2进制驱动(无法二次开发，不建议)

```bash
sudo apt-get install ros-${ROS_DISTRO}-novatel-gps-driver
```

（注意source或加入.bashrc）

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


提示：编译过程中出现未发现依赖未安装或缺少某文件时，建议运行rosdep install . --from-paths -i






