/**
 * Code made by changing the:
 * @file bg_sub.cpp
 * @brief Background subtraction tutorial sample code
 * @author Domenico D. Bloisi
 */

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
      "{ input          | vtest.avi | Path to a video or a sequence of image }"
      "{ output         | output.mp4| Path to save the processed video }"
      "{ algo           | MOG2      | Background subtraction method (KNN, MOG2) }";

int main(int argc, char* argv[])
{
    CommandLineParser parser(argc, argv, params);
    parser.about( "This program shows how to use background subtraction methods provided by "
                  " OpenCV. You can process both videos and images.\n" );
    if (parser.has("help"))
    {
        //print help information
        parser.printMessage();
    }

    // Debug. Comentarei depois.
    //string inputFile = parser.get<String>("input");
    //string outputFile = parser.get<String>("output");
    //cout << "Input file: " << inputFile << endl;
    //cout << "Output file: " << outputFile << endl;
    // Nota: bug descoberto - bastava usar aspas nos inputs 

    //! [create]
    //create Background Subtractor objects
    Ptr<BackgroundSubtractor> pBackSub;
    if (parser.get<String>("algo") == "MOG2")
        pBackSub = createBackgroundSubtractorMOG2();
    else
        pBackSub = createBackgroundSubtractorKNN();
    //! [create]

    //! [capture]
    VideoCapture capture( samples::findFile( parser.get<String>("input") ) );
    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open: " << parser.get<String>("input") << endl;
        return 0;
    }
    //! [capture]

    //! [Write]
    string outputPath = parser.get<String>("output"); //Incluso nos inputs para não termos que compilar separadamente pra todos os vídeos
    int codec = VideoWriter::fourcc('m', 'p', '4', 'v'); // MPEG-4 codec
    double fps = capture.get(CAP_PROP_FPS);
    Size frameSize(
        (int)capture.get(CAP_PROP_FRAME_WIDTH), // Tamanho do arquivo salvo
        (int)capture.get(CAP_PROP_FRAME_HEIGHT)
    );
    VideoWriter videoWriter(outputPath, codec, fps, frameSize, true);
    
    if (!videoWriter.isOpened()) { // Garantindo sucesso de escrita
        cerr << "Error: Could not open the output video file for writing: " << outputPath << endl;
        return 0;
    }
    //! [Write]

    Mat frame, fgMask, fgMaskColor;
    while (true) {
        capture >> frame;
        if (frame.empty())
            break;

        //! [apply]
        //update the background model
        pBackSub->apply(frame, fgMask);
        //! [apply]

        //! [display_frame_number]
        //get the frame number and write it on the current frame
        rectangle(frame, cv::Point(10, 2), cv::Point(100,20),
                  cv::Scalar(255,255,255), -1);
        stringstream ss;
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNumberString = ss.str();
        putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
                FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));
        //! [display_frame_number]

        //Convertendo para compatibilizar
        cvtColor(fgMask, fgMaskColor, COLOR_GRAY2BGR);

        //! [show & save]
        //show the current frame and the fg masks
        imshow("Frame", frame);
        imshow("FG Mask", fgMask);
        videoWriter.write(fgMaskColor); // Salvando o resultado em vídeo
        //! [show & save]

        //get the input from the keyboard
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }

    capture.release(); //Estava faltando. Não necessário porém boa prática
    videoWriter.release(); //Boa prática

    return 0;
}
