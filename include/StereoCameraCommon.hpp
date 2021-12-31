/**
  * @file StereoCameraCommon.hpp
  * @brief This file is part of UnitreeCameraSDK, which declare stereo camera algorithm APIs.  
  * @details image capture, image rectification, disparity computation, depth image and point cloud generation
  * @author SunMingzhe
  * @date  2021.12.1
  * @version 1.1.0
  * @copyright Copyright (c)2020-2021, Hangzhou Yushu Technology Stock CO.LTD. All Rights Reserved.
  * Use of this source code is governed by the MPL-2.0 license, see LICENSE.
  * @ref ZhangChunyang v1.0.1
  */
#ifndef __STEREO_CAMERA_COMMOM_HPP__
#define __STEREO_CAMERA_COMMOM_HPP__

#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>
#include <chrono>
#include "SystemLog.hpp"

/**
  * @struct PCL
  * @brief this struct is used for rendering point cloud by RGB color
  * @details pts for point coordinates (x, y, z), clr for point color (b, g, r)
  * @note point cloud simple type 
  * @attention point data element type float, color data element type uchar 
  */
typedef struct PCL {
  cv::Vec3f pts;  ///< points coordinates (x, y, z)
  cv::Vec3b clr;  ///< points color (b, g, r)
}PCLType;
/**
  * @class StereoCamera
  * @brief this class provide the APIs of the algorithm and opration method of stereo cameras
  * @details this class is the base of UnitreeCameraSDK, it includes many functions, such as
  * camera frame capturing, image rectification, disparity computation, generating point cloud and etc.
  */
class StereoCamera
{
private:
    typedef struct TimeFrame{
        cv::Mat data1;                        ///< frame data
        cv::Mat data2;                        ///< frame data
        std::chrono::microseconds timeStamp;  ///< time since 1970-01-01 00:00:00, unit is microseconds(10^-6 s) 
    }TimeFrameType;
private:
    int m_radius = 0;
    int m_algorithm = 0;
    int m_logLevel = 1;
    int m_deviceNode = 0;
    int m_posNumber = 0;
    int m_serialNumber = 0;
    int m_threshold = 120;
    float m_frameRate = 30.0;
    int m_ipLastSegment = 15;

    bool m_shareMemSupport = false;
    bool m_udpH264Support = false;
    bool m_isOpened = false;
    bool m_isCompute = false;
    float maxdepth = 1;
    float mindepth = 0.05;

    int m_transmode = -1;
    double m_transrate = 0;
    double m_hfov = 90;
    double default_pfov = 90;
    double default_fov = 222;
    double default_llfov = 180;
    int m_depthmode = 1; 

    cv::VideoWriter gstWriter;

    cv::Size m_frameSize;
    cv::Size m_rectSize;

    TimeFrameType m_stampFrame, m_dispFrame;
    
    cv::Mat m_leftXi,m_leftRotation,m_leftIntrinsic,m_leftDistortion;
    cv::Mat m_rightXi,m_rightRotation,m_rightIntrinsic,m_rightDistortion;
    cv::Mat m_translation;
       
    cv::Mat m_kfe, m_fmap[2][2], m_lmap[2][2], m_lagerFmap[2];
    
    SystemLog *m_log = nullptr;
    std::string m_logName = "StereoCamera";
    
    std::mutex m_capLock, m_dispLock;
    std::condition_variable m_capTrigger, m_dispTrigger;
    
    cv::VideoCapture *m_videoCap = nullptr;
    
