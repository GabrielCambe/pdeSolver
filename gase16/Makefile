.PHONY: plot3d clean dist-clean doc

INC = ./inc
SRC = ./src
cFLAGS = -I$(INC) -Wall -O0 -g
objFLAGS = $(cFLAGS) -c
OBJETOs = leitura.o escrita.o sistema.o metodo.o
libFLAGS = -lm

# all: compila e produz um executável chamado pdeSolver no diretório login1-login2/;
all: dist-clean leitura escrita sistema metodo
	gcc -o pdeSolver $(cFLAGS) $(OBJETOs) $(libFLAGS) $(SRC)/pdeSolver.c

leitura:
	gcc $(objFLAGS) $(SRC)/leitura.c

escrita:
	gcc $(objFLAGS) $(SRC)/escrita.c

sistema:
	gcc $(objFLAGS) $(SRC)/sistema.c

metodo:
	gcc $(objFLAGS) $(SRC)/metodo.c

# gnuplot -c "pde3d.plg" "arq.dat" $${NX} $${NY}
plot3d:
	ARQDAT=$$(cat arqdat) && \
	NX=$$(cat nx) && NY=$$(cat ny) && \
	gnuplot -c "pde3d.plg" "$${ARQDAT}" $${NX} $${NY}

# clean: remove todos os arquivos temporários e os arquivos gerados pelo Makefile (*.o, executável, etc.).
clean:
	rm *.o || true
	rm *~ || true
 
dist-clean: clean
	rm nx || true
	rm ny || true
	ARQDAT=$$(cat arqdat) && \
	rm $${ARQDAT}
	rm arqdat || true
	
clean-docs:
	rm -rf ./doc/ || true

purge: clean dist-clean clean-docs
	rm pdeSolver || true

# doc: gera a documentação Doxygen em formato html
doc:
	doxygen Doxyfile
