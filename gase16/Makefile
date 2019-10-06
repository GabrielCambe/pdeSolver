cFLAGS = -Wall -O0 -lm -g
objFLAGS = $(cFLAGS) -c

# all: compila e produz um executável chamado pdeSolver no diretório login1-login2/;
all: leitura escrita sistema
	gcc -o pdeSolver  leitura.o escrita.o sistema.o pdeSolver.c $(cFLAGS)

leitura:
	gcc $(objFLAGS) leitura.c 

escrita:
	gcc $(objFLAGS) escrita.c

sistema:
	gcc $(objFLAGS) sistema.c

plot2d:
	gnuplot -e "solucao='arq.dat';" pde2d.plg

plot3d:
	NX=$$(cat nx) && NY=$$(cat ny) && \
	gnuplot -c "pde3d.plg" "arq.dat" $${NX} $${NY}

# clean: remove todos os arquivos temporários e os arquivos gerados pelo Makefile (*.o, executável, etc.).
clean:
	rm *.o || true
	rm *~ || true
	rm pdeSolver

# doc: gera a documentação Doxygen em formato html
doc:
	echo GeraDoc
