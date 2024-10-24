#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int KERNEL_SIZE = 11; // Não será mais input
Mat frame, dst;
char current_filter = 'a';  // Filtro inicial: média (blur)

void apply_filter(char filter) {
    switch (filter) {
        // Era pra escolher dois filtros. Porém, mais fácil "iniciar" o desafio e alternar entre todos eles de uma vez
        // dado que os comandos são os mesmos
        case 'a':  // Média
            blur(frame, dst, Size(KERNEL_SIZE, KERNEL_SIZE), Point(-1, -1));
            break;
        case 'g':  // Gaussiano
            GaussianBlur(frame, dst, Size(KERNEL_SIZE, KERNEL_SIZE), 0, 0);
            break;
        case 'm':  // Blur Mediano
            medianBlur(frame, dst, KERNEL_SIZE);
            break;
        case 'b':  // Bilateral
            bilateralFilter(frame, dst, KERNEL_SIZE, KERNEL_SIZE * 2, KERNEL_SIZE / 2);
            break;
        default:
            dst = frame.clone();  // Sem filtro
            break;
    }
}

int main() {
    VideoCapture cap(0);  // Captura da webcam
    if (!cap.isOpened()) {
        cerr << "Erro ao abrir a câmera!" << endl;
        return -1;
    }

    namedWindow("Filtro em Tempo Real", WINDOW_AUTOSIZE);

    while (true) {
        cap >> frame;  // Captura o frame atual da webcam
        if (frame.empty()) {
            cerr << "Erro ao capturar frame da câmera!" << endl;
            break;
        }

        apply_filter(current_filter);  // Aplica o filtro escolhido

        imshow("Filtro em Tempo Real", dst);  // Mostra o frame filtrado

        char key = (char)waitKey(30);  // Espera por uma tecla por 30ms
        if (key == 's') {  // Tecla 's' para salvar a imagem
            string filename = "imagem_wc_filtrada.jpg";
            imwrite(filename, dst);
            cout << "Imagem salva como " << filename << endl;
        } else if (key == 'a' || key == 'g' || key == 'm' || key == 'b' || key == 'o') {
            current_filter = key;  // Troca de filtro
        } else if (key == 27) {  // Tecla 'ESC' para sair. Estava me irritando ao tentar fechar a janela e ela reabrir
            break;
        }
    }

    cap.release(); // Finalizando captura de wc
    destroyAllWindows(); // Garantir que se encerre a janela
    return 0;
}
