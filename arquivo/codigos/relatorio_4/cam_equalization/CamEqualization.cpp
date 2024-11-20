#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
 
using namespace cv;
using namespace std;
 
int main( int argc, char** argv )
{	
    VideoCapture cap(argc > 1 ? atoi(argv[1]) : 0);
    Mat src, dst;
    
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
		
        char key = (char) waitKey(30);
        if (key == 'q' || key == 27)
        {
			imwrite("original_grayscale.jpg", src);
			imwrite("grayscale_equalized.jpg", dst);
            break;
        }
	}
 
    return 0;
 
}
