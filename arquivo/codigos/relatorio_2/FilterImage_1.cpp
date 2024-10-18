#include <iostream>
#include <sstream>
#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
 
using namespace std;
using namespace cv;

Mat src; Mat dst;

int main ()
{
	int KERNEL_SIZE;
	
	std::cin >> KERNEL_SIZE;
	
	if(std::cin.fail()) {
			std::cout << "Invalid Input." << std::endl;
			return -1;
	}
	
	std::stringstream ss;
	ss << KERNEL_SIZE;
	
	std::string image_path = samples::findFile("lab1_grupo_rgb.jpeg");
    Mat src = imread(image_path, IMREAD_COLOR);
 
    if(src.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    dst = src.clone();
	
	blur(src, dst, Size( KERNEL_SIZE, KERNEL_SIZE), Point(-1,-1));
	imwrite(ss.str() + "x" + ss.str() + "_Blur.jpg", dst);
	
	GaussianBlur( src, dst, Size( KERNEL_SIZE, KERNEL_SIZE ), 0, 0 );
	imwrite(ss.str() + "x" + ss.str() + "_GaussianBlur.jpg", dst);
	
	medianBlur ( src, dst, KERNEL_SIZE );
	imwrite(ss.str() + "x" + ss.str() + "_MedianBlur.jpg", dst);
	
	bilateralFilter ( src, dst, KERNEL_SIZE, KERNEL_SIZE*2, KERNEL_SIZE/2 );
	imwrite(ss.str() + "x" + ss.str() + "_Bilateral.jpg", dst);
	
	return 0;
	
}

/* 
int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;
 
Mat src; Mat dst;
char window_name[] = "Smoothing Demo";
 
int display_caption( const char* caption );
int display_dst( int delay );
 
 
int main( int argc, char ** argv )
{
    namedWindow( window_name, WINDOW_AUTOSIZE );
 
    const char* filename = argc >=2 ? argv[1] : "lab1_grupo_rgb.jpeg";
 
    src = imread( samples::findFile( filename ), IMREAD_COLOR );
    if (src.empty())
    {
        printf(" Error opening image\n");
        printf(" Usage:\n %s [image_name-- default lab1_grupo_rgb.jpeg] \n", argv[0]);
        return EXIT_FAILURE;
    }
 
    if( display_caption( "Original Image" ) != 0 )
    {
        return 0;
    }
 
    dst = src.clone();
    if( display_dst( DELAY_CAPTION ) != 0 )
    {
        return 0;
    }
 
    if( display_caption( "Homogeneous Blur" ) != 0 )
    {
        return 0;
    }
 
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {
        blur( src, dst, Size( i, i ), Point(-1,-1) );
        if( display_dst( DELAY_BLUR ) != 0 )
        {
            return 0;
        }
    }
 
    if( display_caption( "Gaussian Blur" ) != 0 )
    {
        return 0;
    }
 
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {
        GaussianBlur( src, dst, Size( i, i ), 0, 0 );
        if( display_dst( DELAY_BLUR ) != 0 )
        {
            return 0;
        }
    }
 
    if( display_caption( "Median Blur" ) != 0 )
    {
        return 0;
    }
 
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {
        medianBlur ( src, dst, i );
        if( display_dst( DELAY_BLUR ) != 0 )
        {
            return 0;
        }
    }
 
    if( display_caption( "Bilateral Blur" ) != 0 )
    {
        return 0;
    }
 
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {
        bilateralFilter ( src, dst, i, i*2, i/2 );
        if( display_dst( DELAY_BLUR ) != 0 )
        {
            return 0;
        }
    }
 
    display_caption( "Done!" );
 
    return 0;
}
 
int display_caption( const char* caption )
{
    dst = Mat::zeros( src.size(), src.type() );
    putText( dst, caption,
             Point( src.cols/4, src.rows/2),
             FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255) );
 
    return display_dst(DELAY_CAPTION);
}
 
int display_dst( int delay )
{
    imshow( window_name, dst );
    int c = waitKey ( delay );
    if( c >= 0 ) { return -1; }
    return 0;
}
*/
