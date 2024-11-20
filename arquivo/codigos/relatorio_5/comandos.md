## Comandos para execução dos labs

# Item 0 (Código Original)
```shell
    .\Debug\bg_sub MOG2
```
```shell
    .\Debug\bg_sub KNN
```
ou
```shell
    .\Debug\bg_sub --input="vtest.avi" --algo="MOG2"
```
```shell
    .\Debug\bg_sub --input="vtest.avi" --algo="KNN"
```

# Item 1 (Adicionada função de salvar vídeo)
```shell
    .\Debug\bg_sub_video.exe --input="lab1_obj_fast.mp4" --output="lab5_movi_obj_fast.mp4"
```
```shell
    .\Debug\bg_sub_video.exe --input="lab1_obj_slow.mp4" --output="lab5_movi_obj_slow.mp4"
```
```shell
    .\Debug\bg_sub_video.exe --input="lab1_video_fast.mp4" --output="lab5_movi_video_fast.mp4"
```
```shell
    .\Debug\bg_sub_video.exe --input="lab1_video_slow.mp4" --output="lab5_movi_video_slow.mp4"
```
(Remover diretório debug se compilado no linux do lab)

# Item 2 (Alterado para gravar Webcam. Apertar r para iniciar gravação)
```shell
    .\Debug\bg_sub_webcam.exe --output="bg_sub_renato.mp4"
```
```shell
    .\Debug\bg_sub_webcam.exe --output="bg_sub_joao.mp4"
```
```shell
    .\Debug\bg_sub_webcam.exe --output="bg_sub_pedro.mp4"
```
```shell
    .\Debug\bg_sub_webcam.exe --output="bg_sub_objeto.mp4"
```