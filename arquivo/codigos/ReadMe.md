## Notas sobre execução no Windows

1. **Instalar o Visual Studio**  
   Certifique-se de que o Visual Studio está instalado no seu sistema.

2. **Adicionar as dependências para C++ no Visual Studio**  
   Durante a instalação ou através do instalador, inclua os pacotes necessários para o desenvolvimento em C++.

3. **Instalar o CMake**  
   Baixe e instale o CMake, garantindo que ele esteja no PATH do sistema.

4. **Compilar o OpenCV com os módulos de contribuição**  
   Alguns itens requerem os módulos adicionais do OpenCV. Para isso, não instale o OpenCV diretamente; será necessário compilá-lo:

   ```shell
   git clone https://github.com/opencv/opencv.git
   git clone https://github.com/opencv/opencv_contrib.git
   cmake -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules ../opencv
   cmake --build . --config Release  # Windows
    ```