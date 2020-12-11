#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <std_srvs/Empty.h>
#include <string>
#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>
#include <geometry_msgs/Quaternion.h>
#include <ctime>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <math.h>
#include "serialtest/sensor_value.h"

int sen_num1=0;
double sensorx1=0;
double sensory1=0;

int sen_num2=0;
double sensorx2=0;
double sensory2=0;

int sen_num3=0;
double sensorx3=0;
double sensory3=0;

using namespace Eigen;
using namespace std;


void write_callback(const std_msgs::String,int);
void sensor_data(int argc, char **argv);


double cal_digger_angle(double *,double *,int num);

double cal_digger_angle(double *roll,double *pitch,int num)
{


        double angle=0;

        double pitch0=pitch[0];
        double roll0=roll[0];
        double pitch1=pitch[1];
        double roll1=roll[1];

        int np=1;
        if(pitch0*roll0 < 0)
          np=-1;

        int np2=1;
        if(pitch1*roll1 < 0)
          np2=-1;


      return angle=sqrt(pitch0*pitch0+roll0*roll0)*np-sqrt(pitch1*pitch1+roll1*roll1)*np2;

      //  return angle=sqrt(pitch0*pitch0+roll0*roll0);
      //  return angle=pitch0/cos(roll0/57.3);



/*  旋转由于不可交换性的特点，下面的计算无法获得正确的旋转信息
 
         pitch[0]=pitch[0]/57.3;
         roll[0]=roll[0]/57.3;
         pitch[1]=pitch[1]/57.3;
         roll[1]=roll[1]/57.3;



        Matrix<double,3,3> Rot_pitch1,Rot_roll1,Rot_pitch2,Rot_roll2;
      
        Matrix<double,3,1> rot1,rot2;

        rot1 << 0,0,1;
        rot2 << 0,1,0;

        Rot_pitch1 << cos(pitch[0]),0,sin(pitch[0]),0,1,0,-sin(pitch[0]),0,cos(pitch[0]);
        Rot_roll1 << 1,0,0,0,cos(roll[0]),-sin(roll[0]),0,sin(roll[0]),cos(roll[0]);
        Rot_pitch2 << cos(pitch[1]),0,sin(pitch[1]),0,1,0,-sin(pitch[1]),0,cos(pitch[1]);
        Rot_roll2 << 1,0,0,0,cos(roll[1]),-sin(roll[1]),0,sin(roll[1]),cos(roll[1]);

 //       Vector3d n_ref = Rot_pitch1*Rot_roll1*rot1;
//        Vector3d n_tar = Rot_pitch2*Rot_roll2*rot1;

        Vector3d n_ref =Rot_roll1*Rot_pitch1*rot1;
        Vector3d n_tar =Rot_roll2*Rot_pitch2*rot1;

        Vector3d cross_tr=n_ref.cross(n_tar);   //n_ref和n_tar的叉积（法向量）

        double dot_rt= cross_tr.dot(rot1);     //cross_tr和rot1的点积

        double dot_cr= n_ref.dot(n_tar);    
        double B_ang=acos(dot_cr);

        int np=1;
        if(dot_rt < 0)
          np=-1;

        angle = np*B_ang*57.3;


*/



}



//回调函数
void write_callback(const std_msgs::String::ConstPtr &msg,int test)
{
    ROS_INFO_STREAM("Writing to serial port" << msg->data);
    ser[test].write(msg->data); //发送串口数据
}



void sensor_data(int argc, char **argv)
{

 //   int argc;
 //   char **argv;  //加上吧，虽然没用
    //初始化节点
    ros::init(argc, argv, "sensor_read");
    //声明节点句柄
    ros::NodeHandle nh;

    ros::Rate r(50); // 调整频率 hz
    while (ros::ok())
    {


         double digger_angle;

         double pitch1=sensorx1;     
         double pitch2=sensorx2; 

         double roll1 =sensory1;     
         double roll2=sensory2; 

         double roll[2]={roll1,roll2};
         double pitch[2]={pitch1,pitch2};

         digger_angle=cal_digger_angle(roll,pitch,2);

         printf("extern data sensor:----Sensor number: %d  ---Angle_x: %3.3f  ---Angle_y:  %3.3f\r\n",sen_num1,sensorx1,sensory1);
         printf("extern data sensor:----Sensor number: %d  ---Angle_x: %3.3f  ---Angle_y:  %3.3f\r\n",sen_num2,sensorx2,sensory2);
         printf("extern data sensor:----Sensor number: %d  ---Angle_x: %3.3f  ---Angle_y:  %3.3f\r\n",sen_num3,sensorx3,sensory3);

         printf("digger_angle:----Sensor number: %3.3f  \r\n",digger_angle);






         r.sleep();

    }


}

