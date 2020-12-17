#include <ros/ros.h>
#include <serial/serial.h>
#include <pthread.h>
#include <string>
#include <thread>
#include "serialtest/message_extractor.h"
#include "serialtest/angle_process.h"
#include "serialtest/sensor_value.h"

//#include <vector.h>/
//#pragma once
using namespace std;

//#define NUM_THREADS 2  //线程数，每个传感器一个


// 线程的运行函数
void* read_serial_port(std::string port,int test)
{
    connect(port,test);

    if (test == 0)
    {
      msg_sentence0(test);
    }
    else if (test == 1)
    {
      msg_sentence1(test);
    }
    else if (test == 2)
    {
      msg_sentence2(test);
    }
    return 0;
}



int main(int argc, char **argv)
{


    /***************************************************************************************/
    std::string port0="/dev/ttyUSB0";

    array<std::string,3> port={"/dev/ttyUSB0","/dev/ttyUSB1","/dev/ttyUSB2"};
   //  connect(port[1]);
   //  msg_sentence();

    int test1=0;
    int test2=1;
    int test3=2;
    thread th1(read_serial_port,port[0],test1);
    thread th2(read_serial_port,port[1],test2);
  //  thread th4(read_serial_port,port[2],test3);
    thread th3(sensor_data,argc,argv);
    th1.join(); 
    th2.join(); 
    th3.join();   
   // th4.join();




}
