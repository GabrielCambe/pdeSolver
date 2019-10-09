#!/bin/bash

for i in $(seq 1 100);
do
	echo ./pdeSolver -ny 50 -nx 50 -i $i;
	echo gnuplot -c "pde3dAnimacaoBash.plg" arq.dat 50 50 "$i.png";

done