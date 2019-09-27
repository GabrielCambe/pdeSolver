# all: compila e produz um executável chamado pdeSolver no diretório login1-login2/;
all:
	gcc -o pdeSolver pdeSolver.c
# clean: remove todos os arquivos temporários e os arquivos gerados pelo Makefile (*.o, executável, etc.).
clean:
	rm *.o || true
	rm *~ || true
	rm pdeSolver
# doc: gera a documentação Doxygen em formato html
doc:
	echo GeraDoc
