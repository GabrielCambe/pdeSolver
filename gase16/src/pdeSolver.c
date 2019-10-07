#include "utils.h"

int main(int argc, char *argv[]){
  srand(time(NULL));

  unsigned int nx, ny, iter;
  char *arq_saida;

  // ler entradas da linha de comando
  if( le_comandos( argc, argv, &nx, &ny, &iter, &arq_saida ) != (-1)){

    // alocar e inicializar sistema de equações 
    Sist_Lin *sistema;
    aloca_sist( &sistema, ny, ny );

    // alocar e inicializar o vetor solução nulo
    Real_t *u;
    aloca_e_inicializa_solucao( &u, sistema );

    // alocar e inicializar vetor de residuos
    Real_t *norma_residuo_por_iter = myMalloc( iter, Real_t );
    testMalloc(norma_residuo_por_iter);

    // criar varíaveis para calculo de tempo
    double tempo_inicial;
    double tempo_total_GaussSiedel = 0;

    // comece um for ate o numero maximo de iteracoes
    unsigned int k = 0;
    do{
      ++k;
      tempo_inicial = timestamp();
      
      // resolva a equacao diferencial por diferencas finitas e gaus-siedel
      gaussSiedel(&sistema, &u);

      // calcule o tempo decorrido e atualize a variavel de tempo
      tempo_total_GaussSiedel += (timestamp() - tempo_inicial);
      
      // calcule o residuo para esta iteração
      norma_residuo_por_iter[k-1] = calcula_norma_residuo(sistema, u);

    } while( ((k <= iter) && (norma_residuo_por_iter[k-1] >= MAXERR)) );

    // Checar se o método convergiu
    if(norma_residuo_por_iter[k-1] >= MAXERR){
      perror("Atenção! Não houve convergência para o método! Tente aumentar o número de iterações.\n");
      
      // libere a memoria usada para as estruturas
      libera_sist( &sistema );
      libera_vetor( &u );
      free( norma_residuo_por_iter );
      norma_residuo_por_iter = NULL;
      
      return (-1);
    }

    // escreve o arquivo de saída
    escreve_solucao_gnuplot( arq_saida, tempo_total_GaussSiedel, k, norma_residuo_por_iter, sistema, u );
    
    // libere a memoria usada para as estruturas
    libera_sist( &sistema );
    libera_vetor( &u );
    free( norma_residuo_por_iter );
    norma_residuo_por_iter = NULL;
  
    // if( maior_err_aprox >= 0.25 ){
    //   perror("Atenção! Não houve convergência para o método! Tente aumentar o número de iterações.\n");
    //   return (-1);
    // }

  }else{

    return (-1);
  }

  return 0;
}

