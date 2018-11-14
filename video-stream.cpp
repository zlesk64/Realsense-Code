// include the librealsense C++ header file
#include <librealsense2/rs.hpp>

// include OpenCV header file
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    //Contruct a pipeline which abstracts the device
    rs2::pipeline pipe;

    //Create a configuration for configuring the pipeline with a non default profile
    rs2::config cfg;

    //Add desired streams to configuration
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
  //  while (true) {
    //Instruct pipeline to start streaming with the requested configuration
    pipe.start(cfg);

    // Camera warmup - dropping several first frames to let auto-exposure stabilize
    rs2::frameset frames;
    for(int i = 0; i < 30; i++)
//   while (true) {
    {
        //Wait for all configured streams to produce a frame
        frames = pipe.wait_for_frames();
    }
    while (true) {
    //Get each frame

    frames = pipe.wait_for_frames();

    rs2::frame color_frame = frames.get_color_frame();

    // Creating OpenCV Matrix from a color image
    Mat color(Size(640, 480), CV_8UC3, (void*)color_frame.get_data(), Mat::AUTO_STEP);
    Mat dst, cdst;
    Canny(color, dst, 50, 200, 3);
    cvtColor(dst, cdst, CV_GRAY2BGR);
    vector<Vec2f> lines;
    HoughLines(dst, lines, 1, CV_PI/180, 150, 0, 0 );
         for( size_t i = 0; i < lines.size(); i++ )
      {
          float rho = lines[i][0], theta = lines[i][1];
          if( theta>CV_PI/180*50 || theta<CV_PI/180*40)
        { Point pt1, pt2;
          double a = cos(theta), b = sin(theta);
          double x0 = a*rho, y0 = b*rho;
          pt1.x = cvRound(x0 + 1000*(-b));
          pt1.y = cvRound(y0 + 1000*(a));
          pt2.x = cvRound(x0 - 1000*(-b));
          pt2.y = cvRound(y0 - 1000*(a));
          line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
      }
    }  
    // Display in a GUI
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", color);
    imshow("Lines", cdst);
//    cout << "running" << endl;
     waitKey(25);
    //cout << "running" << endl;
}
    return 0;

}
