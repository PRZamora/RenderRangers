#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
 
using namespace cv;
 
Mat src, src_gray, src_hsv;
Mat dst, detected_edges;

const int max_lowThreshold = 100;
const int ratio = 3;
const int kernel_size = 3;
const char* window_capture_name = "Original Image";
const char* window_border_name = "Edge Map";
int lowThreshold = 0;
bool shouldRecord = false;

static void CannyThreshold(int, void*)
{
    GaussianBlur( src_gray, detected_edges, Size(3,3), 0, 0 );
 
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
 
    dst = Scalar::all(0);
 
    src.copyTo( dst, detected_edges);
 
    imshow( window_border_name, dst );
}
 
int main(int argc, char* argv[])
{
    VideoCapture cap(argc > 1 ? atoi(argv[1]) : 0);// Initialize video writer object
    
    // Acquire frame width and height with the help of get() method
    // You can replace 3 and 4 with CAP_PROP_FRAME_WIDTH and CAP_PROP_FRAME_HEIGHT
    // They are just enumerations
	int frame_width = static_cast<int>(cap.get(3));
	int frame_height = static_cast<int>(cap.get(4));

	//define frame_size to be used in VideoWriter() argument
    Size frame_size(frame_width, frame_height);
	int fps = 20;
    
	VideoWriter original_output;//("original_video.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, frame_size);
	VideoWriter canny_output;//("canny_video.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, frame_size);
	
    namedWindow( window_border_name, WINDOW_AUTOSIZE );   
 
    createTrackbar( "Min Threshold:", window_border_name, &lowThreshold, max_lowThreshold, CannyThreshold );
 
    while (true) {
        cap >> src;
        if(src.empty())
        {
            break;
        }
 
        // Convert from BGR to grayscale colorspace
        cvtColor(src, src_gray, COLOR_BGR2GRAY);
		// Create something for the canny
        dst.create( src.size(), src.type() );
        
        CannyThreshold(0, 0);
 
        // Show the frames
        imshow(window_capture_name, src);
 
        char key = (char) waitKey(30);
        
        if (key == 'q' || key == 27)
        {
            break;
        }
        
        switch (key)
        {
			case 'k':
				original_output.open("original_video.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, frame_size);
				canny_output.open("canny_video.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, frame_size);
				shouldRecord = true;
				break;				
			case 'h':
				shouldRecord = false;
				break;
			case 's':
				imwrite("original_image.jpg", src);
				imwrite("canny_detected.jpg", dst);
				break;
		}
		
		if (shouldRecord)
		{
			original_output.write(src);
			canny_output.write(dst);
		}
    }
    
	original_output.release();
	canny_output.release();
    return 0;
}
