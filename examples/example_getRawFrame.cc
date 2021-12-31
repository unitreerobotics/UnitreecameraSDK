/**
  * @file example_getRawFrame.cc
  * @brief This file is part of UnitreeCameraSDK.
  * @details This example that how to get camera raw frame.
  * @author ZhangChunyang
  * @date  2021.07.31
  * @version 1.0.1
  * @copyright Copyright (c) 2020-2021, Hangzhou Yushu Technology Stock CO.LTD. All Rights Reserved.
  */

#include <UnitreeCameraSDK.hpp>
#include <unistd.h>

int main(int argc, char *argv[]){
    
    int deviceNode = 0; // default 0 -> /dev/video0
    cv::Size frameSize(1856, 800); // defalut image size: 1856 X 800
    int fps = 30;
    
    if(argc >= 2){
        deviceNode = std::atoi(argv[1]);
        if(argc >= 4){
            frameSize = cv::Size(std::atoi(argv[2]), std::atoi(argv[3]));
        }
        if(argc >=5)
            fps = std::atoi(argv[4]);
    }
    
    UnitreeCamera cam(deviceNode);  ///< init camera by device node number
    if(!cam.isOpened())
        exit(EXIT_FAILURE);
    
    cam.setRawFrameSize(frameSize); ///< set camera frame size
    cam.setRawFrameRate(fps);       ///< set camera frame rate
    
    std::cout << "Device Position Number:" << cam.getPosNumber() << std::endl;
    
    cam.startCapture();            ///< start camera capturing

    while(cam.isOpened())
    {
        
        cv::Mat frame;
        std::chrono::microseconds t;
        if(!cam.getRawFrame(frame, t)){ ///< get camera raw image
            usleep(1000);
            continue;
        }
 
        cv::Mat left,right;
        frame(cv::Rect(0, 0, frame.size().width/2, frame.size().height)).copyTo(right);
        frame(cv::Rect(frame.size().width/2,0, frame.size().width/2, frame.size().height)).copyTo(left);
        cv::hconcat(left, right, frame); 
        cv::imshow("UnitreeCamera_Left-Right", frame);
        char key = cv::waitKey(10);
        if(key == 27) // press ESC key
           break;
    }
    
    cam.stopCapture();  ///< stop camera capturing
    
    return 0;
}
