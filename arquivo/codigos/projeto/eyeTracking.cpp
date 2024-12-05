#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <ctime>

using namespace cv;
using namespace std;
using namespace chrono;

int main() {
    // Inicia captura de vídeo pela webcam
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Erro ao abrir a webcam!" << endl;
        return -1;
    }

    // Carrega o classificador Haar para detecção de olhos
    CascadeClassifier eye_cascade;
    if (!eye_cascade.load("haarcascade_eye.xml")) {
        cerr << "Erro ao carregar o classificador Haar para olhos." << endl;
        return -1;
    }

    Mat frame;
    string last_direction = "...";
    steady_clock::time_point last_update = steady_clock::now();
    bool debounce_active = false;

    // Definindo a folga central (tolerância)
    const int center_tolerance = 8; // Pixels para considerar o centro como neutro

    while (true) {
        cap >> frame; // Captura o quadro atual da webcam
        if (frame.empty()) break;

        // Pré-processamento da imagem
        Mat gray, equalized;
        cvtColor(frame, gray, COLOR_BGR2GRAY); // Conversão para escala de cinza
        equalizeHist(gray, equalized);         // Equalização do histograma
        GaussianBlur(equalized, equalized, Size(5, 5), 0); // Redução de ruído

        // Detecção de olhos
        vector<Rect> eyes;
        eye_cascade.detectMultiScale(equalized, eyes, 1.1, 6, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

        string direction = "...";

        for (size_t i = 0; i < eyes.size() && i < 1; i++) { // Processa no máximo 1 olho
            Rect eye = eyes[i];
            Mat eyeROI = equalized(eye);

            // Threshold para isolar a pupila
            Mat thresholded;
            threshold(eyeROI, thresholded, 50, 255, THRESH_BINARY_INV);

            // Localiza o centro de massa da pupila
            Moments m = moments(thresholded, true);
            if (m.m00 != 0) {
                int cx = int(m.m10 / m.m00);
                int cy = int(m.m01 / m.m00);

                int eye_center_x = eye.width / 2;

                // Determina a direção
                if (abs(cx - eye_center_x) <= center_tolerance) {
                    direction = "...";
                } else {
                    direction = (cx < eye_center_x) ? "SIM (Esquerda)" : "NAO (Direita)";
                }

                // Desenha retângulo e círculo
                rectangle(frame, eye, Scalar(255, 0, 0), 2);
                circle(frame, Point(eye.x + cx, eye.y + cy), 5, Scalar(0, 255, 0), -1);
            }
        }

        // Lógica de debounce para saída estável
        steady_clock::time_point now = steady_clock::now();
        if (direction != last_direction) {
            if (!debounce_active) {
                debounce_active = true;
                last_update = now;
            } else if (duration_cast<milliseconds>(now - last_update).count() >= 150) {
                last_direction = direction;
                debounce_active = false;
            }
        } else {
            debounce_active = false;
        }

        // Exibe resultados
        Mat display = Mat::zeros(100, 800, CV_8UC3);
        putText(display, last_direction, Point(50, 70), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 0), 3);
        imshow("Resultado", display);

        // Exibe o quadro da webcam
        imshow("Eye Tracking", frame);

        // Manipulação de teclas
        char key = waitKey(10);
        if (key == 'q') break; // Encerra a aplicação
        if (key == 's') { // Salva o quadro atual e a direção
            // Gera o nome do arquivo com timestamp
            auto t = system_clock::now();
            time_t now = system_clock::to_time_t(t);
            char timestamp[100];
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", localtime(&now));

            // Salva o quadro
            string filename = string("eye_tracking_") + timestamp + ".png";
            imwrite(filename, frame);

            cout << "Quadro salvo como " << filename << endl;
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
