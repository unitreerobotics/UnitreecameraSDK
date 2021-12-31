Introduction
---
Unitree Robotics is a energetic start-up company that focuses on the development, production and sales of high-performance quadruped robots. It has been interviewed by BBC and CCTV, and is one of the earliest company to publicly sell quadruped robots.

The company has an outstanding leadership in developing robot core components, motion control, robot perception, etc.

We attaches great importance to research and development, and thus independently developed the motors, reducers, controllers, and even some sensors of the quadruped robot.

1.Overview
---
UnitreeCameraSDK 1.1.0 is a cross-platform library for unitree stereo cameras

The SDK allows depth and color streaming, and provides intrinsic calibration information. The library also offers pointcloud, depth image aligned to color image.

2.Dependencies
---

OpenCV, version: equal or lager than 4 (need gstreamer)

CMake, version: 2.8 or higher

[OpenGL] for point cloud gui

[GLUT] for point cloud gui

[X11], for point cloud gui 

2.Build
---

```
cd UnitreeCameraSDK;
mkdir build && cd build;
cmake ..; make
```

3.Run Examples
---

Get Camera Raw Frame:
```
cd UnitreeCameraSDK; 
./bin/example_getRawFrame 
```

Get Calibration Parameters File
```
cd UnitreeCameraSDK;
./bin/example_getCalibParamsFile 
```

Get Rectify Frame
```
cd UnitreeCameraSDK;
./bin/example_getRectFrame
```

Get Depth Frame
```
cd UnitreeCameraSDK;
./bin/example_getDepthFrame
```

Get Point Cloud:
```
cd UnitreeCameraSDK; 
./bin/example_getPointCloud
```

4.send image and listen image
sender:put image to another devices
```
cd UnitreeCameraSDK; 
./bin/example_putImagetrans
```

listener:get image from another devices
```
cd UnitreeCameraSDK; 
./bin/example_getimagetrans
```



