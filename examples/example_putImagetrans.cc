/**
  * @file example_getRectFrame.cc
  * @brief This file is part of UnitreeCameraSDK.
  * @details This example that how to Transmission picture
  * @author SunMingzhe
  * @date  2021.12.07
  * @version 1.1.0
  * @copyright Copyright (c) 2020-2021, Hangzhou Yushu Technology Stock CO.LTD. All Rights Reserved.
  */

#include <UnitreeCameraSDK.hpp>
#include <unistd.h>


/*
发送端：
方法概述：采用udp方法输出图片，传输方式已写入程序，由StereoCameraCommon::startCapture(true,false)中第一个参数控制定向udp传输，当其为true时打开udp264硬编码传输模式
参数：配置文件中IpLastSegment是接收端ip，接收端地址必须是192.168.123.IpLastSegment。
     配置文件中Transmode决定传输模式
     配置文件中Transrate决定传输速率，限制了传输速率小于图片速率
     端口9201~9205分别对应前方，下巴，左，右，腹部
     启动自定义图传时需使用kill.sh文件关闭Unitree/autostart/02camerarosnode程序来解除相机占用，Unitree/autostart/04imageai来释放算力。
     （也可使用以下几条指令
     ps -A | grep point | awk '{print $1}' | xargs kill -9
     ps -aux|grep mqttControlNode|grep -v grep|head -n 1|awk '{print $2}'|xargs kill -9
     ps -aux|grep live_human_pose|grep -v grep|head -n 1|awk '{print $2}'|xargs kill -9
     ）
传输图片模式：由配置文件中Transmode决定，该值只可通过读取配置文件方式写入
0：传输左目原始图片
1：传输双目原始图片
2：传输左目畸变校正后图片，视野范围角度可调60~140
3：传输双目畸变校正后图片
※※※注意，双目图的最后的左右目输出位置是反的。左目->右图，右目->左图
4：不建议使用！！，Depthmode = 2时 可输出深度图和对应左目图片，不建议使用。
*/

/*
sender:
Introduction: This program uses directed UDP methods to output pictures, transfer method written in programs.  StereoCameraCommon::startCapture(true,false) the first parameter is a switch.
parameter:IpLastSegment is listener ip. listener ip:192.168.123.IpLastSegment
     Transrate(config yaml) Transmission rate.it must lower than FrameRate
     port:9201~9205 -> Front,chin,left,right,abdomen
     If you start this file, you must bash "kill.sh" to turn off automatic startup program.  "Unitree/autostart/02camerarosnode/kill.sh" and "Unitree/autostart/04imageai/kill.sh".
     （You can also use the following instructions:
     ps -A | grep point | awk '{print $1}' | xargs kill -9
     ps -aux|grep mqttControlNode|grep -v grep|head -n 1|awk '{print $2}'|xargs kill -9
     ps -aux|grep live_human_pose|grep -v grep|head -n 1|awk '{print $2}'|xargs kill -9
     ）
Transfer picture mode: Decided by Transmode in the configuration yaml, which only writes in programs by reading the configuration yaml.
0：ori left
1：ori stereo
2：rect left
3：rect stereo
※※※warning:The last left and right output position of the stereo picture is reversed    left cam-> right picture      right cam -> left picture
4：Not recommended for use!!，set Depthmode = 2  , rect left && depthimage
*/

/*
local ip config
ip 192.168.123.x
netmask 255.255.255.0
gateway 192.168.123.1
*/
int main(int argc, char *argv[])
{
   
    UnitreeCamera cam("trans_rect_config.yaml"); ///< init camera by device node number
    if(!cam.isOpened())   ///< get camera open state
        exit(EXIT_FAILURE);   
    cam.startCapture(true,false); ///< disable share memory sharing and able image h264 encoding

    usleep(500000);
    while(cam.isOpened())
    {
        cv::Mat left,right,feim;
        if(!cam.getRectStereoFrame(left,right))
        {
            usleep(1000);
            continue;
        }
        char key = cv::waitKey(10);
        if(key == 27) // press ESC key
           break;
    }
    
    cam.stopCapture(); ///< stop camera capturing
    
    return 0;
}
/*
int main(int argc, char *argv[])
{
     
    UnitreeCamera cam("trans_rect_config.yaml"); ///< init camera by device node number
    if(!cam.isOpened())   ///< get camera open state
        exit(EXIT_FAILURE);
    
    cam.startCapture(true,false); ///< disable share memory sharing and able image h264 encoding

    usleep(500000);
    while(cam.isOpened())
    {
        cv::Mat frame;
        std::chrono::microseconds t;
        if(!cam.getRawFrame(frame, t)){ ///< get camera raw image
            usleep(1000);
            continue;
        }
        char key = cv::waitKey(10);
        if(key == 27) // press ESC key
           break;
    }
    
    cam.stopCapture(); ///< stop camera capturing
    
    return 0;
}
*/
