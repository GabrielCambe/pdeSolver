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

#include "utils.h"

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
  free( norma_residuo_por_iter );
  
  return 0;
}

