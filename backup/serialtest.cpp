#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <std_srvs/Empty.h>
#include <string>
#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>
#include <geometry_msgs/Quaternion.h>
#include <pthread.h>

serial::Serial ser; //声明串口对象
#define BASE_CALLBACK_PACKAGE_LENGTH 100

std::string input;
const std::string packet_1 = "0x77";
const std::string packet_2 = "0xd";
int data_packet_start;
int data_packet_end;

using namespace std;

#define NUM_THREADS 2  //线程数，每个传感器一个

double convert16to10(unsigned char chr1,unsigned char chr2,int np)  //16进制转换为10进制
{
    
    int data1=int(chr1);
    int data2=int(chr2);


    double test2=(int(data1/16*10)+data1%16+double(data2/16*10+data2%16)/100)*np;

    return test2;
}



void DecodeIBEWISata(double *  anglex,double * angley, unsigned char chrTemp[])
{

    int np1=1;
    int np2=1;


    if(chrTemp[4]==0x10)
       np1=-1;
    if(chrTemp[7]==0x10)
       np2=-1;    



    *anglex=convert16to10(chrTemp[5],chrTemp[6],np1);
    *angley=convert16to10(chrTemp[8],chrTemp[9],np2);


  //  printf("extracte data angle x------------: %f ------------angle y------------: %f\r\n",anglex,angley);


}



//回调函数
void write_callback(const std_msgs::String::ConstPtr &msg)
{
    ROS_INFO_STREAM("Writing to serial port" << msg->data);
    ser.write(msg->data); //发送串口数据
}

void rx_message_extractor(const std::string sentence)
{
    printf("%s  \r\n", sentence.c_str());
}

void connect()
{

    try
    {
        ser.setPort("/dev/ttyUSB2");
        ser.setBaudrate(115200);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();
    }
    catch (serial::IOException &e)
    {
        ROS_ERROR_STREAM("Unable to open port /dev/ttyUSB0");
        ros::Duration(5).sleep();
        return;
    }

    //检测串口是否已经打开，并给出提示信息
    if (ser.isOpen())
    {
        ROS_INFO_STREAM("Serial Port /dev/ttyUSB0 initialized");
    }
    else
    {
        ROS_INFO_STREAM("Could not initialize serial port /dev/ttyUSB0");
        return;
    }
}

void msg_sentence(ros::NodeHandle *_nh)
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

        size_t n = ser.available();

        if (ser.available())
        {

            usLength = ser.read(chrBuffer, 14);  //chrBuffer倾角传感器原始数据(数据长度为14)
            ROS_INFO("RAW LENGTH-----------------%d", usLength);



            if (usLength > 0)
            {
                usRxLength += usLength;

    //            ROS_INFO("usRxLength-----------------%d", usRxLength);

                while (usRxLength >= 14)   //循环内多次解包
                {

            //    ROS_INFO("usRxLength-----------------%d", usRxLength);
                    memcpy(chrTemp, chrBuffer, usRxLength);


                //      if (!((chrTemp[0] == 0x77) & (chrTemp[1] == 0xd) & usRxLength == 14))
                      if (!((chrTemp[0] == 0x77) & (chrTemp[1] == 0xd)))
                      {
                           for (int i = 1; i < usRxLength; i++) chrBuffer[i - 1] = chrBuffer[i];
                              usRxLength--;
                           continue;
                       }



                    printf("senstence  ---------------------: ");



                        for (int i = 0; i < usRxLength; i++)
                        {
                            //16
                            printf("%x  ", chrTemp[i] & 0xff);
                        }

                        cout << endl;


                      double  Anglex;
                      double  Angley;

                      DecodeIBEWISata(&Anglex,&Angley,chrTemp);
                      printf("extracte data angle x------------: %f ------------angle y------------: %f\r\n",Anglex,Angley);                         



                       for (int i = 14; i < usRxLength; i++)
                         chrBuffer[i - 14] = chrBuffer[i];
                       usRxLength -= 14;


                }



            }
        }

        else // 串口无读取
        {
        }
    }
}



// 线程的运行函数
void* read_serial_port(ros::NodeHandle nh,std::string port)
{
    connect();
    msg_sentence(&nh);
    return 0;
}


int main(int argc, char **argv)
{
    //初始化节点
    ros::init(argc, argv, "serial_example_node");
    //声明节点句柄
    ros::NodeHandle nh;
//    connect();
//    msg_sentence(&nh);

    std::string port[2]={"/dev/ttyUSB0","/dev/ttyUSB2"};
//    std::string port0="/dev/ttyUSB0";


    pthread_t tids[NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; ++i)
    {
        //参数依次是：创建的线程id，线程参数，调用的函数，传入的函数参数
        int ret = pthread_create(&tids[i], NULL, read_serial_port, port[i]);
        if (ret != 0)
        {
           cout << "pthread_create error: error_code=" << ret << endl;
        }
    }
    //等各个线程退出后，进程才结束，否则进程强制结束了，线程可能还没反应过来；
    pthread_exit(NULL);









}
