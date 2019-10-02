#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define PI 3.141592653

/* 

   (Udx + Udy) - (Ud2x + Ud2y) + 4*PI*PI*U =
   4*PI*PI*( sin( 2*PI*x ) * sinh( PI*y ) + sin( 2*PI*(PI-x) ) * sinh( PI*(PI - y) )

   No dominio D = [0, PI] x [0, PI], onde:
   u(x,0) = sin( 2*PI*(PI-x) ) * sinh( PI*PI )
   u(x,PI) = sin( 2*PI*x ) * sinh( PI*PI )
   u(0,y) = 0
   u(PI,y) = 0


  A chamada deve ser da forma:
pdeSolver -nx <Nx> -ny <Ny> -i <maxIter> -o arquivo_saida

-nx: parâmetro obrigatório definindo o número de pontos a serem calculados na dimensão X.

-ny: parâmetro obrigatório definindo o número de pontos a serem calculados na dimensão Y.

-i maxIter: parâmetro obrigatório definindo o número de iterações a serem executadas.

-o arquivo_saida: parâmetro no qual arquivo_saida é o caminho completo para o arquivo que vai conter a solução (valores da função em cada ponto da grade). Caso este parâmetro não esteja especificado, a saída deve ser stdout
Esta solução deve estar formatada para servir de entrada ao comando gnuplot, de forma que ele possa automaticamente gerar o gráfico da função.

Além disso, no início do arquivo, deve constar sob a forma de comentários do gnuplot:
  1. O tempo médio de execução de cada iteração do Método de Gauss-Seidel.
          Deve ser calculado em milisegundos, utilizando-se a função timestamp() especificada aqui https://gitlab.c3sl.ufpr.br/nicolui/ci1164-sislin/blob/master/utils.c
          O tempo é calculado a partir do início da iteração do método até a obtenção do vetor solução daquela iteração. O resultado deve ser a média aritmética do tempo de todas iterações.

  2. O valor do resíduo para cada iteração.

###########
# Tempo Método GS: <média de tempo para o cálculo de uma iteração do método, em milisegundos>
#
# Norma L2 do Residuo
# i=1: <valor>
# i=2: <valor>
# i=3: <valor>
# ...
##########
*/

typedef double Real_t;

typedef struct {

}Sist_Lin;

typedef Solucao;

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
      // abortar e imprimir erro
      printf("Erro! Número errado de argumentos!\nA chamada deve ser da forma 'pdeSolver -nx <Nx> -ny <Ny> -i <maxIter> -o arquivo_saida'.\n");
    }
      
  }
  else{
    // abortar e imprimir erro
    printf("Número errado de argumentos!\nA chamada deve ser da forma 'pdeSolver -nx <Nx> -ny <Ny> -i <maxIter> -o arquivo_saida'.\n");
  }
  return;
}

Solucao* aloca_solucao( Real_t var_init ){
  return NULL;
}

Sist_Lin* aloca_sist(){
  return NULL;
}

void escreve_solucao_gnuplot(char* arq_saida, Real_t tempo_medio, unsigned int num_iter, Real_t* residuo_iter){
  // abra o arquivo de saida
  FILE *saida = fopen(arq_saida, "w+");
  // escreva os comentarios do gnulot acerca da execução do programa
  fprintf(saida, "###########\n");
  fprintf(saida, "# Tempo Método GS: %f ms.\n", tempo_medio);
  fprintf(saida, "#\n# Norma L2 do Residuo\n");
  for(unsigned int i = 0; i < num_iter; ++i){
    fprintf(saida, "# i=%d: %lf\n", i+1, residuo_iter[i] );
  }
  fprintf(saida, "###########\n");
  
  // escreva os valores de x, y e u(x,y) no arquivo de saida

  // feche o arquivo de saída
  fclose(saida);
  return;
}

void libera_sist(){
  return;
}

void libera_solucao(){
  return;
}

int main(int argc, char* argv[]){
  unsigned int nx, ny, iter;
  char* arq_saida;

  // ler entradas da linha de comando
  le_comandos( argc, argv, &nx, &ny, &iter, &arq_saida );
  // alocar as estruturas necessárias pra resolver o problema
  Sist_Lin *sistema = aloca_sist();

  // crie o vetor solução nulo
  Solucao *x = aloca_solucao( 0 );

  // comece um for ate o numero maximo de iteracoes
    // resolva a equacao diferencial por diferencas finitas e gaus-siedel
  
  // escreve o arquivo de saída
  Real_t norma[5] = { 4.0, 4.58, 13.57, 19.12, 5.55 };
  escreve_solucao_gnuplot( arq_saida, 7.98, 5, norma );
  
  // libere a memoria usada para as estruturas
  libera_sist();
  libera_solucao();
  
  return 0;
}

