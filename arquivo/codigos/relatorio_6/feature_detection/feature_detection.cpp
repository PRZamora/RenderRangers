#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>

// Para o SURF:
#include <opencv2/features2d.hpp>
#ifdef HAVE_OPENCV_XFEATURES2D // Esse método requisita essa dep pra funcionar. Mantidas as tratativas feitas no código-tutorial
#include <opencv2/xfeatures2d.hpp>
#endif

using namespace cv;
using namespace std;

// Command-line parameters
const char* params =
    "{ help h         |           | Print usage }"
    "{ input          | image.png | Path to an image }"
    "{ output         | output.png| Path to save the processed image }"
    "{ algo           | SHI       | Feature Detection (SHI para cantos, SURF para robustez) }";

// Placeholder for Shi-Tomasi tracking
void shiTrack(const Mat& src_gray, const string& outputPath)
{
    Mat dst = src_gray.clone();
    vector<Point2f> corners;
    goodFeaturesToTrack(dst, corners, 100, 0.01, 10);
    for (const auto& corner : corners)
    {
        circle(dst, corner, 5, Scalar(255), -1); // Draw corners
    }
    cout << "Shi-Tomasi detection completed.\n";

    // Save result
    if (!imwrite(outputPath, dst))
    {
        cerr << "Failed to save Shi-Tomasi result to " << outputPath << "\n";
    }
}

// Placeholder for feature tracking using SURF
void surfTrack(const Mat& src_gray, const string& outputPath)
{
#ifdef HAVE_OPENCV_XFEATURES2D
    int minHessian = 400;
    Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create(minHessian);
    std::vector<KeyPoint> keypoints;
    detector->detect(src_gray, keypoints);

    Mat img_keypoints;
    drawKeypoints(src_gray, keypoints, img_keypoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    imshow("SURF Keypoints", img_keypoints);
    waitKey(); // Wait for a key press

    // Save result
    if (!imwrite(outputPath, img_keypoints))
    {
        cerr << "Failed to save SURF result to " << outputPath << "\n";
    }
#else
    cerr << "SURF functionality needs the xfeatures2d contrib module to be run.\n"; // Esse método requisita essa dep pra funcionar. Mantidas as tratativas feitas no código-tutorial
#endif
}

int main(int argc, char* argv[])
{
    // Parse command-line arguments
    CommandLineParser parser(argc, argv, params);
    parser.about("This program applies feature detection processing on images.\n");
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    // Args de entrada
    string inputPath = parser.get<String>("input");
    string outputPath = parser.get<String>("output");
    string algorithm = parser.get<String>("algo");

    // Source
    Mat src = imread(samples::findFile(inputPath));
    if (src.empty())
    {
        cerr << "Could not open or find the image!\n";
        return -1;
    }

    // Conversão pra cinza
    Mat src_gray;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    // Seleção do argo
    if (algorithm == "SHI")
    {
        cout << "SHI chosen...\n";
        shiTrack(src_gray, outputPath);
    }
    else if (algorithm == "SURF")
    {
        cout << "SHI chosen...\n";
        surfTrack(src_gray, outputPath);
    }
    else
    {
        cerr << "Invalid algorithm specified! Use SHI or SURF.\n";
        return -1;
    }

    cout << "Processed image saved to " << outputPath << "\n";
    return 0;
}
