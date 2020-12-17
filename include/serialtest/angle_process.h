#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <std_srvs/Empty.h>
#include <string>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <cstdio>
#include <tf2/LinearMath/Quaternion.h>
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


double d1=0.09;  //上车体原点到动臂
double a2=5.681;  //动臂
double a3=2.925;  //斗杆
double a4=1.494;   //铲斗

#define PI 3.14159



void write_callback(const std_msgs::String,int);

void sensor_data(int argc, char **argv);

double cal_digger_angle(double *,double *,int num);


void tf2_publisher(double *position2,double *posture2,double * angle,double * partLength,tf2_ros::TransformBroadcaster br);

void tf2_publisher(double *position2,double *posture2,double * angle,double * partLength,tf2_ros::TransformBroadcaster br)
{

          
          geometry_msgs::TransformStamped odom2baselink,baselink2dongbi,dongbi2dougan,dougan2chandou;

	  tf2::Quaternion q_odom2baselink,q_baselink2dongbi,q_dongbi2dougan,q_dougan2chandou;

// odom--------->baselink

	  odom2baselink.header.frame_id = "odom";
	  odom2baselink.child_frame_id = "baselink";


	  odom2baselink.transform.translation.x = position2[0];
	  odom2baselink.transform.translation.y = position2[1];
	  odom2baselink.transform.translation.z = position2[2];


	  q_odom2baselink.setRPY(posture2[0], posture2[1], posture2[2]);
	  odom2baselink.transform.rotation.x = q_odom2baselink.x();
	  odom2baselink.transform.rotation.y = q_odom2baselink.y();
	  odom2baselink.transform.rotation.z = q_odom2baselink.z();
	  odom2baselink.transform.rotation.w = q_odom2baselink.w();


         odom2baselink.header.stamp = ros::Time::now();
         br.sendTransform(odom2baselink);





// baselink--------->动臂
	  baselink2dongbi.header.frame_id = "baselink";
	  baselink2dongbi.child_frame_id = "dongbi";


	  baselink2dongbi.transform.translation.x = partLength[0];
	  baselink2dongbi.transform.translation.y = 0.0;
	  baselink2dongbi.transform.translation.z = 0.0;


	  q_baselink2dongbi.setRPY(0, angle[0], 0);
	  baselink2dongbi.transform.rotation.x = q_baselink2dongbi.x();
	  baselink2dongbi.transform.rotation.y = q_baselink2dongbi.y();
	  baselink2dongbi.transform.rotation.z = q_baselink2dongbi.z();
	  baselink2dongbi.transform.rotation.w = q_baselink2dongbi.w();


         baselink2dongbi.header.stamp = ros::Time::now();
         br.sendTransform(baselink2dongbi);

// 动臂--------->斗杆
	  dongbi2dougan.header.frame_id = "dongbi";
	  dongbi2dougan.child_frame_id = "dougan";


	  dongbi2dougan.transform.translation.x = partLength[1];
	  dongbi2dougan.transform.translation.y = 0.0;
	  dongbi2dougan.transform.translation.z = 0.0;

	  q_dongbi2dougan.setRPY(0, angle[1], 0);
	  dongbi2dougan.transform.rotation.x = q_dongbi2dougan.x();
	  dongbi2dougan.transform.rotation.y = q_dongbi2dougan.y();
	  dongbi2dougan.transform.rotation.z = q_dongbi2dougan.z();
	  dongbi2dougan.transform.rotation.w = q_dongbi2dougan.w();


         dongbi2dougan.header.stamp = ros::Time::now();
         br.sendTransform(dongbi2dougan);

// 斗杆--------->铲斗
	  dougan2chandou.header.frame_id = "dougan";
	  dougan2chandou.child_frame_id = "chandou";


	  dougan2chandou.transform.translation.x = partLength[2];
	  dougan2chandou.transform.translation.y = 0.0;
	  dougan2chandou.transform.translation.z = 0.0;



	  q_dougan2chandou.setRPY(0, angle[2], 0);
	  dougan2chandou.transform.rotation.x = q_dougan2chandou.x();
	  dougan2chandou.transform.rotation.y = q_dougan2chandou.y();
	  dougan2chandou.transform.rotation.z = q_dougan2chandou.z();
	  dougan2chandou.transform.rotation.w = q_dougan2chandou.w();


         dougan2chandou.header.stamp = ros::Time::now();
         br.sendTransform(dougan2chandou);
}





















void position_posture(double * position,double * posture)
{
   






}









void cal_two_parts_tf(double angle,int part);   //计算机构相对位姿关系，即tf转换关系并发布，odom是否为特殊项待讨论

void cal_two_parts_tf(double angle,int part)
{


        
        //part1  上车体到动臂

       angle=angle*PI/180;
       double ca=cos(angle);
       double sa=sin(angle);



       Matrix<double,4,4> trans;
  //     trans << ca,-sa,0,  

    


}




double cal_digger_angle(double *roll,double *pitch,int num)
{


        double angle=0;

        double pitch0=pitch[0];
        double roll0=roll[0];
        double pitch1=pitch[1];
        double roll1=roll[1];
/*
        int np=1;
        if(pitch0*roll0 < 0)
          np=-1;

        int np2=1;
        if(pitch1*roll1 < 0)
          np2=-1;

*/
        int np=1;
        if(pitch0 < 0)
          np=-1;

        int np2=1;
        if(pitch1 < 0)
          np2=-1;





      return angle=sqrt(pitch0*pitch0+roll0*roll0)*np-sqrt(pitch1*pitch1+roll1*roll1)*np2;

      //  return angle=sqrt(pitch0*pitch0+roll0*roll0);
      //  return angle=pitch0/cos(roll0/57.3);

    //  position,double *posture
   


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
    ros::init(argc, argv, "bewis_sensor_read");
    //声明节点句柄
    ros::NodeHandle nh;

    tf2_ros::TransformBroadcaster br;

    ros::Rate r(50); // 调整频率 hz
    while (ros::ok())
    {


         double digger_angle;

         double pitch1=sensorx1;     
         double pitch2=sensorx2; 
         double pitch3=sensorx2; 

         double roll1 =sensory1;     
         double roll2=sensory2; 
         double roll3=sensory2; 



         double roll[2]={roll1,roll2};
         double pitch[2]={pitch1,pitch2};

         digger_angle=cal_digger_angle(roll,pitch,2);

         printf("extern data sensor:----Sensor number: %d  ---Angle_x: %3.3f  ---Angle_y:  %3.3f\r\n",sen_num1,sensorx1,sensory1);
         printf("extern data sensor:----Sensor number: %d  ---Angle_x: %3.3f  ---Angle_y:  %3.3f\r\n",sen_num2,sensorx2,sensory2);
         printf("extern data sensor:----Sensor number: %d  ---Angle_x: %3.3f  ---Angle_y:  %3.3f\r\n",sen_num3,sensorx3,sensory3);

         printf("digger_angle:----Sensor number: %3.3f  \r\n",digger_angle);

         digger_angle=digger_angle/57.3;



         double angleP[3]={digger_angle,digger_angle,digger_angle};
         double partLengthP[3]={5.7,6.8,1.8};

         double position[3]={0,0,0};  //rtk位置（转换为m）
         double posture[3]={0,0,0};   //rtk姿态

         tf2_publisher(position,posture,angleP,partLengthP,br);



         r.sleep();

    }


}

