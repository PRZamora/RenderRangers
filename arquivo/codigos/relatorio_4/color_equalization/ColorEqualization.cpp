#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core.hpp>
#include <iostream>
 
using namespace cv;
using namespace std;
 
int main( int argc, char** argv )
{	
    VideoCapture cap(argc > 1 ? atoi(argv[1]) : 0);
    Mat src, eq_r, eq_g, eq_b, orig_r, orig_g, orig_b;
 
    vector<Mat> bgr_planes;
 
    int histSize = 256;
 
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange[] = { range };
 
    bool uniform = true, accumulate = false;
 
    Mat original_hist, equalized_hist;
    Mat original_image, equalized_image;
    
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
 
    Mat histOriginal( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    Mat histEqualized( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) ); 
    Mat b_hist, g_hist, r_hist, be_hist, ge_hist, re_hist;
    
    while (true)
    {
        cap >> src;
        if(src.empty())
        {
            break;
        }
        
		split( src, bgr_planes );
	 
		equalizeHist( &bgr_planes[0], eq_b );
		equalizeHist( &bgr_planes[1], eq_g );
		equalizeHist( &bgr_planes[2], eq_r );
		
        char key = (char) waitKey(30);
        if (key == 'q' || key == 27)
        {			
			calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
			calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate );
			calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );
			
			calcHist( eq_b, 1, 0, Mat(), be_hist, 1, &histSize, histRange, uniform, accumulate );
			calcHist( eq_g, 1, 0, Mat(), ge_hist, 1, &histSize, histRange, uniform, accumulate );
			calcHist( eq_r, 1, 0, Mat(), re_hist, 1, &histSize, histRange, uniform, accumulate );
 
			normalize(b_hist, b_hist, 0, histOriginal.rows, NORM_MINMAX, -1, Mat() );
			normalize(g_hist, g_hist, 0, histOriginal.rows, NORM_MINMAX, -1, Mat() );
			normalize(r_hist, r_hist, 0, histOriginal.rows, NORM_MINMAX, -1, Mat() );
 
			normalize(be_hist, be_hist, 0, histEqualized.rows, NORM_MINMAX, -1, Mat() );
			normalize(ge_hist, ge_hist, 0, histEqualized.rows, NORM_MINMAX, -1, Mat() );
			normalize(re_hist, re_hist, 0, histEqualized.rows, NORM_MINMAX, -1, Mat() );
			
			for( int i = 1; i < histSize; i++ )
			{
				line( histOriginal, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
					  Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
					  Scalar( 255, 0, 0), 2, 8, 0  );
				line( histOriginal, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
					  Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
					  Scalar( 0, 255, 0), 2, 8, 0  );
				line( histOriginal, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
					  Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
					  Scalar( 0, 0, 255), 2, 8, 0  );
					  
				line( histEqualized, Point( bin_w*(i-1), hist_h - cvRound(be_hist.at<float>(i-1)) ),
					  Point( bin_w*(i), hist_h - cvRound(be_hist.at<float>(i)) ),
					  Scalar( 255, 0, 0), 2, 8, 0  );
				line( histEqualized, Point( bin_w*(i-1), hist_h - cvRound(ge_hist.at<float>(i-1)) ),
					  Point( bin_w*(i), hist_h - cvRound(ge_hist.at<float>(i)) ),
					  Scalar( 0, 255, 0), 2, 8, 0  );
				line( histEqualized, Point( bin_w*(i-1), hist_h - cvRound(re_hist.at<float>(i-1)) ),
					  Point( bin_w*(i), hist_h - cvRound(re_hist.at<float>(i)) ),
					  Scalar( 0, 0, 255), 2, 8, 0  );
			}
			
			Mat eq_channels[3] = {eq_b, eq_g, eq_r};
			merge(eq_channels, 3, equalized_image);
			merge(bgr_planes, 3, original_image);
			
			imshow("Imagem original", original_image);
			imshow("Imagem equalizada", equalized_image);
			
			imwrite("original_histogram.jpg", histOriginal);
			imwrite("equalized_histogram.jpg", histEqualized);
			
			imwrite("original_image.jpg", original_image);
			imwrite("equalized_image.jpg", equalized_image);
			
            break;
        }
	}
 
    return 0;
 
}
