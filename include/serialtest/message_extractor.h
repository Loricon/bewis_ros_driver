#include "ros/ros.h"
#include <string>
#include <serial/serial.h>

#include "serialtest/sensor_value.h"
//#include "serialtest/angle_process.h"

serial::Serial ser[3];  //声明串口对象
using namespace std;
//#pragma once

//int sen_num1=0;
//double sensorx1=0;
//double sensory1=0;


std::string input;
const std::string packet_1 = "0x77";
const std::string packet_2 = "0xd";
int data_packet_start;
int data_packet_end;



void msg_sentence0(int);
void msg_sentence1(int);
void rx_message_extractor(const std::string);
void connect(std::string,int);
void DecodeIBEWISata(int * ,double * ,double * , unsigned char);
double convert16to10(unsigned char,unsigned char,int);



double convert16to10(unsigned char chr1,unsigned char chr2,int np)  //16进制转换为10进制
{
    
    int data1=int(chr1);
    int data2=int(chr2);


    double test2=(int(data1/16*10)+data1%16+double(data2/16*10+data2%16)/100)*np;

    return test2;
}



void DecodeIBEWISata(int * sensor_num,double *  anglex,double * angley, unsigned char chrTemp[])
{

    int np1=1;
    int np2=1;


    if(chrTemp[4]==0x10)
       np1=-1;
    if(chrTemp[7]==0x10)
       np2=-1;    

    *sensor_num =int(chrTemp[2]); 
    *anglex=convert16to10(chrTemp[5],chrTemp[6],np1);
    *angley=convert16to10(chrTemp[8],chrTemp[9],np2);


  //  printf("extracte data angle x------------: %f ------------angle y------------: %f\r\n",anglex,angley);


}



void rx_message_extractor(const std::string sentence)
{
    printf("%s  \r\n", sentence.c_str());
}

void connect(std::string port,int test)
{


    try
    {
        ser[test].setPort(port);
        ser[test].setBaudrate(115200);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser[test].setTimeout(to);
        ser[test].open();
    }
    catch (serial::IOException &e)
    {
        ROS_ERROR_STREAM("Unable to open port /dev/ttyUSB0");
        ros::Duration(5).sleep();
        return;
    }

    //检测串口是否已经打开，并给出提示信息
    if (ser[test].isOpen())
    {
        ROS_INFO_STREAM("Serial Port /dev/ttyUSB0 initialized");
    }
    else
    {
        ROS_INFO_STREAM("Could not initialize serial port /dev/ttyUSB0");
        return;
    }





}





void msg_sentence0(int test)
{

    //订阅主题，并配置回调函数
  //  ros::Subscriber write_sub = _nh->subscribe("write", 1000, write_callback);
    //发布主题
  //  ros::Publisher read_pub = _nh->advertise<std_msgs::String>("read", 1000);

    //指定循环的频率
  //  ros::Rate loop_rate(50);

    
    unsigned char chrBuffer[1000];
    unsigned char chrTemp[1000];


    unsigned short usLength = 0, usRxLength = 0;
    int dataLen = 14;
    unsigned char cheaksum;


    while (ros::ok())
    {

        //处理ROS的信息，比如订阅消息,并调用回调函数
        ros::spinOnce();

        size_t n = ser[test].available();

        if (ser[test].available())
        {

            usLength = ser[test].read(chrBuffer, 28);  //chrBuffer倾角传感器原始数据(数据长度为14)
/*

            printf("RAW LENGTH-----%d------senstence: ", usLength);


                        for (int i = 0; i < usLength; i++)
                        {
                            //16
                            printf("%x  ", chrBuffer[i] & 0xff);
                        }

                        cout << endl;

*/

            if (usLength > 0)
            {
                 usRxLength += usLength;  //放在内部会出错，造成数组后为无效值              


                while (usRxLength >= 14)   //循环内多次解包
                {

                    memcpy(chrTemp, chrBuffer, usRxLength);


                //      if (!((chrTemp[0] == 0x77) & (chrTemp[1] == 0xd) & usRxLength == 14))
                      if (!((chrTemp[0] == 0x77) & (chrTemp[1] == 0xd)))
                      {
                           for (int i = 1; i < usRxLength; i++) chrBuffer[i - 1] = chrBuffer[i];
                              usRxLength--;
                           continue;
                       }

/*

                    printf("data len : %d senstence  --------------: ",usRxLength);


                        for (int i = 0; i < usRxLength; i++)
                        {
                            //16
                            printf("%x  ", chrTemp[i] & 0xff);
                        }

                        cout << endl;
*/

                      if (usRxLength >=14) 
                      {
                         int num;
                         double  Anglex;
                         double  Angley;

                         DecodeIBEWISata(&num,&Anglex,&Angley,chrTemp);
                       //printf("port /dev/ttyUSB%d------extracte data angle x------: %f -------angle y--------: %f\r\n",test,Anglex,Angley);                         

                         sen_num1=num;
                         sensorx1=Anglex;
                         sensory1=Angley;

                          for (int i = 14; i < usRxLength; i++)
                            chrBuffer[i - 14] = chrBuffer[i];
                          usRxLength -= 14;
                       }

                }

            //   usRxLength += usLength;  //这里还有点问题，有空调整下合适位置

            }
        }

        else // 串口无读取
        {
          // ROS_INFO("Read sensor %d fail----!", test);
        }
    }
}

