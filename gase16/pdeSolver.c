#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

#define myMalloc(tam, tipo) (tipo*) malloc(tam*sizeof(tipo))
#define testMalloc(pointer) if( pointer == NULL ) abort()
#define index(i,j,n) ((i*n)+j)

typedef double Real_t;


typedef struct {
  Real_t dInfx, dInfy, dPrin, dSupx, dSupy;
  Real_t *b;
  unsigned int nx;
  unsigned int ny;
}Sist_Lin;


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

Real_t* aloca_e_inicializa_solucao( Sist_Lin * sist ){
  // inicializa a solução com 0s nos pontos interiores e com cada uma das funções nas bordas (preencha o vetor com zero e as condições de contorno)
  Real_t* solucao = myMalloc(((sist->nx+2)*(sist->ny+2)), Real_t);
  testMalloc( solucao );

  Real_t hx = 1/((sist->nx)+1); Real_t hy = 1/((sist->ny)+1);
  Real_t x = 0;
  Real_t y = 0;

  for(unsigned int i = 0; i < ((sist->nx)+2); ++i, x += hx){
    y = 0;
    for(unsigned int j = 0; j < ((sist->ny)+2); ++j, y += hy){
      // acessa e inicializa o vetor x
      if(i == 0 || i == ((sist->nx)+1)){
	solucao[ index(i,j, ((sist->nx+2)*(sist->ny+2))) ] = 0 ;
	continue;
      }
      
      if(j == 0){
	solucao[ index(i,j, ((sist->nx+2)*(sist->ny+2))) ] = sin( 2*PI*(PI-x) ) * sinh( PI*PI );
	continue;
      }
      
      if(j == ((sist->ny)+1)){
	solucao[ index(i,j, ((sist->nx+2)*(sist->ny+2))) ] = sin( 2*PI*x ) * sinh( PI*PI );
	continue;
      }
      
      solucao[ index(i,j, ((sist->nx+2)*(sist->ny+2))) ] = 0;
    }
  }

  return solucao;
}

// alocar as estruturas necessárias pra resolver o problema:
Sist_Lin* aloca_sist(unsigned int nx, unsigned int ny){

  Sist_Lin* sist = myMalloc( 1, Sist_Lin );
  testMalloc( sist );

  // alocar e inicializar vetor b
    // calcular o valor dos passos
  Real_t hx = 1/(nx+1); Real_t hy = 1/(ny+1);
 
    // criar vetor b de tamanho (nx+2)(ny+2)
  sist->b = myMalloc( (nx+2)*(ny+2), Real_t );
  testMalloc( sist->b );
  
    // b depende de vetores x e y, de tamanho nx+2 e ny+2, qu possuem os valores dos passos dados em x e em y
    // populados com x = 0; x <= PI; x += PI/(nx+1) e y = 0; y <= PI; y += PI/(ny+1)
    // preencher esse vetor com os valores de f(x,y) = 4*PI*PI*( sin( 2*PI*x ) * sinh( PI*y ) + sin( 2*PI*(PI-x) ) * sinh( PI*(PI - y) )
  Real_t x = 0;
  Real_t y = 0;
  for(unsigned int i = 0; i < (nx+2); ++i, x += hx){
    y = 0;
    for(unsigned int j = 0; j < (ny+2); ++j, y += hy){
      sist->b[ index(i,j,((sist->ny)+2)) ] = 4*PI*PI*( sin( 2*PI*x ) * sinh( PI*y ) + sin( 2*PI*(PI-x) ) * sinh( PI*(PI - y) ));
    }
  }
  sist->nx = nx;
  sist->ny = ny;

    // criar os valores que representam a matriz A
  sist->dInfy = -((1/(hx*hx)) + (1/(2*hx))); 
  sist->dInfx = -((1/(hy*hy)) + (1/(2*hy)));
  sist->dPrin = ((4*PI*PI)+(2/(hx*hx))+(2/(hy*hy)));
  sist->dSupy = -((1/(hy*hy)) - (1/(2*hy)));
  sist->dSupx = -((1/(hx*hx)) - (1/(2*hx)));

  return sist;
}

