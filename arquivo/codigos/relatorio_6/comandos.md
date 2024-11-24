# Comandos para execução dos labs

## Item 1
### Grupo:
```shell
    .\Debug\feature_detection.exe --input="lab1_grupo_rgb.jpeg" --output="feature_shi_grupo.png" --algo="SHI"
```
```shell
    .\Debug\feature_detection.exe --input="lab1_grupo_rgb.jpeg" --output="feature_surf_grupo.png" --algo="SURF"
```

### Objeto

```shell
    .\Debug\feature_detection.exe --input="original_grayscale.jpg" --output="feature_shi_obj.png" --algo="SHI"
```
```shell
    .\Debug\feature_detection.exe --input="original_grayscale.jpg" --output="feature_surf_obj.png" --algo="SURF"
```


(Remover diretório debug se compilado no linux do lab)

## Item 2 (Alterado para gravar Webcam. Apertar s para salvar imagem atual)

```shell
    .\Debug\feature_detection_webcam.exe --algo="SHI"
```
```shell
    .\Debug\feature_detection_webcam.exe --algo="SURF"
```
