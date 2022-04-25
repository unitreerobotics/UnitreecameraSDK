#include <UnitreeCameraSDK.hpp>
#include <unistd.h>


  /*
  ##########################################################################################################
  Sender example:

  This example can send  original/rect/depth  images.


    For example:
      NanoB (as Sender)    --- # ./bins/example_putimagetrans2 origin
       argv[1] is "origin" / "rect" / "depth" (No option-> origin)

      NanoC (as Receiver)  --- # ./bins/example_getimagetrans 5

      Then flipped image will appear.
      If setting is wrong, no response. It is not kind.
      (Note that yaml file relative path. Maybe you should "cd .." after "make")


  [Images]
    - original image
    - rect image
    - depth image

    resolution(origin)  1856x800 / 928x400
    resolution(rect)     928x800 / 464x400 / 232x200



  ##########################################################################################################
  [Go1 robot]
    NanoA: Jetson Nano (Go1's Head, IP: 192.168.123.13)
    NanoB: Jetson Nano (Go1's Body, IP: 192.168.123.14)
    NanoC: Jetson Nano (Go1's Body, IP: 192.168.123.15)
    (Raspi board has OpenCV 3.x by default. This SDK need OpenCV 4.x .)
    You can login to console by SSH / GUI. 


  [example source]
    exPUT2: example_putimagetrans2.cc (This example. Sender)
    exPUT:  example_putimagetrans.cc  (Basic example. Sender)
    exGET:  example_getimagetrans.cc  (Receiver)

    Sender <---> Receiver



  [Preparation]
    This SDK can change the function by yaml file or manually in the source.


  !!!!!! Do it !!!!!
  [kill the process]
    In advance, kill the camera & imageai process.
    -> # ps aux | grep camera
    -> # sudo ~/Unitree/camerarosnode/cameraRosnode/kill.sh 
    -> # ps aux | grep camera
    -> # ps aux | grep image
    -> # sudo kill (PSID)  (Maybe Head's Nano)
    -> # ps aux | grep image
    (release the video devices. /dev/video0 and /dev/video1 .)


  [trans_rect_config.yaml]
    Change the settings.

    IpLastSegment:
      It should be receiver side IP address.
      13-15 or other numbers.
      (192.168.123.IpLastSegment)

    DeviceNode: 
      0/1:
      (It means /dev/video0 or /dev/video1)

    FrameSize / RectifyFrameSize:
      Change this proper resolution.
      (origin)  1856x800 / 928x400
      (rect)     928x800 / 464x400 / 232x200

    Transmode:
      0: Forigin left (right?)
      1: origin stereo
      2: rect left (right?)
      3: rect stereo
      4: rect left && depthimage, with setting Depthmode = 2.  (Not recommended ? Read the previous example_putimagetrans.cc .)

    Depthmode:
      1: default
      2: depthimage, Check the Transmode.


  [exGET(example_getimagetrans.cc)]
    Change the IpLastSegment in the source.
    It should be receiver side's IP address. (same as exPUT2.)

    port is automatically decided by SDK with camera position. (9201-9205)
    exGET can change the received port No. with argv[1] command line.
    port No.: 9201-9205 -> Front, Chin, Left, Right, Abdomen



  [Transfer the image]
    For example:
      NanoB (as Sender)    --- # ./bins/example_putimagetrans2 origin
       argv[1] is "origin" / "rect" / "depth" (No option-> origin)

      NanoC (as Receiver)  --- # ./bins/example_getimagetrans 5
      
      Then flipped image will appear.
      If setting is wrong, no response. It is not kind.
      (Note that yaml file relative path. Maybe you should "cd .." after "make")

      
      ('5' means port 9205.)
      (Note that yaml file relative path.)



  ##########################################################################################################
  [Supplement]

  1. You should set the correct timezone & date time.
     I spent a lot of time on it with not working.
  
       # sudo tzselect
       # sudo date -s "04/25 12:30 2022"


  2. You can check the transfer function with gstreamer command stand-alone.

    Sender ->
      # gst-launch-1.0 v4l2src device=/dev/video0 ! image/jpeg,width=1856,height=800,framerate=30/1 ! nvv4l2decoder mjpeg=1 ! nvvidconv ! video/x-raw,width=1856,height=800,format=I420,framerate=30/1 ! omxh264enc ! video/x-h264,stream-format=byte-stream ! h264parse ! rtph264pay ! udpsink host=192.168.123.15 port=9205

    Receiver ->
      # gst-launch-1.0 udpsrc address=192.168.123.15 port=9205 ! application/x-rtp,media=video,encoding-name=H264 ! rtph264depay name=hoge ! h264parse ! queue ! omxh264dec ! videoconvert ! autovideosink

    (if not Jetson Nano, It can alternate the 'omxh264enc' by 'avdec_h264'.  # sudo apt install gstreamer1.0-libav)


  3. trial and error.
    I will leave it for some reference.
    https://twitter.com/devemin/status/1509783551948226560?s=20&t=PUGf2EP9yaqXF4nP-vnRhg

  4. Log
    [Pattern of success]

    [exPUT2]
        unitree@nano2gb:~/myscripts/UnitreecameraSDK-main$ ./bins/example_putImagetrans2 depth
        [ WARN:0] global /home/nvidia/host/build_opencv/nv_opencv/modules/videoio/src/cap_gstreamer.cpp (1757) handleMessage OpenCV | GStreamer warning: Embedded video playback halted; module v4l2src0 reported: Internal data stream error.
        [ WARN:0] global /home/nvidia/host/build_opencv/nv_opencv/modules/videoio/src/cap_gstreamer.cpp (886) open OpenCV | GStreamer warning: unable to start pipeline
        [ WARN:0] global /home/nvidia/host/build_opencv/nv_opencv/modules/videoio/src/cap_gstreamer.cpp (480) isPipelinePlaying OpenCV | GStreamer warning: GStreamer: pipeline have not been created
        [UnitreeCameraSDK][INFO] Load camera flash parameters OK!
        depth image transfer
        [StereoCamera][INFO] Initialize parameters OK!
        [StereoCamera][INFO] Start stereo computing ...
        [StereoCamera][INFO] Start capture ...
        hostIp+portString:host=192.168.123.15 port=9201
        Framerate set to : 30 at NvxVideoEncoderSetParameterNvMMLiteOpen : Block : BlockType = 4
        ===== NVMEDIA: NVENC =====
        NvMMLiteBlockCreate : Block : BlockType = 4
        H264: Profile = 66, Level = 40



    [exGET]
        unitree@unitree-desktop:~/myscripts/UnitreecameraSDK-main$ ./bins/example_getimagetrans 1
        udpSendIntegratedPipe:udpsrc address=192.168.123.15 port=9201 ! application/x-rtp,media=video,encoding-name=H264 ! rtph264depay ! h264parse ! omxh264dec ! videoconvert ! appsink
        (example_getimagetrans:11476): GStreamer-CRITICAL **: 17:54:57.836: gst_caps_is_empty: assertion 'GST_IS_CAPS (caps)' failed
        (example_getimagetrans:11476): GStreamer-CRITICAL **: 17:54:57.836: gst_caps_truncate: assertion 'GST_IS_CAPS (caps)' failed
        (example_getimagetrans:11476): GStreamer-CRITICAL **: 17:54:57.836: gst_caps_fixate: assertion 'GST_IS_CAPS (caps)' failed
        (example_getimagetrans:11476): GStreamer-CRITICAL **: 17:54:57.836: gst_caps_get_structure: assertion 'GST_IS_CAPS (caps)' failed
        (example_getimagetrans:11476): GStreamer-CRITICAL **: 17:54:57.836: gst_structure_get_string: assertion 'structure != NULL' failed
        (example_getimagetrans:11476): GStreamer-CRITICAL **: 17:54:57.837: gst_mini_object_unref: assertion 'mini_object != NULL' failed
        NvMMLiteOpen : Block : BlockType = 261
        NVMEDIA: Reading vendor.tegra.display-size : status: 6
        NvMMLiteBlockCreate : Block : BlockType = 261
        Allocating new output: 928x400 (x 23), ThumbnailMode = 0
        OPENMAX: HandleNewStreamFormat: 3605: Send OMX_EventPortSettingsChanged: nFrameWidth = 928, nFrameHeight = 400
        [ WARN:0] global /home/unitree/opencv_source/opencv-4.1.1/modules/videoio/src/cap_gstreamer.cpp (933) open OpenCV | GStreamer warning: Cannot query video position: status=1, value=0, duration=-1
        Gtk-Message: 17:54:57.989: Failed to load module "canberra-gtk-module"


  */


