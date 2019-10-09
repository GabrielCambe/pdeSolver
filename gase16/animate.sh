#!/bin/bash

for i in $(seq 101 320);
do
	./pdeSolver -nx 50 -ny 50 -i $i -o arq.dat;
	gnuplot -c "pde3dAnimacaoBash.plg" "arq.dat" 50 50 "$i.png";

done
