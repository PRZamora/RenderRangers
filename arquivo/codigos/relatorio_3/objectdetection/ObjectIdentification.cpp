#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
 
using namespace cv;
 
const int max_value_H = 360/2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";

const cv::Scalar green_max_value = Scalar(106, 153, 149);
const cv::Scalar green_min_value = Scalar(63, 43, 82);

const cv::Scalar red_max_value = Scalar(24, 206, 255);
const cv::Scalar red_min_value = Scalar(0, 89, 157);

const cv::Scalar blue_max_value = Scalar(112, 198, 255);
const cv::Scalar blue_min_value = Scalar(100, 115, 190);
 
int main(int argc, char* argv[])
{
    VideoCapture cap(argc > 1 ? atoi(argv[1]) : 0);
 
    namedWindow(window_capture_name);
    namedWindow(window_detection_name);
 
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
			// resize(blue_frame, blue_frame, frame.size(), INTER_LINEAR);
			// resize(green_frame, green_frame, frame.size(), INTER_LINEAR);
			// resize(red_frame, red_frame, frame.size(), INTER_LINEAR);
			
			cv::bitwise_and(frame, frame, blue_frame, blue_frame);
			cv::bitwise_and(frame, frame, green_frame, green_frame);
			cv::bitwise_and(frame, frame, red_frame, red_frame);
			
			imwrite("original_image.jpg", frame);
			imwrite("blue_object.jpg", blue_frame);
			imwrite("green_object.jpg", green_frame);
			imwrite("red_object.jpg", red_frame);
            break;
        }
    }
    return 0;
}