    std::thread *m_capWorker = nullptr;
    std::thread *m_dispWorker = nullptr;


public:
    /**
      * @fn StereoCamera
      * @brief StereoCamera
      * @details default constructor
      * @param[in] None
      * @param[out] None
      * @return None
      * @note initialize StereoCamera object by default settings
      * @code
      *     StereoCamera *pCam = new StereoCamera(); // or StereoCamera cam;
      * @endcode
      */
    StereoCamera(void);
    /**
      * @fn StereoCamera
      * @brief StereoCamera constructor
      * @details use camera config file to init stereo camera
      * @param[in] fileName camera config file, include file path, for example: ~/test/stereoConfig.yaml
      * @param[out] None
      * @return None
      * @note initialize StereoCamera object by settings from configure file
      * @code
      *     StereoCamera("path_to/config.yaml");
      * @endcode
      */
    StereoCamera(std::string fileName);
    /**
      * @overload
      * @fn StereoCamera 
      * @brief StereoCamera constructor overload
      * @details use camera device node number to initialize stereo camera object
      * @param[in] deviceNode camera device node, for example: /dev/video0, camera device node: 0
      * @param[out] None
      * @return None
      * @note initialize StereoCamera object by device node number
      * @code
      *     StereoCamera(0); // for /dev/video0
      * @endcode
      */
    StereoCamera(int deviceNode);
    /**
      * @fn ~StereoCamera() 
      * @brief StereoCamera destructor
      * @details release all stereo camera resource
      * @param[in] None
      * @param[out] None
      * @return None
      * @note automatically called when StereoCamera object released
      */
    virtual ~StereoCamera();
    
public:
    /**
      * @fn isOpened
      * @brief get stereo camera running status 
      * @details detect camera is opened or closed
      * @param[in] None
      * @param[out] None
      * @return true or false, if camera is opened return true, otherwise return false
      * @note tell camera running status, it should be called after StereoCamera object initialization 
      * @code
      *     StereoCamera cam;
      *     if(cam.isOpened())
      *         printf("Camera is opened!");
      * @endcode
      */
    virtual bool isOpened(void);
    /**
      * @fn setLogLevel
      * @brief set stereo camera ouput log level 
      * @details log level means that different kind of information will be output
      * @param[in] level
      * @param[out] None
      * @return true or false, if assignment successfully return true, otherwise return false
      * @note output information starts with "StereoCamera", it should be called after StereoCamera object initialization 
      * @code
      *     cam.setLogLevel(2); //debug level 
      * @endcode
      */
    virtual bool setLogLevel(int level);
    /**
      * @fn setPosNumber
      * @brief set stereo camera position number 
      * @details position number means that the number of camera location in robot body
      * @param[in] posNumber
      * @param[out] None
      * @return true or false, if assignment successfully return true, otherwise return false
      * @note face NO.1, chin NO.2, left NO.3, right NO.4, down NO.5
      * @code
      *     cam.setPosNumber(2);
      * @endcode
      */
    virtual bool setPosNumber(int posNumber);
    /**
      * @fn setSerialNumber
      * @brief set stereo camera serial number 
      * @details serial number same as robot serial number
      * @param[in] serialNumber
      * @param[out] None
      * @return true or false, if assignment successfully return true, otherwise return false
      * @note 
      * @code
      *     cam.setSerialNumber(2);
      * @endcode
      */
    virtual bool setSerialNumber(int serialNumber);
    /**
      * @fn setRawFrameRate
      * @brief set stereo camera FPS (Frames Per Seconds)
      * @details for 1856X800 FPS:30, for 928x400 FPS:30 or 60
      * @param[in] frameRate
      * @param[out] None
      * @return true or false, if assignment successfully return true, otherwise return false
      * @note 
      * @code
      *     cam.setRawFrameRate(30)
      * @endcode
      */
    virtual bool setRawFrameRate(int frameRate);
    /**
      * @fn setRawFrameSize
      * @brief set stereo camera frame size
      * @details There are two choise: 1856X800,928x400.
      * @param[in] frameSize
      * @param[out] None
      * @return true or false, if assignment successfully return true, otherwise return false
      * @attention if do not assign frameSize, default size is 1856x800 
      * @note
      * @code
      *    cam.setRawFrameSize(cv::Size(1856, 800));
      * @endcode
      */
    virtual bool setRawFrameSize(cv::Size frameSize);
    /**
      * @fn setRectFrameSize
      * @brief set rectification image size
      * @details this size should keep ratio as camera raw image size, for example: raw single frame size: 928x800
      * rectification image size: 464x400 or 232x200
      * @param[in] frameSize
      * @param[out] None
      * @return true or false, if assignment successfully return true, otherwise return false
      * @attention rectification image size must be smaller than camera raw single image size 
      * @note
      * @code
      *     cam.setRawFrameSize(cv::Size(464, 400));
      * @endcode
      */
    virtual bool setRectFrameSize(cv::Size frameSize);
    /**
      * @fn setCalibParams
      * @brief set stereo camera calibration parameters
      * @details calibration paramerters include: camera intrinsic matrix, 
      * camera distortion coefficient, xi(mei model), rotation matrix, translation vector, rectification image intrinsic matrix
      * @param[in] paramsArray arrange Array element as follows: intrinsic,distortion,xi,rotation,translation,kfe
      * @param[in] flag false: set left camera params, true: right camera params
      * @param[out] None
      * @return true or false, if assignment successfully return true, otherwise return false
      * @attention the order of parameters should be correct
      * @note 
      * @code
      *     std::vector<cv::Mat> leftParamsArray;
      *     cam.setCalibParams(leftParamsArray);
      * @endcode
      */
    virtual bool setCalibParams(std::vector<cv::Mat> paramsArray, bool flag = false);
    /**
      * @fn getLogLevel
      * @brief get log system output level
      * @details 
      * @param[in] None 
      * @param[out] None
      * @return log level
      * @note 1: output runtime information, 2: output runtime infomation and debug information
      * @code
      *     int LogLevel = cam.getLogLevel();
      * @endcode
      */
    virtual int getLogLevel(void) const;
    /**
      * @fn getDeviceNode
      * @brief get camera device node 
      * @details 
      * @param[in] None 
      * @param[out] None
      * @return device node number
      * @note  for example: /dev/video2, device node number is 2, the return value of function is 2. 
      * @code
      *     int nodeNumber = cam.getDeviceNode();
      * @endcode
      */
    virtual int getDeviceNode(void) const;
    /**
      * @fn getPosNumber
      * @brief get stereo camera device node
      * @details 
      * @param[in] None 
      * @param[out] None
      * @return positon number 
      * @note face NO.1, chin NO.2, left NO.3, right NO.4, down NO.5
      * @code
      *     int posNum = cam.getPosNumber();
      * @endcode
      */
    virtual int getPosNumber(void) const;
    /**
      * @fn getSerialNumber
      * @brief get stereo camera serial number
      * @details this number is same as robot serial number
      * @param[in] None 
      * @param[out] None
      * @return robot serial number 
      * @note 
      * @code
      *     int seiralNumber = cam.getSerialNumber();
      * @endcode
      */
    virtual int getSerialNumber(void) const;
    /**
      * @fn getRawFrameRate
      * @brief get stereo camera capture frame rate  
      * @details 
      * @param[in] None 
      * @param[out] None
      * @return current stereo camera fps.
      * @code 
      *     float fps = getRawFrameRate();
      * @endcode
      */
    virtual float getRawFrameRate(void) const;
    /**
      * @fn getRawFrameSize
      * @brief get stereo camera frame size  
      * @details 
      * @param[in] None 
      * @param[out] None
      * @return frame size.
      * @code
      *     cv::Size fSize = cam.getRawFrameSize();
      * @endcode
      */
    virtual cv::Size getRawFrameSize(void) const;
    /**
      * @fn getRawFrame
      * @brief get stereo camera raw frame
      * @details 
      * @param[in] None 
      * @param[out] frame raw frame, include left and right image
      * @param[out] timeStamp  raw frame time stamp
      * @return true or false, if frame is not empty, return true, otherwise return false. 
      * @note 
      * @code
      *     cv::Mat frame;
      *     std::chrono::microseconds timeStamp
      *     cam.getRawFrame(frame, timeStamp);
      * @endcode
      */
    virtual bool getRawFrame(cv::Mat &frame, std::chrono::microseconds &timeStamp);
    /**
      * @fn getStereoFrame
      * @brief get stereo camera left and right image
      * @details 
      * @param[in] None 
      * @param[out] left left image
      * @param[out] right right image
      * @param[out] timeStamp  frame time stamp
      * @return true or false, if left and right images are not empty, return true, otherwise return false. 
      * @attention This funtion must be called after startCapture().
      * @note 
      * @code
      *     cam.startCapture();
      *     cv::Mat left, right;
      *     std::chrono::microseconds timeStamp;
      *     if(cam.getRawFrame(left,right, timeStamp)){
      *         //do something
      *     }
      * @endcode
      */
    virtual bool getStereoFrame(cv::Mat &left, cv::Mat &right, std::chrono::microseconds &timeStamp);
    /**
      * @fn getDepthFrame
      * @brief get stereo camera depth image
      * @details 
      * @param[in] color  true: depth is color image, false:depth is gray image
      * @param[out] depth left image
      * @param[out] timeStamp  frame time stamp
      * @return true or false, if depth image is not empty, return true, otherwise return false. 
      * @attention This funtion must be called after startCapture().
      * @note 
      * @code
      *     cam.startCapture();
      *     cam.startStereoCompute();
      *     cv::Mat depth;
      *     std::chrono::microseconds timeStamp;
      *     if(cam.getPointCloud(depth, true, timeStamp)){
      *         //do something
      *     }
      * @endcode
      */
    virtual bool getDepthFrame(cv::Mat& depth, bool color, std::chrono::microseconds &timeStamp);
    /**
      * @fn getPointCloud
      * @brief get a stereo camera point cloud frame
      * @details A point cloud is a dataset of points in a coordinate frame.
      * @param[in] None
      * @param[out] pcl point cloud, which element has 3D coordinates information (x, y, z) 
      * @param[out] timeStamp  poind cloud time stamp
      * @return true or false, if point cloud size bigger than 0, return true, otherwise return false. 
      * @attention This funtion must be called after startCapture() and startStereoCompute()
      * @note
      * @code
      *     cam.startCapture();
      *     cam.startStereoCompute();
      *     std::vector<cv::Vec3f> pcl;
      *     std::chrono::microseconds timeStamp;
      *     if(cam.getPointCloud(pcl, timeStamp)){
      *         //do something
      *     }
      * @endcode
      */


