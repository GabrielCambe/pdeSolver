#!/bin/bash

for i in $(seq -f "%05g" 1 $3);
do
    if [[ -f "./animate/frames/im$i.png" ]]; then
	echo "./animate/frames/im$i.png exists..."
    else
	./pdeSolver -nx $1 -ny $2 -i $i -o arq.dat
	gnuplot -c "./animate/pde3dAnimacaoBash.plg" "arq.dat" $1 $2 "./animate/frames/im$i.png"
    fi
done

ffmpeg -r 24 -f image2 -s 1920x1080 -i ./animate/frames/im%05d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p -vframes $3 ./animate/pdeAnim.mp4
if [[ $4 = "-remove-frames" ]]; then
    rm -r ./animate/frames/*
fi
    
	     
xdg-open ./animate/pdeAnim.mp4