void msg_sentence1(int test)
{

    //订阅主题，并配置回调函数
  //  ros::Subscriber write_sub = _nh->subscribe("write", 1000, write_callback);
    //发布主题
  //  ros::Publisher read_pub = _nh->advertise<std_msgs::String>("read", 1000);

    //指定循环的频率
  //  ros::Rate loop_rate(50);

    
    unsigned char chrBuffer2[1000];
    unsigned char chrTemp2[1000];


    unsigned short usLength = 0, usRxLength = 0;
    int dataLen = 14;
    unsigned char cheaksum;


    while (ros::ok())
    {

        //处理ROS的信息，比如订阅消息,并调用回调函数
        ros::spinOnce();

        size_t n = ser[test].available();

        if (ser[test].available())
        {

            usLength = ser[test].read(chrBuffer2, 28);  //chrBuffer倾角传感器原始数据(数据长度为14)
     //       ROS_INFO("RAW LENGTH-----------------%d", usLength);

     //       usRxLength += usLength;

            if (usLength > 0)
            {
                 usRxLength += usLength;  //放在内部会出错，造成数组后为无效值              

    //            ROS_INFO("usRxLength-----------------%d", usRxLength);

                while (usRxLength >= 14)   //循环内多次解包
                {

           //     ROS_INFO("usRxLength-----------------%d", usRxLength);
                    memcpy(chrTemp2, chrBuffer2, usRxLength);


                //      if (!((chrTemp[0] == 0x77) & (chrTemp[1] == 0xd) & usRxLength == 14))
                      if (!((chrTemp2[0] == 0x77) & (chrTemp2[1] == 0xd)))
                      {
                           for (int i = 1; i < usRxLength; i++) chrBuffer2[i - 1] = chrBuffer2[i];
                              usRxLength--;
                           continue;
                       }

/*

                    printf("senstence len ----------%d-----------: usRxLength");


                        for (int i = 0; i < usRxLength; i++)
                        {
                            //16
                            printf("%x  ", chrTemp2[i] & 0xff);
                        }

                        cout << endl;
*/

                      if (usRxLength >=14) 
                      {

                        int num;
                        double  Anglex;
                        double  Angley;

                          DecodeIBEWISata(&num,&Anglex,&Angley,chrTemp2);
                       //printf("port /dev/ttyUSB%d------extracte data angle x------: %f -------angle y--------: %f\r\n",test,Anglex,Angley);    
                     
                         sen_num2=num;
                         sensorx2=Anglex;
                         sensory2=Angley;

                         for (int i = 14; i < usRxLength; i++)
                            chrBuffer2[i - 14] = chrBuffer2[i];
                         usRxLength -= 14;
                       }

                }
          //      usRxLength += usLength;  //这里还有点问题，有空调整下合适位置

            }
        }

        else // 串口无读取
        {
         //  ROS_INFO("Read sensor %d fail----!", test);
         
        }
    }
}


void msg_sentence2(int test)
{

    //订阅主题，并配置回调函数
  //  ros::Subscriber write_sub = _nh->subscribe("write", 1000, write_callback);
    //发布主题
  //  ros::Publisher read_pub = _nh->advertise<std_msgs::String>("read", 1000);

    //指定循环的频率
  //  ros::Rate loop_rate(50);

    
    unsigned char chrBuffer3[1000];
    unsigned char chrTemp3[1000];


    unsigned short usLength = 0, usRxLength = 0;
    int dataLen = 14;
    unsigned char cheaksum;


    while (ros::ok())
    {

        //处理ROS的信息，比如订阅消息,并调用回调函数
        ros::spinOnce();

        size_t n = ser[test].available();

        if (ser[test].available())
        {

            usLength = ser[test].read(chrBuffer3, 28);  //chrBuffer倾角传感器原始数据(数据长度为14)
     //       ROS_INFO("RAW LENGTH-----------------%d", usLength);

     //       usRxLength += usLength;

            if (usLength > 0)
            {
                 usRxLength += usLength;  //放在内部会出错，造成数组后为无效值              

    //            ROS_INFO("usRxLength-----------------%d", usRxLength);

                while (usRxLength >= 14)   //循环内多次解包
                {

           //     ROS_INFO("usRxLength-----------------%d", usRxLength);
                    memcpy(chrTemp3, chrBuffer3, usRxLength);


                //      if (!((chrTemp[0] == 0x77) & (chrTemp[1] == 0xd) & usRxLength == 14))
                      if (!((chrTemp3[0] == 0x77) & (chrTemp3[1] == 0xd)))
                      {
                           for (int i = 1; i < usRxLength; i++) chrBuffer3[i - 1] = chrBuffer3[i];
                              usRxLength--;
                           continue;
                       }

/*

                    printf("senstence len ----------%d-----------: usRxLength");


                        for (int i = 0; i < usRxLength; i++)
                        {
                            //16
                            printf("%x  ", chrTemp2[i] & 0xff);
                        }

                        cout << endl;
*/

                      if (usRxLength >=14) 
                      {

                        int num;
                        double  Anglex;
                        double  Angley;

                          DecodeIBEWISata(&num,&Anglex,&Angley,chrTemp3);
                       //printf("port /dev/ttyUSB%d------extracte data angle x------: %f -------angle y--------: %f\r\n",test,Anglex,Angley);    
                     
                         sen_num3=num;
                         sensorx3=Anglex;
                         sensory3=Angley;

                         for (int i = 14; i < usRxLength; i++)
                            chrBuffer3[i - 14] = chrBuffer3[i];
                         usRxLength -= 14;
                       }

                }
          //      usRxLength += usLength;  //这里还有点问题，有空调整下合适位置

            }
        }

        else // 串口无读取
        {
        //  ROS_INFO("Read sensor %d fail----!", test);
        }
    }
}




