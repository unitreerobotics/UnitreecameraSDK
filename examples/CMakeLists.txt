
add_executable(example_getRawFrame ./example_getRawFrame.cc)
target_link_libraries(example_getRawFrame ${SDKLIBS})

add_executable(example_getDepthFrame ./example_getDepthFrame.cc)
target_link_libraries(example_getDepthFrame ${SDKLIBS})

add_executable(example_getRectFrame ./example_getRectFrame.cc)
target_link_libraries(example_getRectFrame ${SDKLIBS})

add_executable(example_getCalibParamsFile ./example_getCalibParamsFile.cc)
target_link_libraries(example_getCalibParamsFile ${SDKLIBS})

add_executable(example_putImagetrans ./example_putImagetrans.cc)
target_link_libraries(example_putImagetrans ${SDKLIBS})

add_executable(example_getimagetrans ./example_getimagetrans.cc)
target_link_libraries(example_getimagetrans ${SDKLIBS})

# add_executable(example_share ./example_share.cc)
# target_link_libraries(example_share ${SDKLIBS})

find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
    message(STATUS ${OPENGL_INCLUDE_DIR})
    message(STATUS ${OPENGL_LIBRARIES})
else()
    message(WARNING "OpenGL Library Not Found")
endif()

find_package(GLUT REQUIRED)
if(GLUT_FOUND)
    include_directories(${GLUT_INCLUDE_DIR})
    message(STATUS ${GLUT_INCLUDE_DIR})
    message(STATUS ${GLUT_LIBRARY})
else()
    message(WARNING "GLUT Library Not Found")
endif()

find_package(X11 REQUIRED)
if(X11_FOUND)
    include_directories(${X11_INCLUDE_DIR})
    message(${X11_INCLUDE_DIR})
    message(${X11_LIBRARIES})
else()
    message(WARNING "X11 Library Not Found")
endif()

if(X11_FOUND AND OpenGL_FOUND AND GLUT_FOUND)
    set(ShowPointCloud true)
    message(STATUS "Point Cloud Example Enabled")
else()
    set(ShowPointCloud false)
    message(WARNING "Point Cloud Example Disabled")
endif()

if(${ShowPointCloud})
add_executable(example_getPointCloud ./example_getPointCloud.cc ./glViewer/glwindow_x11.cpp ./glViewer/scenewindow.cpp)
target_link_libraries(example_getPointCloud ${SDKLIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${X11_LIBRARIES} )
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")


