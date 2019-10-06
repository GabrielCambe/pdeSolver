cFLAGS = -Wall -O0 -lm -g

# all: compila e produz um executável chamado pdeSolver no diretório login1-login2/;
all: leitura escrita sistema
	gcc -o pdeSolver  leitura.o escrita.o sistema.o pdeSolver.c $(cFLAGS)

leitura:
	gcc -c leitura.c

escrita:
	gcc -c escrita.c

sistema:
	gcc -c sistema.c

plot2d:
	gnuplot -e "solucao='arq.dat'" pde2d.plg

plot3d:
	gnuplot -e "solucao='arq.dat'" pde3d.plg

# clean: remove todos os arquivos temporários e os arquivos gerados pelo Makefile (*.o, executável, etc.).
clean:
	rm *.o || true
	rm *~ || true
	rm pdeSolver

# doc: gera a documentação Doxygen em formato html
doc:
	echo GeraDoc
