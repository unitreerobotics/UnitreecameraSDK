/**
  * @file example_getRectFrame.cc
  * @brief This file is part of UnitreeCameraSDK.
  * @details This example that how to Transmission picture
  * @author SunMingzhe
  * @date  2021.12.07
  * @version 1.1.0
  * @copyright Copyright (c) 2020-2021, Hangzhou Yushu Technology Stock CO.LTD. All Rights Reserved.
  */

/*
接收端：
方法概述：采用定向udp方法接收图片，在应用其之前，需要先发送图片，例子:example_putImagetrans.cc
端口9201~9205分别对应前方，下巴，左，右，腹部
本机ip需要是192.168.123.IpLastSegment，IpLastSegment被设置在发送端cofigure yaml-
*/

/*
listener
Introduction: This program uses directed UDP methods to get pictures,whitch requires sending pictures on other programs. for example:example_putImagetrans.cc
port:9201~9205 -> Front,chin,left,right,abdomen
local ip must be set to 192.168.123.IpLastSegment and IpLastSegment musb be set in cofigure yaml
*/

/*
local ip config
ip 192.168.123.IpLastSegment
netmask 255.255.255.0
gateway 192.168.123.1
*/



#include <opencv2/opencv.hpp>
#include <iostream>
int main(int argc,char** argv)
{
    std::string IpLastSegment = "15";
    int cam = 1;
    if (argc>=2)
        cam = std::atoi(argv[1]);
    std::string udpstrPrevData = "udpsrc address=192.168.123."+ IpLastSegment + " port=";
    //端口：前方，下巴，左，右，腹部
	std::array<int,5> udpPORT = std::array<int, 5>{9201, 9202, 9203, 9204, 9205};
    std::string udpstrBehindData = " ! application/x-rtp,media=video,encoding-name=H264 ! rtph264depay ! h264parse ! omxh264dec ! videoconvert ! appsink";
    std::string udpSendIntegratedPipe = udpstrPrevData +  std::to_string(udpPORT[cam-1]) + udpstrBehindData;
    std::cout<<"udpSendIntegratedPipe:"<<udpSendIntegratedPipe<<std::endl;
    cv::VideoCapture cap(udpSendIntegratedPipe);
    if(!cap.isOpened())
        return 0 ;
    cv::Mat frame;
    while(1)
    {
        cap >> frame;
        if(frame.empty())
            break;
        imshow("video", frame);
        cv::waitKey(20);
    }
    cap.release();//释放资源
    return 0;
}
