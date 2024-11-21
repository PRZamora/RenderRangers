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
 
    int histSize = 256;
 
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange[] = { range };
 
    bool uniform = true, accumulate = false;
 
    Mat original_hist, equalized_hist; 
    
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
 
    Mat histOriginal( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    Mat histEqualized( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    
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
			
			calcHist( &src, 1, 0, Mat(), original_hist, 1, &histSize, histRange, uniform, accumulate );
			calcHist( &dst, 1, 0, Mat(), equalized_hist, 1, &histSize, histRange, uniform, accumulate );
 
			normalize(original_hist, original_hist, 0, histOriginal.rows, NORM_MINMAX, -1, Mat() );
			normalize(equalized_hist, equalized_hist, 0, histOriginal.rows, NORM_MINMAX, -1, Mat() );
			
			for( int i = 1; i < histSize; i++ )
			{
				line( histOriginal, Point( bin_w*(i-1), hist_h - cvRound(original_hist.at<float>(i-1)) ),
					  Point( bin_w*(i), hist_h - cvRound(original_hist.at<float>(i)) ),
					  Scalar( 255, 0, 0), 2, 8, 0  );
				line( histEqualized, Point( bin_w*(i-1), hist_h - cvRound(equalized_hist.at<float>(i-1)) ),
					  Point( bin_w*(i), hist_h - cvRound(equalized_hist.at<float>(i)) ),
					  Scalar( 0, 255, 0), 2, 8, 0  );
			}
			
			imwrite("original_histogram.jpg", histOriginal);
			imwrite("equalized_histogram.jpg", histEqualized);
			
            break;
        }
	}
 
    return 0;
 
}
