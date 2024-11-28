#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

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
    const int center_tolerance = 8; // Pixel de tolerância para considerar o centro como neutro

    while (true) {
        cap >> frame; // Captura o quadro atual da webcam
        if (frame.empty()) break;

        // Pré-processamento da imagem
        Mat gray, equalized, thresholded;

        // 1. Conversão para escala de cinza (Processamento de cores)
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        // 2. Equalização do histograma para melhorar o contraste
        equalizeHist(gray, equalized);

        // 3. Filtragem para redução de ruído
        GaussianBlur(equalized, equalized, Size(5, 5), 0);

        // Detecção de olhos
        vector<Rect> eyes;
        eye_cascade.detectMultiScale(equalized, eyes, 1.1, 6, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

        string direction = "...";

        // for (size_t i = 0; i < eyes.size() && i < 1; i++) { // Processa no máximo 1 olho
            Rect eye = eyes[0];

            // Recorta a região do olho
            Mat eyeROI = equalized(eye);

            // Subtração de fundo para identificar a pupila (assumindo intensidade mais escura)
            threshold(eyeROI, thresholded, 25, 255, THRESH_BINARY_INV);

            vector<Vec3f> circles;
            HoughCircles(thresholded, circles, HOUGH_GRADIENT, 1,
                        thresholded.rows,  // change this value to detect circles with different distances to each other
                        30, 10, thresholded.rows/12, thresholded.rows/6 // change the last two parameters (min_radius & max_radius) to detect larger circles
            );

            Mat canniedImage;
            Canny(thresholded, canniedImage, 10, 30, 3);

            for( size_t i = 0; i < circles.size(); i++ )
            {
                Vec3i c = circles[i];
                Point center = Point(c[0], c[1]);
                // circle center
                circle( thresholded, center, 1, Scalar(0,100,100), 3, LINE_AA);
                // circle outline
                int radius = c[2];
                circle( thresholded, center, radius, Scalar(255,0,255), 3, LINE_AA);
            }

			imshow("imagem thresholdada", thresholded);
            imshow("imagem cannyada", canniedImage);

            // Localização de pontos de destaque (centro de massa da pupila)
            Moments m = moments(thresholded, true);
            if (m.m00 != 0) {
                int cx = int(m.m10 / m.m00); // Coordenada X do centro
                int cy = int(m.m01 / m.m00); // Coordenada Y do centro

                // Determina a direção com base na posição da pupila em relação ao centro do olho
                int eye_center_x = eye.width / 2;

                // Se a pupila estiver dentro da zona central, mantém como "..."
                if (abs(cx - eye_center_x) <= center_tolerance) {
                    direction = "...";
                } else {
                    direction = (cx < eye_center_x) ? "Esquerda" : "Direita";
                }

                // Desenha o retângulo do olho e o ponto da pupila
                rectangle(frame, eye, Scalar(255, 0, 0), 2);
                circle(frame, Point(eye.x + cx, eye.y + cy), 5, Scalar(0, 255, 0), -1);
            }
        // }

        // Controle de debounce para estabilidade do resultado
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

        // Exibe o resultado em um quadro separado
        Mat display = Mat::zeros(100, 400, CV_8UC3);
        putText(display, last_direction, Point(50, 70), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 0), 3);
        imshow("Resultado", display);

        // Exibe o quadro processado
        imshow("Eye Tracking", frame);

        // Encerra se a tecla 'q' for pressionada
        if (waitKey(10) == 'q') break;
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