    virtual void getDepthFrame(cv::Mat& dispf, cv::Mat& depth);

    virtual bool getPointCloud(std::vector<cv::Vec3f> &pcl, std::chrono::microseconds &timeStamp);
    /**
      * @overload
      * @fn getPointCloud
      * @brief get a stereo camera point cloud frame
      * @details A point cloud is a dataset of points in a coordinate frame.
      * @param[in] None
      * @param[out] pcl point cloud, which element has 3D coordinates information (x, y, z) and color information (B, G, R)
      * @param[out] timeStamp  poind cloud time stamp
      * @return true or false, if point cloud size bigger than 0, return true, otherwise return false. 
      * @attention This funtion must be called after startCapture() and startStereoCompute()
      * @note
      * @code
      *     cam.startCapture();
      *     cam.startStereoCompute();
      *     std::vector<PCLType> pcl;
      *     std::chrono::microseconds timeStamp;
      *     if(cam.getPointCloud(pcl, timeStamp)){
      *         //do something
      *     }
      * @endcode
      */
    virtual bool getPointCloud(std::vector<PCLType>& pcl, std::chrono::microseconds& timeStamp);
    /**
      * @fn getCalibParams
      * @brief get stereo camera calibration paramerters
      * @details calibration paramerters include: camera intrinsic matrix, 
      * camera distortion coefficient, xi(mei model), rotation matrix, translation vector, rectification image intrinsic matrix
      * @param[in] flag default false, false: get left camera parameters, true: get right camera parameters.
      * @param[out] paramsArray arrange Array element as follows: intrinsic,distortion,xi,rotation,translation,kfe
      * @return true or false, if assignment successfully return true, otherwise return false 
      * @attention This funtion must be called after startCapture()
      * @note
      * @code
      *     cam.startCapture();
      *     std::vector<cv::Mat> paramsArray;
      *     if(cam.getCalibParams(paramsArray)){
      *         //do something
      *     }
      * @endcode
      */
    virtual bool getCalibParams(std::vector<cv::Mat> &paramsArray, bool flag = false);
    /**
      * @fn getRectStereoFrame
      * @brief get stereo camera rectification image
      * @details 
      * @param[in] None
      * @param[out] left rect left image, use LONGLAT
      * @param[out] right rect right image, use LONGLAT
      * @param[out] feim rect left image, use PERSPECTIVE
      * @return true or false, if left, right and feim are not empty return true, otherwise return false 
      * @attention This funtion must be called after startCapture()
      * @note
      * @code
      *     cam.startCapture();
      *     left, right, feim;
      *     if(cam.getRectStereoFrame(left, right, feim)){
      *         //do something
      *     }
      * @endcode
      */
    virtual bool getRectStereoFrame(cv::Mat &left, cv::Mat &right, cv::Mat &feim);
    /**
      * @fn getRectStereoFrame
      * @brief get stereo camera rectification image
      * @details 
      * @param[in] None
      * @param[out] left rect left image, use PERSPECTIVE
      * @param[out] right rect right image, use PERSPECTIVE
      * @return true or false, if left, right are not empty return true, otherwise return false 
      * @attention This funtion must be called after startCapture()
      * @note
      * @code
      *     cam.startCapture();
      *     left, right;
      *     if(cam.getRectStereoFrame(left, right)){
      *         //do something
      *     }
      * @endcode
      */
    virtual bool getRectStereoFrame(cv::Mat &left, cv::Mat &right);
    /**
      * @overload 
      * @fn getRectStereoFrame
      * @brief get stereo camera rectification image
      * @details 
      * @param[in] None
      * @param[out] left rect left image, use LONGLAT
      * @param[out] right rect right image, use LONGLAT
      * @param[out] feim rect left image, use PERSPECTIVE
      * @param[out] timeStamp images timeStamp
      * @return true or false, if left, right and feim are not empty return true, otherwise return false 
      * @attention This funtion must be called after startCapture()
      * @note
      * @code
      *     cam.startCapture();
      *     cv::Mat left, right, feim;
      *     if(cam.getRectStereoFrame(left, right, feim)){
      *         //do something
      *     }
      * @endcode
      */
    virtual bool getRectStereoFrame(cv::Mat &left, cv::Mat &right, cv::Mat &feim, std::chrono::microseconds &timeStamp);
    /**
      * @fn loadConfig
      * @brief load stereo camera config parameters 
      * @details parameters include: LogLevel,Threshold,Algorithm,IpLastSegment,
      * DeviceNode,FrameSize,RectifyFrameSize,FrameRate, etc.
      * @param[in] fileName config name: include config path, for example: "path_to/config.yaml"
      * @param[out] None
      * @return true or false, if load config file successfully return true, otherwise return false 
      * @attention This funtion must be called before startCapture() and after init the camera object.
      * @note 
      * @code
      *     cam.loadConfig("path_to/config.yaml");
      *     cam.startCapture();
      * @endcode
      */
    virtual bool loadConfig(std::string fileName);
    /**
      * @fn loadCalibParams
      * @brief load stereo camera calibration params  
      * @details calibration paramerters include: camera intrinsic matrix, 
      * camera distortion coefficient, xi(mei model), rotation matrix, translation vector, rectification image intrinsic matrix, etc.
      * @param[in] fileName calibration file name: include file path, for example: "path_to/calib.yaml"
      * @param[out] None
      * @return true or false, if load file successfully return true, otherwise return false 
      * @attention This funtion must be called before startCapture() and after loadConfig("xxx");
      * @note 
      * @code
      *     cam.loadConfig("path_to/config.yaml");
      *     cam.loadCalibParams("path_to/calib.yaml");
      *     cam.startCapture();
      * @endcode
      */
    virtual bool loadCalibParams(std::string fileName);
    /**
      * @fn saveConfig
      * @brief save stereo camera settings to config file 
      * @details 
      * @param[in] fileName default name: stereo_camera_config.yaml, 
      * output config file: include file path, for example: "path_to/config.yaml"
      * @param[out] None
      * @return true or false, if save file successfully return true, otherwise return false 
      * @code
      *     cam.saveConfig("stereo_camera_config.yaml");
      * @endcode
      */
    virtual bool saveConfig(std::string fileName = "stereo_camera_config.yaml");
    /**
      * @fn saveCalibParams
      * @brief save stereo camera calibration parameters to file 
      * @details 
      * @param[in] fileName default name: stereo_camera_calibparams.yaml, 
      * output file: include file path, for example: "path_to/calib.yaml"
      * @param[out] None
      * @return true or false, if save file successfully return true, otherwise return false 
      * @code
      *     cam.saveConfig("stereo_camera_config.yaml");
      * @endcode
      */
    virtual bool saveCalibParams(std::string fileName = "stereo_camera_calibparams.yaml");
    /**
      * @fn startCapture
      * @brief start stereo camera capture thread 
      * @details 
      * @param[in] udpFlag default false, true: enable using H264 encode camera frame and send it by gstream udp command 
      * @param[in] shmFlag default false, true: enable using share memory to share camera frame 
      * @param[out] None
      * @return true or false, if create capture thread successfully return true, otherwise return false 
      * @note 
      * gstreamer send command: "appsrc ! videoconvert ! omxh264enc ! video/x-h264, stream-format=byte-stream ! rtph264pay ! udpsink host=192.168.123.SEG port=hostPort". 
      * SEG is last segment of your recieve board IP, its range from 2~254,for example: 13, host=192.168.123.13.
      * hostPort is defined by the result of adding camera position number and 9000, for example: camera positon number is 5, port=9005.
      * share memory key number is defined by the result of adding camera position number and 9000.
      * This function must be called after camera config or settings finished.
      * @code
      *     cam.startCapture(true, false); // support h264 encode frame and send it by udp
      * @endcode
      */
    virtual bool startCapture(bool udpFlag = false, bool shmFlag = false);
    /**
      * @fn startStereoCompute
      * @brief start stereo camera computing disparity thread 
      * @details 
      * @param[in] None
      * @param[out] None
      * @return true or false, if create thread successfully return true, otherwise return false 
      * @attention This function must be called after startCapture();
      * @note 
      * @code
      *     cam.startCapture();
      *     cam.startStereoCompute();
      * @endcode
      */
    virtual bool startStereoCompute(void);
    /**
      * @fn stopStereoCompute
      * @brief stop stereo camera computing disparity thread 
      * @details 
      * @param[in] None
      * @param[out] None
      * @return true or false, if stop thread successfully return true, otherwise return false 
      * @attention This function must be called after startStereoCompute()
      * @note 
      * @code
      *     cam.startStereoCompute();
      *     cam.stopStereoCompute();
      * @endcode
      */
    virtual bool stopStereoCompute(void);
    /**
      * @fn stopCapture
      * @brief stop stereo camera capture thread 
      * @details 
      * @param[in] None
      * @param[out] None
      * @return true or false, if stop thread successfully return true, otherwise return false 
      * @attention This function must be called after startCapture().
      * @note
      * @code
      *     cam.startCapture();
      *     cam.startStereoCompute();
      *     cam.stopStereoCompute();
      *     cam.stopCapture();
      * @endcode
      */
    virtual bool stopCapture(void);



};




#endif //__STEREO_CAMERA_COMMOM_HPP__
