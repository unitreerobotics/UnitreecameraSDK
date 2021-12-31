/**
  * @file example_getCalibParamsFile.cc
  * @brief This file is part of UnitreeCameraSDK.
  * @details This example that how to get camera internal parameters
  * @author ZhangChunyang
  * @date  2021.07.31
  * @version 1.0.1
  * @copyright Copyright (c) 2020-2021, Hangzhou Yushu Technology Stock CO.LTD. All Rights Reserved.
  */

#include <UnitreeCameraSDK.hpp>
#include <unistd.h>

int main(int argc, char *argv[]){
    
    UnitreeCamera cam("stereo_camera_config.yaml"); ///< init UnitreeCamera object by config file
    if(!cam.isOpened())     ///< get camera open state
        exit(EXIT_FAILURE);
    
    cam.startCapture(); ///< disable image h264 encoding and share memory sharing
    usleep(100000);     ///< wait parameters initialization finished
    cam.saveCalibParams("output_camCalibParams.yaml"); ///< save parameters to output_camCalibParams.yaml
    std::cout << cam.getSerialNumber() << " " <<  cam.getPosNumber() << std::endl;
    usleep(100000);
    cam.stopCapture();  ///< stop camera capturing
    return 0;
}
