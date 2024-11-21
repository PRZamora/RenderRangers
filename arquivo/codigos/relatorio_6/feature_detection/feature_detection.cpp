#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

const char* params
    = "{ help h         |           | Print usage }"
      "{ input          | image.png | Path to a image }"
      "{ output         | output.png| Path to save the processed image }"
      "{ algo           | SHI       | Feature Detection (SHI, FEAT) }";

int main(int argc, char* argv[])
{
	// Print help
	CommandLineParser parser(argc, argv, params);
    parser.about( "This program applies feature detection processing on images.\n" );
    if (parser.has("help"))
    {
        parser.printMessage();
    }	
	
	// Parse input arguments
	string inputPath  = parser.get<String>("input");
	string outputPath = parser.get<String>("output");
	string algoritm   = parser.get<String>("algo");
	
	// Read src image
	src = imread( samples::findFile(inputPath) );
	if( src.empty() )
    {
        cout << "Could not open or find the image!\n" << endl;
        cout << "Usage: " << argv[0] << " <Input image>" << endl;
        return -1;
    }
    
    // Turns to gray (single-channel)
    cvtColor( src, src_gray, COLOR_BGR2GRAY );
		
    return 0;
}