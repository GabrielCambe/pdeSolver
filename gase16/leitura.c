#include "utils.h"

// Trata a entrada passada pela linha de comando
/* PRECISA MUDAR A CHECAGEM DA VALIDADE DOS ARGUMENTOS -nx, -ny e -i PARA C CERTIFICAR QUE NÃO EXISTEM LETRAS NO NUMERO PASSADO */
void le_comandos(int argc, char* argv[], unsigned int* nx, unsigned int* ny, unsigned int* iter, char** arq_saida){
  // testar validade das entradas
  if(argc >= 7){ // existem os 3 parametros obrigatorios?
    // "nx", "ny" e "i" existem e são inteiros positivos?
    if( !(strcmp(argv[1], "-nx")) ){
      if( atoi(argv[2]) > 0 ){
	*nx = (unsigned int)atoi(argv[2]);
      }
      else{
	// abortar e imprimir erro
	printf("Argumento Inválido! O valor para -nx é um número inteiro maior que zero!\n");
      }
    }
    else{
      // abortar e imprimir erro
      printf("Argumento Inválido! A primeira opção deveria ser -nx.\n");
    }
      
      
    if( !(strcmp(argv[3], "-ny")) ){
      if( atoi(argv[4]) > 0 ){
	*ny = (unsigned int)atoi(argv[4]);
      }
      else{
	// abortar e imprimir erro
	printf("Argumento Inválido! O valor para -ny é um número inteiro maior que zero!\n");
      }
    }
    else{
      // abortar e imprimir erro
      printf("Argumento Inválido! A segunda opção deveria ser -ny.\n");
    }

      
    if( !(strcmp(argv[5], "-i")) ){
      if( atoi(argv[6]) > 0 ){
	*iter = (unsigned int)atoi(argv[6]);
      }
      else{
	// abortar e imprimir erro
	printf("Argumento Inválido! O valor para -i é um número inteiro maior que zero!");
      }
    }
    else{
      printf("Argumento Inválido! A terceira opção deveria ser -i.\n");
    }
      
      
    if( argc >= 8 ){ // existe um 4o parametro?
      if( argc == 9 && !(strcmp(argv[7], "-o")) ){ // ele é -o?
	  *arq_saida = argv[8];
      }
      else{
	// abortar e imprimir erro0
	printf("Erro!\nA chamada deve ser da forma 'pdeSolver -nx <Nx> -ny <Ny> -i <maxIter> -o arquivo_saida'.\n");
      }
    }
    else{
      // setta a saída para stdout caso -o não exista
      *arq_saida = NULL;
    }
      
  }
  else{
    // abortar e imprimir erro
    printf("Número errado de argumentos!\nA chamada deve ser da forma 'pdeSolver -nx <Nx> -ny <Ny> -i <maxIter> -o arquivo_saida'.\n");
  }
  return;
}