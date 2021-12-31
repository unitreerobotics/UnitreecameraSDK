/**
  * @file example_getPointCloud.cc
  * @brief This file is part of UnitreeCameraSDK.
  * @details This example that how to get camera point cloud. 
  * @author ZhangChunyang
  * @date  2021.07.31
  * @version 1.0.1
  * @copyright Copyright (c) 2020-2021, Hangzhou Yushu Technology Stock CO.LTD. All Rights Reserved.
  */

#include <GL/gl.h>
#include <signal.h>
#include <cerrno>
#include <cfenv>
#include <unistd.h>
#include "glViewer/scenewindow.hpp"
#include <UnitreeCameraSDK.hpp>

#define RGB_PCL true ///< Color Point Cloud Enable Flag

void DrawScene(const std::vector<PCLType>& pcl_vec) {
    glBegin(GL_POINTS);
    for (uint i = 0; i < pcl_vec.size(); ++i) {
        PCLType pcl = pcl_vec[i];
        glColor3ub(pcl.clr(2), pcl.clr(1), pcl.clr(0));
        glVertex3f(-pcl.pts(0), -pcl.pts(1), pcl.pts(2));
    }
    glEnd();
}

void DrawScene(const std::vector<cv::Vec3f>& pcl_vec) {
    glBegin(GL_POINTS);
    for (uint i = 0; i < pcl_vec.size(); ++i) {
        cv::Vec3f pcl = pcl_vec[i];
        glColor3ub(255, 255, 0);
        glVertex3f(-pcl(0), -pcl(1), pcl(2));
    }
    glEnd();
}

bool killSignalFlag = false;
void ctrl_c_handler(int s){
    killSignalFlag = true;
    return ; 
}

int main(int argc, char *argv[]){
    
    UnitreeCamera cam("stereo_camera_config.yaml");
    if(!cam.isOpened())
        exit(EXIT_FAILURE);

    cam.startCapture();
    cam.startStereoCompute();
    
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = ctrl_c_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
    
    std::cout << cam.getSerialNumber() << " " <<  cam.getPosNumber() << std::endl;
    
    glwindow::SceneWindow scene(960, 720, "Panorama 3D Scene");
    
    while(cam.isOpened()){
        
        if(killSignalFlag){
            break;
        }
        
        std::chrono::microseconds t;
#if RGB_PCL       
        std::vector<PCLType> pcl_vec;
        if(!cam.getPointCloud(pcl_vec, t)){
            usleep(1000);
            continue;
        }
#else        
        std::vector<cv::Vec3f> pcl_vec;
        if(!cam.getPointCloud(pcl_vec, t)){
            usleep(1000);
            continue;
        }
#endif
        if (scene.win.alive()) {
            if (scene.start_draw()) {
                DrawScene(pcl_vec);
                scene.finish_draw();
            }
        }
    }
    
    cam.stopStereoCompute();
    cam.stopCapture();
    return 0;
}
