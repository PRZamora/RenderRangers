#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
 
using namespace cv;
using namespace std;

Mat src, dst, orig_thresh, equal_thresh;
int threshold_orig_value = 0;
int threshold_equal_value = 0;
const char* window_name_orig = "Limiarizacao - original";
const char* window_name_eq = "Limiarizacao - equalizada";

static void threshold_original(int, void*)
{
	threshold( src, orig_thresh, threshold_orig_value, 255, 0 );
    imshow( window_name_orig, orig_thresh );
}

static void threshold_equalized(int, void*)
{
	threshold( dst, equal_thresh, threshold_equal_value, 255, 0 );
    imshow( window_name_eq, equal_thresh );
}
 
int main( int argc, char** argv )
{	
    VideoCapture cap(argc > 1 ? atoi(argv[1]) : 0);
		 
	namedWindow( window_name_orig, WINDOW_AUTOSIZE ); // Create a window to display results
	namedWindow( window_name_eq, WINDOW_AUTOSIZE ); // Create a window to display results
	 
	createTrackbar( "Valor de Threshold:",
					window_name_orig, &threshold_orig_value,
					255, threshold_original ); // Create a Trackbar to choose Threshold value
					
	createTrackbar( "Valor de Threshold:",
					window_name_eq, &threshold_equal_value,
					255, threshold_equalized ); // Create a Trackbar to choose Threshold value
        
    while (true)
    {
        cap >> src;
        if(src.empty())
        {
            break;
        }			
	 
		cvtColor( src, src, COLOR_BGR2GRAY );
		equalizeHist( src, dst );
	 
		imshow( "Source image", src );
		imshow( "Equalized Image", dst );
						
		threshold_original(0, 0);
		threshold_equalized(0, 0);
		
        char key = (char) waitKey(30);
        if (key == 'q' || key == 27)
        {
			imwrite("original_grayscale.jpg", src);
			imwrite("grayscale_equalized.jpg", dst);
			
			imwrite("original_threshold.jpg", orig_thresh);
			imwrite("equalized_threshold.jpg", equal_thresh);
            break;
        }
	}
 
    return 0;
 
}
