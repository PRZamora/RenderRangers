#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
 
using namespace cv;
 
const int max_value_H = 360/2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";

const cv::Scalar green_max_value = Scalar(51, 189, 229);
const cv::Scalar green_min_value = Scalar(38, 99, 159);

const cv::Scalar red_max_value = Scalar(25, 249, 202);
const cv::Scalar red_min_value = Scalar(0, 180, 83);

const cv::Scalar blue_max_value = Scalar(119, 217, 255);
const cv::Scalar blue_min_value = Scalar(101, 84, 164);
 
int main(int argc, char* argv[])
{
    VideoCapture cap(argc > 1 ? atoi(argv[1]) : 0);
 
    namedWindow(window_capture_name);
 
    Mat frame, frame_HSV, blurred_frame_HSV, blue_frame, green_frame, red_frame;
    while (true) {
        cap >> frame;
        if(frame.empty())
        {
            break;
        }
 
        // Convert from BGR to HSV colorspace
        cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
        // Apply a Gaussian Blur to the HSV frame
        GaussianBlur(frame_HSV, blurred_frame_HSV, Size(3,3), 0, 0);
        
        // Detect objects within the blue HSV color range
        inRange(frame_HSV, blue_min_value, blue_max_value, blue_frame);
        
        // Detect objects within the green HSV color range
        inRange(frame_HSV, green_min_value, green_max_value, green_frame);
        
        // Detect objects within the red HSV color range
        inRange(frame_HSV, red_min_value, red_max_value, red_frame);
 
        // Show the frames
        imshow(window_capture_name, frame);
        imshow("Blue Detection", blue_frame);
        imshow("Green Detection", green_frame);
        imshow("Red Detection", red_frame);
 
        char key = (char) waitKey(30);
        if (key == 'q' || key == 27)
        {
			
			Mat red_result = frame.clone();
			red_result.setTo( Scalar( 0, 0, 0 ), ~red_frame);
			
			Mat green_result = frame.clone();
			green_result.setTo( Scalar( 0, 0, 0 ), ~green_frame);
			
			Mat blue_result = frame.clone();
			blue_result.setTo( Scalar( 0, 0, 0 ), ~blue_frame);
			
			
			imwrite("original_image.jpg", frame);
			imwrite("blue_object.jpg", blue_result);
			imwrite("green_object.jpg", green_result);
			imwrite("red_object.jpg", red_result);
            break;
        }
    }
    return 0;
}
