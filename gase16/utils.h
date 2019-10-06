#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


#ifndef _UTILS_PDESOLVER
#define _UTILS_PDESOLVER

#define myMalloc(tam, tipo) (tipo*) malloc(tam*sizeof(tipo))
#define testMalloc(pointer) if( pointer == NULL ) abort()
#define index(i,j,n) ((i*n)+j)

#define PI 3.141592653

typedef double Real_t;

typedef struct {
  Real_t dInfx, dInfy, dPrin, dSupx, dSupy;
  Real_t *b;
  unsigned int nx;
  unsigned int ny;
}Sist_Lin;


void le_comandos(int argc, char* argv[], unsigned int* nx, unsigned int* ny, unsigned int* iter, char** arq_saida);

void escreve_solucao_gnuplot(char* arq_saida, Real_t tempo_medio, unsigned int num_iter, Real_t* residuo_iter, Sist_Lin* sist, Real_t* solucao);

Sist_Lin* aloca_sist(unsigned int nx, unsigned int ny);
Real_t* aloca_e_inicializa_solucao( Sist_Lin * sist );
void libera_sist(Sist_Lin* sist);
void libera_solucao( Real_t* solucao );
Real_t calcula_norma_residuo();



#endif