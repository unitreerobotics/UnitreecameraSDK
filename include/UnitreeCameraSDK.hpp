
/**
  * @file UnitreeCameraSDK.hpp
  * @brief This file is part of UnitreeCameraSDK, which declare unitree camera APIs.  
  * @details class UnitreeCamera inherits the APIs of class StereoCamera 
  * image capture, image rectification, disparity computation, depth image and point cloud generation
  * update camera firmware, flash camera parameters to camera
  * @author ZhangChunyang
  * @date  2021.07.31
  * @version 1.0.1
  * @copyright Copyright (c) 2020-2021, Hangzhou Yushu Technology Stock CO.LTD. All Rights Reserved.
  */

#ifndef __UNITREE_CAMERA_SDK_HPP__
#define __UNITREE_CAMERA_SDK_HPP__

#include "StereoCameraCommon.hpp"

class UnitreeCamera : public StereoCamera
{
private:
    std::string m_logName = "UnitreeCameraSDK";
    SystemLog *m_log = nullptr;
    
public:
    /**
      * @fn UnitreeCamera
      * @brief UnitreeCamera init unitree stereo camera object for default parameters
      * @details default constructor
      * @param[in] None
      * @param[out] None
      * @return None
      * @note 
      * @code
      *     UnitreeCamera();
      * @endcode
      */    
    UnitreeCamera();
    /**
      * @overload
      * @fn UnitreeCamera
      * @brief UnitreeCamera constructor overload
      * @details use camera config file to init unitree stereo camera
      * @param[in] fileName camera config file, include file path, for example: ~/test/stereoConfig.yaml
      * @param[out] None
      * @return None
      * @note 
      * @code
      *     UnitreeCamera("path_to/config.yaml");
      * @endcode
      */
    UnitreeCamera(std::string fileName);
    /**
      * @overload
      * @fn UnitreeCamera 
      * @brief UnitreeCamera constructor overload
      * @details use camera device node number to init unitree stereo camera
      * @param[in] deviceNode camera device node, for example: /dev/video0, camera device node: 0
      * @param[out] None
      * @return None
      * @note 
      * @code
      *     StereoCamera(0); // for /dev/video0
      * @endcode
      */
    UnitreeCamera(int deviceNode);
    /**
      * @fn ~UnitreeCamera 
      * @brief UnitreeCamera destructor
      * @details release all unitree stereo camera resource
      * @param[in] None
      * @param[out] None
      * @return None
      * @note  automatically called when UnitreeCamera object released
      */
    ~UnitreeCamera(void);

public:
    /**
      * @fn updateCalibParams2Flash 
      * @brief update camera calibrate parameters  
      * @details 
      * @param[in] fileName calibration parameters file 
      * @param[out] None
      * @return None
      * @note 
      * @code
      * @endcode
      */
    bool updateCalibParams2Flash(std::string fileName);
    /**
      * @fn checkFirmwareVersion 
      * @brief check camera's firmware version
      * @details 
      * @param[in] version firmware version
      * @param[out] None
      * @return None
      * @note 
      * @code
      * @endcode
      */
    bool checkFirmwareVersion(std::string &version);
    /**
      * @fn updateFirmware 
      * @brief update camera's firmware
      * @details 
      * @param[in] fileName firmware file
      * @param[out] None
      * @return None
      * @note 
      * @code
      * @endcode
      */
    bool updateFirmware(std::string fileName);

private:
    /**
      * @fn loadInternalCalibParams 
      * @brief 
      * @details 
      * @param[in] None
      * @param[out] None
      * @return None
      * @note 
      * @code
      * @endcode
      */
    bool loadInternalCalibParams(void);
};

/**
  * @fn getRotationMatrix 
  * @brief Obtain transformation matrix that can express the rotation of an Euler angle of the coordinate system around a coordinate axis 
  * @details 
  * @param[in] axis frame axis name, default 'x'
  * @param[in] theta Euler angle
  * @param[out] None
  * @return 3x3 rotation matrix 
  * @note 
  * @code
  * @endcode
  */
cv::Mat getRotationMatrix(int axis = 'x', float theta = 0.0);
/**
  * @fn getTranslationMatrix 
  * @brief transformation matrix that can express the moving coordinate system relative to the base coordinate system 
  * @details 
  * @param[in] axis frame axis name, default 'x'
  * @param[in] theta Euler angle
  * @param[out] None
  * @return 3x3 rotation matrix 
  * @note 
  * @code
  * @endcode
  */
cv::Mat getTranslationMatrix(int axis = 'x', float theta = 0.0, cv::Mat vec = cv::Mat());

#endif
