#include <UnitreeCameraSDK.hpp>
#include <unistd.h>

int main(int argc, char *argv[])
{
   
    UnitreeCamera cam("trans_rect_config.yaml"); ///< init camera by device node number
    if(!cam.isOpened())   ///< get camera open state
        exit(EXIT_FAILURE);   
    cam.startCapture(false, true);

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