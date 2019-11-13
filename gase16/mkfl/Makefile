.PHONY: clean dist-clean doc plot3d
.ONESHELL: plot3d

INC = ./inc
SRC = ./src
cFLAGS = -I$(INC) -Wall -O0 -g
objFLAGS = $(cFLAGS) -c
OBJETOs = leitura.o escrita.o sistema.o metodo.o
libFLAGS = -lm

# all: compila e produz um executável chamado pdeSolver no diretório login1-login2/;
all: leitura escrita sistema metodo
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
	@ARQDAT=$$(cat arqdat)
	@NX=$$(cat nx) && NY=$$(cat ny)
	@gnuplot -c "pde3d.plg" "$${ARQDAT}" $${NX} $${NY}

# clean: remove todos os arquivos temporários e os arquivos gerados pelo Makefile (*.o, executável, etc.).
clean:
	@echo Remvendo arquivos-objeto e arquivos temporários...
	@rm -f *.o 
	@rm -f *~ 

dist-clean: clean
	@echo Removendo arquivos auxiliares...
	@rm -f nx 
	@rm -f ny 
	@ARQDAT=$$(cat arqdat) && rm -f $${ARQDAT} 
	@rm -f ./animate/frames/* 
	@echo Removendo arquivo dos pontos...
	@rm -f arqdat 

clean-docs:
	@rm -f -rf ./doc/ 

purge: clean dist-clean clean-docs
	@rm -f pdeSolver 
	@rm -f ./animate/pdeAnim.mp4 

# doc: gera a documentação Doxygen em formato html
doc:
	doxygen Doxyfile