int main(int argc, char* argv[])
{
    std::chrono::microseconds t;
    cv::Mat frame;
    cv::Mat depth;
    cv::Mat left, right, feim;

    
    UnitreeCamera cam("trans_rect_config.yaml");                //Read the settings.
    if (!cam.isOpened())
    {
        exit(EXIT_FAILURE);
    }


    std::string argstr = "";
    if (argc >= 2) {
        argstr = argv[1];
    }

    if ( (argstr.compare("origin") == 0) || (argstr == "")) {    //Original fish Eye image. (Transmode = 0/1, Depthmode = 1)
                                                                 //or No argv[1] is same as "origin"
        std::cout << "origin image transfer" << std::endl;
        cam.startCapture(true, false);                      //enable H264 encoding , disable memory sharing.

        usleep(500000);
        while (cam.isOpened())
        {

            if (!cam.getRawFrame(frame, t))
            {
                usleep(1000);
                continue;
            }
            char key = cv::waitKey(10);
            if (key == 27) {                                // press ESC key
                break;
            }
        }
    }
    else if (argstr.compare("rect") == 0) {                 //Rectify image. (Transmode = 2/3, Depthmode = 1)

        std::cout << "rect image transfer" << std::endl;
        cam.startCapture(true, false);

        usleep(500000);
        while (cam.isOpened())
        {
            if (!cam.getRectStereoFrame(left, right))
            {
                usleep(1000);
                continue;
            }
            char key = cv::waitKey(10);
            if (key == 27) {                                // press ESC key
                break;
            }
        }
    }
    else if (argstr.compare("depth") == 0) {                //Depth image. (Transmode = 4, Depthmode = 2)

        std::cout << "depth image transfer" << std::endl;
        cam.startCapture(true, false);                      //H264 encoding true/false
        cam.startStereoCompute();                           //start disparity computing

        usleep(500000);
        while (cam.isOpened())
        {
            if (!cam.getDepthFrame(depth, true, t))
            {
                usleep(1000);
                continue;
            }
            char key = cv::waitKey(10);
            if (key == 27) {                                // press ESC key
                break;
            }
        }
    }

    cam.stopStereoCompute();                                //stop disparity computing 
    cam.stopCapture();                                      //stop camera capturing

    return 0;
}
