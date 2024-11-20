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
      "{ output         | output.mp4| Path to save the processed video }"    
      "{ algo           | MOG2      | Background subtraction method (KNN, MOG2) }";

int main(int argc, char* argv[])
{
    CommandLineParser parser(argc, argv, params);
    parser.about("This program shows how to use background subtraction methods provided by OpenCV.");
    if (parser.has("help"))
    {
        parser.printMessage();
    }

    Ptr<BackgroundSubtractor> pBackSub;
    if (parser.get<String>("algo") == "MOG2")
        pBackSub = createBackgroundSubtractorMOG2();
    else
        pBackSub = createBackgroundSubtractorKNN();

    // Abrindo a wc
    VideoCapture capture(0);
    if (!capture.isOpened()){
        cerr << "Unable to open webcam" << endl;
        return 0;
    }

    // Inicializando o VideoWriter para gravar o vídeo
    string outputPath = parser.get<String>("output");
    string outputWebcamPath = "webcam_" + outputPath;
    int codec = VideoWriter::fourcc('m', 'p', '4', 'v'); // MPEG-4 codec
    double fps = capture.get(CAP_PROP_FPS);
    Size frameSize(
        (int)capture.get(CAP_PROP_FRAME_WIDTH),
        (int)capture.get(CAP_PROP_FRAME_HEIGHT)
    );
    
    // Saída 1: Sub
    VideoWriter videoWriter(outputPath, codec, fps, frameSize, true);
    // Saída 2: Webcam
    VideoWriter videoWriterWebcam(outputWebcamPath, codec, fps, frameSize, true);

    if (!videoWriter.isOpened()) {
        cerr << "Error: Could not open the output video file for writing: " << outputPath << endl;
        return 0;
    }

    Mat frame, fgMask, fgMaskColor;
    bool recording = false;

    while (true) {
        capture >> frame;
        if (frame.empty())
            break;

        pBackSub->apply(frame, fgMask);
        cvtColor(fgMask, fgMaskColor, COLOR_GRAY2BGR);

        imshow("Frame", frame);
        imshow("FG Mask", fgMaskColor); //Alterado para exibir diretamente o objeto pós conv de cor

        // Gravar o vídeo se a gravação estiver ativada
        if (recording) {
            videoWriter.write(fgMaskColor); // Gravar a máscara de fundo no arquivo de vídeo
            videoWriterWebcam.write(frame); // Gravar a imagem da wc
        }

        // Aguardar entrada do teclado
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)  // Fechar com 'q' ou 'Esc'
            break;
        else if (keyboard == 'r') {  // Iniciar/pausar gravação com 'r'
            recording = !recording;
            cout << (recording ? "Recording started" : "Recording stopped") << endl;
        }
    }

    capture.release();  // Libera a captura da webcam
    videoWriter.release();  // Libera o gravador de vídeo

    return 0;
}
