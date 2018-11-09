#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
//using namespace std;

int main () 
{
    //Setup
    rs2::colorizer color_map;
    rs2::pipeline pipe;
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30); 
    //Start Stream
    pipe.start(cfg);
    rs2::frameset frames;
    //Create Windows
    namedWindow("Depth", WINDOW_AUTOSIZE);
    namedWindow("Color", WINDOW_AUTOSIZE);

    //Keep Getting Frames
    while (waitKey(1) < 0 && cvGetWindowHandle("Color") && cvGetWindowHandle("Depth")) {
        rs2::frameset data = pipe.wait_for_frames();
        rs2::frame depth = data.get_depth_frame().apply_filter(color_map);
        rs2::frame color_frame = data.get_color_frame();
        Mat image(Size(640, 480), CV_8UC3, (void*)depth.get_data(), Mat::AUTO_STEP);
        Mat color(Size(640, 480), CV_8UC3, (void*)color_frame.get_data(), Mat::AUTO_STEP);
        imshow("Color", color);
        imshow("Depth", image);
    }

    return EXIT_SUCCESS;

}
        
