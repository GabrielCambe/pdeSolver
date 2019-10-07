#include "utils.h"

void escreve_solucao_gnuplot(char* arq_saida, double tempo_total_GaussSiedel, unsigned int num_iter, Real_t* residuo_iter, Sist_Lin* sist, Real_t* solucao){
  
    const Real_t hx = PI/((sist->nx)+1);
    const Real_t hy = PI/((sist->ny)+1);
    Real_t x = 0;
    Real_t y = 0;
  
    if( arq_saida != NULL ){
        // abra o arquivo de saida
        FILE *saida = fopen(arq_saida, "w+");
        
        // escreva os comentarios do gnuplot acerca da execução do programa no arquivo de saída
        fprintf(saida, "###########\n");
        fprintf(saida, "# Tempo Método GS: %f ms.\n", (tempo_total_GaussSiedel/num_iter));
        fprintf(saida, "#\n# Norma L2 do Residuo\n");
        for(unsigned int i = 1; i <= num_iter; ++i){
            fprintf(saida, "# i=%d: %lf\n", i, residuo_iter[i-1] );
        }
        fprintf(saida, "###########\n");
  
        // escreva os valores de x, y e u(x,y) no arquivo de saida
        for(unsigned int i = 0; i < ((sist->nx)+2); ++i, x += hx){
            y = 0;
            for(unsigned int j = 0; j < ((sist->ny)+2); ++j, y += hy){
                // acessa o vetor x
	            fprintf(saida, "%lf %lf %lf\n", x, y, solucao[ index(i,j,(sist->ny)+2) ]);
            }
            fprintf(saida, "\n" );
        }
        
        // feche o arquivo de saída
        fclose(saida);
    }
    else{
        // escreva os comentarios acerca da execução do programa na saída padrão?
        
        printf("###########\n");
        printf("# Tempo Método GS: %f ms.\n", (tempo_total_GaussSiedel/num_iter));
        printf("#\n# Norma L2 do Residuo\n");
        for(unsigned int i = 1; i <= num_iter; ++i){
            printf("# i=%d: %lf\n", i, residuo_iter[i-1] );
        }
        printf("###########\n");
  
        // escreva os valores de x, y e u(x,y) no arquivo de saida
        for(unsigned int i = 0; i < ((sist->nx)+2); ++i, x += hx){
            y = 0;
            for(unsigned int j = 0; j < ((sist->ny)+2); ++j, y += hy){
                printf("%lf\t%lf\t%lf\n", x, y, solucao[ index(i,j,(sist->ny)+2) ]);
            }
            printf( "\n" );
        }
    }

    /*!
      \file arqdat
      \brief Esse arquivo é usado para passar o número de pontos como argumentos para o gnuplot.
     */
    FILE *arquivo_de_daods = fopen( "arqdat", "w+" );
    fprintf(arquivo_de_daods, "%s", arq_saida);
    fclose(arquivo_de_daods);

    /*!
      \file nx
      \brief Esse arquivo é usado para passar o número de pontos como argumentos para o gnuplot.
     */
    FILE *num_pontos_x = fopen( "nx", "w+" );
    fprintf(num_pontos_x, "%u", (sist->nx)+2);
    fclose(num_pontos_x);

    /*!
      \file ny
      \brief Esse arquivo é usado para passar o número de pontos como argumentos para o gnuplot.
     */
    FILE *num_pontos_y = fopen( "ny", "w+" );
    fprintf(num_pontos_y, "%u", (sist->ny)+2);
    fclose(num_pontos_y);

    return;
}
