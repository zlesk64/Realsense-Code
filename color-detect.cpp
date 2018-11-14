#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "librealsense2/rs.hpp"

using namespace cv;
using namespace std;

 int main( int argc, char** argv )
 {
//  Realsense Depth Setup

     rs2::colorizer color_map;
     rs2::pipeline pipe;
     rs2::config cfg;
     cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
     pipe.start(cfg);
     rs2::frameset frames;
     cout << "Initialized Depth" << endl;
    
//  Realsense Color Setup 

     VideoCapture cap(0 + CV_CAP_INTELPERC_IMAGE); //capture the video from web cam
     cout << "Initialized Color" << endl;

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }
    
    namedWindow("Depth", CV_WINDOW_AUTOSIZE); //create Depth window
    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
    
 int iLowH = 0;
 int iHighH = 179;

 int iLowS = 0; 
 int iHighS = 255;

 int iLowV = 0;
 int iHighV = 255;

 //Create trackbars in "Control" window
 cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
 cvCreateTrackbar("HighH", "Control", &iHighH, 179);

 cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 cvCreateTrackbar("HighS", "Control", &iHighS, 255);

 cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
 cvCreateTrackbar("HighV", "Control", &iHighV, 255);

    while (true)
    {

        rs2::frameset data = pipe.wait_for_frames();
        rs2::frame depth = data.get_depth_frame().apply_filter(color_map);
        Mat depthStream(Size(640, 480), CV_8UC3, (void*)depth.get_data(), Mat::AUTO_STEP);
        imshow("Depth", depthStream);

        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }

  Mat imgHSV;

  cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
 
  Mat imgThresholded;

  inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
      
  //morphological opening (remove small objects from the foreground)
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

  //morphological closing (fill small holes in the foreground)
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

  imshow("Thresholded Image", imgThresholded); //show the thresholded image
  imshow("Original", imgOriginal); //show the original image

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "Exiting NOW!" << endl;
            break; 
       }
    }

   return 0;

}
