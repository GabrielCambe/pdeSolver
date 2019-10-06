#include "utils.h"

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