void escreve_solucao_gnuplot(char* arq_saida, Real_t tempo_medio, unsigned int num_iter, Real_t* residuo_iter, Sist_Lin* sist, Real_t* solucao){
  srand(time(NULL));
  Real_t hx = 1/((sist->nx)+1); Real_t hy = 1/((sist->ny)+1);
  Real_t x = 0;
  Real_t y = 0;
  
  if( arq_saida != NULL ){
    // abra o arquivo de saida
    FILE *saida = fopen(arq_saida, "w+");
    // escreva os comentarios do gnulot acerca da execução do programa num arquivo de saída
    fprintf(saida, "###########\n");
    fprintf(saida, "# Tempo Método GS: %f ms.\n", tempo_medio);
    fprintf(saida, "#\n# Norma L2 do Residuo\n");
    for(unsigned int i = 0; i < num_iter; ++i){
      fprintf(saida, "# i=%d: %lf\n", i+1, residuo_iter[i] );
    }
    fprintf(saida, "###########\n");
  
    // escreva os valores de x, y e u(x,y) no arquivo de saida
    for(unsigned int i = 0; i < ((sist->nx)+2); ++i, x += hx){
      y = 0;
      for(unsigned int j = 0; i < ((sist->ny)+2); ++i, y += hy){
        // acessa o vetor x
	fprintf(saida, "%lf\t%lf\t%lf\n", x, y, solucao[ index(i,j,((sist->ny)+2)) ]);
      }
    }
    // feche o arquivo de saída
    fclose(saida);
  }
  else{
    // escreva os comentarios do gnulot acerca da execução do programa na saída padrão
    printf("###########\n");
    printf("# Tempo Método GS: %f ms.\n", tempo_medio);
    printf("#\n# Norma L2 do Residuo\n");
    for(unsigned int i = 0; i < num_iter; ++i){
      printf("# i=%d: %lf\n", i+1, residuo_iter[i] );
    }
    printf("###########\n");
  
    // escreva os valores de x, y e u(x,y) no arquivo de saida
    for(unsigned int i = 0; i < ((sist->nx)+2); ++i, x += hx){
      y = 0;
      for(unsigned int j = 0; i < ((sist->ny)+2); ++i, y += hy){
        printf("%lf\t%lf\t%lf\n", x, y, solucao[ index(i,j,((sist->ny)+2)) ]);
      }
    }
  }
  return;
}

void libera_sist(Sist_Lin* sist){
  // desalocar o vetor b
  free(sist->b);
  // desalocar o sistema 
  free(sist);
  
  return;
}

void libera_solucao( Real_t* solucao ){
  // desaloca o vetor de solucao
  free(solucao);
  return;
}

Real_t calcula_norma_residuo(){
  return (Real_t) rand();
}

int main(int argc, char* argv[]){
  unsigned int nx, ny, iter;
  char* arq_saida;

  // ler entradas da linha de comando
  le_comandos( argc, argv, &nx, &ny, &iter, &arq_saida );

  // alocar e inicializar sistema de equações 
  Sist_Lin *sistema = aloca_sist(ny, ny);
  // alocar e inicializar o vetor solução nulo
  Real_t *u = aloca_e_inicializa_solucao( sistema );
  // alocar e inicializar vetor de residuos
  Real_t *norma_residuo_por_iter = myMalloc( iter, Real_t );
  testMalloc(norma_residuo_por_iter);
  
  // comece um for ate o numero maximo de iteracoes
  unsigned int k;
  for( k = 1; k <= iter; ++k ){
    if(rand() % 23) break;
  
    // resolva a equacao diferencial por diferencas finitas e gaus-siedel
    // inicializa x e y com PI/(nx+1) e PI/(ny+1)
    // percorra os pontos atualizando os valores de
    // calcule o residuo para esta iteração
    norma_residuo_por_iter[k-1] = calcula_norma_residuo();
  }
  
  // escreve o arquivo de saída
  escreve_solucao_gnuplot( arq_saida, 7.98, k, norma_residuo_por_iter, sistema, u );
  
  // libere a memoria usada para as estruturas
  libera_sist( sistema );
  libera_solucao( u );
  //free(norma_residuo);
  
  return 0;
}

