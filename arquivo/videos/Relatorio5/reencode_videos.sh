#!/bin/bash
# Correção de incompatibilidades de codec no navegador testado
# Vídeos originais funcionam normalmente no VLC
for file in *.mp4; do
    # Get the base name of the file (without extension)
    filename=$(basename "$file" .mp4)
    
    # Re-encode the video
    ffmpeg -i "$file" -c:v libx264 -c:a aac -strict experimental "${filename}_reencoded.mp4"
    
    echo "Re-encoded: $file"
done