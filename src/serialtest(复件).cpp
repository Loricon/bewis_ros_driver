#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <std_srvs/Empty.h>
#include <string>
#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>
#include <geometry_msgs/Quaternion.h>

serial::Serial ser; //声明串口对象
#define BASE_CALLBACK_PACKAGE_LENGTH 100

std::string input;
const std::string packet_start = "$GPGGA";
const std::string packet_end = "\r\n";
int data_packet_start;
int data_packet_end;

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
        ser.setPort("/dev/ttyUSB0");
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
    ros::Subscriber write_sub = _nh->subscribe("write", 1000, write_callback);
    //发布主题
    ros::Publisher read_pub = _nh->advertise<std_msgs::String>("read", 1000);

 

    //指定循环的频率
    ros::Rate loop_rate(50);

    while (ros::ok())
    {

        //处理ROS的信息，比如订阅消息,并调用回调函数
        ros::spinOnce();

       std::string readMsg;

       // char readMsg[100];

        if (ser.available())
        {

            unsigned char recvDispose[BASE_CALLBACK_PACKAGE_LENGTH];
            readMsg = ser.read(ser.available());
            ROS_INFO("RAW DATA-----------------[%X]", readMsg.c_str());
            input += readMsg;

            while (input.length() >= BASE_CALLBACK_PACKAGE_LENGTH) // 大于BASE_CALLBACK_PACKAGE_LENGTH的长度可能有完整数据
            {
                // parse for data packets
                data_packet_start = input.find(packet_start);
                data_packet_end = input.find(packet_end);
                int data_len = data_packet_end - data_packet_start;
                if (data_packet_start > data_packet_end)
                {
                    input.erase(0, data_packet_end + 2); // 这种条件下删除data_packet_start标识
                }

                else
                {

                    if (data_packet_start != std::string::npos && data_packet_end != std::string::npos && data_packet_start < data_packet_end)
                    {
                        ROS_INFO("found possible  data packet at position_-------------- %d------------%d-----------%d\n", data_packet_start, data_packet_end, input.length());
                        ROS_INFO("input**********8::   %s", input.c_str());
 

                        //暂时不写校验程序


                            std::string sentence;
                            memcpy(recvDispose, input.c_str() + data_packet_start, data_len);
                            ROS_INFO("...right data is : ");


                            sentence=(char*)recvDispose;
                            /*帧数据处理函数*/


                            rx_message_extractor(sentence);


                            std_msgs::String result;
                            read_pub.publish(result);

                            input.erase(0, data_packet_start + data_len); // 在input中删除已经处理的数据
                      
                    }
                    else
                    {
                        // 找不到有效数据，清除input
                        ROS_INFO("No start character found in input \n");
                        input.clear();
                    }
                }


            }
            loop_rate.sleep();
        }
    }

}









int main(int argc, char **argv)
{
    //初始化节点
    ros::init(argc, argv, "serial_example_node");
    //声明节点句柄
    ros::NodeHandle nh;
    connect();
    msg_sentence(&nh);

}
