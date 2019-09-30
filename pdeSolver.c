#include <stdio.h>
#include <math.h>

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

typedef Real_t;

typedef struct {

}Sist_Lin;

typedef Solucao;

void le_comandos(int argc, char* argv[]){
  // testar validade das entradas
    // existem os 3 parametros obrigatorios?
    // "nx", "ny" e "i" existem e são inteiros positivos?
    // existe um 4o parametro? ele é -o?
}

Solucao* aloca_solucao( Real_t var_init ){

}

Sist_Lin* aloca_sist(){

}

void escreve_solucao_gnuplot{
  // abra o arquivo de saida
  // escreva os comentarios do gnulot acerca da execução do programa
  // escreva os valores de x, y e u(x,y) no arquivo de saida
  // feche o arquivo de saída
}

void libera_sist(){

}

int main(int argc, char* argv[]){
  // ler entradas da linha de comando
  le_comandos( argc, argv );
            
  // alocar as estruturas necessárias pra resolver o problema
  Sist_Lin *sistema = aloca_sist();

  // crie o vetor solução nulo
  Solucao *x = aloca_solucao( 0 );

  // comece um for ate o numero maximo de iteracoes
    // resolva a equacao diferencial por diferencas finitas e gaus-siedel
  
  // escreve o arquivo de saída
  escreve_solucao_gnuplot();
  
  // libere a memoria usada para as estruturas
  libera_sist();
  libera_solucao();
  
  return 0;
}

/*
#define index(i,j,n) (i*n)+j
#define mymalloc(n,tipo) (tipo*)malloc(n*sizeof(tipo))
#define testmalloc(v) if(v == NULL) abort()

real_t kahanSum( real_t *input, unsigned int tam ){
  real_t sum = 0.0;                    // Prepare the accumulator.
  real_t c = 0.0;                     // A running compensation for lost low-order bits.
  
  for (int i = 0; i < tam; i++){     // The array input has elements indexed input[1] to input[input.length].
    // adicionei essas definições, eram definidas dentro do for anteriormente.
    real_t y = 0.0;
    real_t t = 0.0;
    //-------------------
    y = input[i] - c;         // c is zero the first time around.
    t = sum + y;              // Alas, sum is big, y small, so low-order digits of y are lost.
    c = (t - sum) - y;            // (t - sum) cancels the high-order part of y; subtracting y recovers negative (low part of y)
    sum = t;                      // Algebraically, c should always be zero. Beware overly-aggressive optimizing compilers!
    // Next time around, the lost low part will be added to y in a fresh attempt.
  }
  return sum;
}

/*!
  \brief Essa função calcula a norma L2 do resíduo de um sistema linear 

  \param SL Ponteiro para o sistema linear
  \param x Solução do sistema linear
*/
real_t normaL2Residuo(SistLinear_t *SL, real_t *x)
{
  unsigned int tam = SL->n;
  real_t *res = mymalloc(tam, real_t);
  testmalloc(res);  
  
  //calcula resíduo
  for(int i = 0; i < tam; i++){
    res[i] = 0;
    for(int j = 0; j < tam; j++){
      res[i] += SL->A[index(i,j,tam)] * x[j];
    }
  }
  real_t norma = 0;
  real_t diff = 0;
  //calcula norma do vetor ao residuo
  for(int i = 0; i < tam; i++){
    for(int j = 0; j < tam; j++){
      diff = fabs(res[i] - SL->b[i]);
      norma += diff * diff;
    }
  }

  norma = sqrt(norma);

  free(res);
  return norma;
}

/*!
  \brief Método de Gauss-Seidel

  \param SL Ponteiro para o sistema linear
  \param x ponteiro para o vetor solução
  \param erro menor erro aproximado para encerrar as iterações

  \return código de erro. Um nr positivo indica sucesso e o nr
          de iterações realizadas. Um nr. negativo indica um erro.
  */

int gaussSeidel (SistLinear_t *SL, real_t *x, real_t erro){
  double tempo_exec = timestamp();
#ifdef DEBUG_p
  printf("\nOperações Gauss-Seidel:\n");
#endif

  unsigned int n = SL->n;
  real_t diff, max_diff;
  real_t *x_next = mymalloc(n,real_t);
  testmalloc(x_next);

  for(int k = 0; k <= MAXIT; ++k){

    //checar maximo de iterações
    if( k == MAXIT ){
      printf("Não houve convergência!\n");
      free(x_next);
      return 0;
    }

#ifdef DEBUG_p
    printf("Iteração %d:\n\n", k);
#endif

    //--------------------- SK
    real_t sum, c, y, t;
    //--------------------- SK

    for(int i = 0; i < n; ++i){
      x_next[i] = SL->b[i];

#ifdef DEBUG_p
      printf("x_next[%d] = b[%d];\n", i, i);
#endif
      //-------------------- SK
      sum = 0.0;
      c = 0.0;
      //-------------------- SK

      for(int j = 0; j < n; ++j){
        //------------------ SK
        y = 0.0;
        t = 0.0;
        //------------------- SK

        if( j != i ){
          if(j < i){
            y = (-(SL->A[index(i,j,n)] * x_next[j]) - c);
	          t = (x_next[i] + y);
            c = (t - x_next[i]) - y;
            x_next[i] = t;

#ifdef DEBUG_p
            printf("x_next[%d] -= A[%d] * x_next[%d];\n", i, index(i,j,n), j);
#endif
	        }else{
            y = (-(SL->A[index(i,j,n)] * x[j]) - c);
	          t = (x_next[i] + y);
            c = (t - x_next[i]) - y;
            x_next[i] = t;

#ifdef DEBUG_p
            printf("x_next[%d] -= A[%d] * x[%d];\n", i, index(i,j,n), j);
#endif
          }
        }
      }
      if(SL->A[index(i,i,n)])
        x_next[i] /= SL->A[index(i,i,n)];

#ifdef DEBUG_p
      printf("x_next[%d] /= A[%d];\n", i, index(i,i,n));
#endif

    }
    
    // checar tolerancia 
    max_diff = 0;
    for(int t = 0; t < n; t++){
      diff = fabs( (x_next[t] - x[t]) ); 
      if( diff > max_diff ){
        max_diff = diff;
      }
    }
    if( max_diff < erro ){
      for(int t = 0; t < n; ++t){
        x[t] = x_next[t]; 
      }     
      break;
    }  

    for(int t = 0; t < n; ++t){
      x[t] = x_next[t]; 
    }
  }

  tempo_exec -= timestamp();
  printf("Tempo de Execucao de gaussSiedel: %10.10lf seg.\n", -(tempo_exec)/1000);

  free(x_next);
  return 1;
}

  

double eq_diff(double x){ // u = u(x,y)
  return (Udx + Udy) - (Ud2x + Ud2y) + 4*PI*PI*U;
}
*/

