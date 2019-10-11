#!/bin/bash

for i in $(seq -f "%05g" 1 $3);
do
    ./pdeSolver -nx $1 -ny $2 -i $i -o arq.dat
    gnuplot -c "pde3dAnimacaoBash.plg" "arq.dat" $1 $2 "./frames/im$i.png"
done

ffmpeg -r 24 -f image2 -s 1920x1080 -i ./frames/im%05d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p -vframes $3 pdeAnim.mp4
if [[ $4 = "-remove-frames" ]]; then
    rm -r ./frames/*
fi
    
	     
xdg-open pdeAnim.mp4

