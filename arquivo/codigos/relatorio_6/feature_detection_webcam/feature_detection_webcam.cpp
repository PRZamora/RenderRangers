#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>

// Para o SURF:
#include <opencv2/features2d.hpp>
#ifdef HAVE_OPENCV_XFEATURES2D // Esse método requisita essa dep pra funcionar. Mantidas as tratativas feitas no código-tutorial
#include <opencv2/xfeatures2d.hpp> //Ps.: para instalar no windows, só recompilando o OpenCV com o OpenCV_Contrib
#endif


using namespace cv;
using namespace std;

void shiTrack(const Mat& src_gray, Mat& dst)
{
    vector<Point2f> corners;
    goodFeaturesToTrack(src_gray, corners, 100, 0.01, 10);
    dst = src_gray.clone();
    for (const auto& corner : corners)
    {
        circle(dst, corner, 5, Scalar(255), -1);
    }
    //cout << "Shi-Tomasi detection completed.\n";
}

void surfTrack(const Mat& src_gray, Mat& dst)
{
#ifdef HAVE_OPENCV_XFEATURES2D
    int minHessian = 400;
    Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create(minHessian);
    vector<KeyPoint> keypoints;
    detector->detect(src_gray, keypoints);
    drawKeypoints(src_gray, keypoints, dst, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
#else
    cerr << "SURF functionality needs the xfeatures2d contrib module to be run.\n";
#endif
}

const char* params = 
    "{ help h         |           | Print usage }"
    "{ algo           | SHI       | Feature Detection algorithm (SHI or SURF) }";

int main(int argc, char* argv[])
{
    CommandLineParser parser(argc, argv, params);
    parser.about("Feature Detection with Webcam Input\n");

    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    string algorithm = parser.get<string>("algo");
    if (algorithm != "SHI" && algorithm != "SURF")
    {
        cerr << "Invalid algorithm specified! Use SHI or SURF.\n";
        return -1;
    }

    // Processamento ao vivo via webcam
    VideoCapture cap(0); // Captura da webcam padrão
    if (!cap.isOpened())
    {
        cerr << "Error: Unable to access the webcam!\n";
        return -1;
    }

    cout << "Pressione 'q' para sair. Pressione 's' para salvar as imagens.\n";

    Mat frame, grayFrame, processedFrame;
    while (true)
    {
        cap >> frame; // Captura um frame
        if (frame.empty())
        {
            cerr << "Error: Frame vazio capturado. Finalizando...\n";
            break;
        }

        // Conversão para escala de cinza
        cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

        // Processamento baseado no algoritmo selecionado
        if (algorithm == "SHI")
        {
            shiTrack(grayFrame, processedFrame);
        }
        else if (algorithm == "SURF")
        {
            surfTrack(grayFrame, processedFrame);
        }    
        else
        {
            cerr << "Invalid algorithm specified! Use SHI or SURF.\n";
            return -1;
        }

        // Mostrar frames
        imshow("Webcam Input", frame);
        imshow("Processed Features", processedFrame);

        // Verificar teclas
        char key = (char)waitKey(30);
        if (key == 'q' || key == 27)
        {
            cout << "Exiting...\n";
            break;
        }
        else if (key == 's')
        {
            string originalPath = "original_image.png";
            string processedPath = "processed_image.png";

            imwrite(originalPath, frame);
            imwrite(processedPath, processedFrame);
        }
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
