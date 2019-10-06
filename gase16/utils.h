#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#ifndef _UTILS_PDESOLVER
#define _UTILS_PDESOLVER

#define myMalloc(tam, tipo) (tipo*) malloc(((tam)*(sizeof(tipo))))
#define testMalloc(pointer) if( pointer == NULL ) abort()
#define index(i,j,n) (((i)*(n))+(j))

#define PI 3.14159265358979323846

typedef double Real_t;

typedef struct {
  Real_t dInfx, dInfy, dPrin, dSupx, dSupy;
  Real_t *b;
  unsigned int nx;
  unsigned int ny;
}Sist_Lin;


int le_comandos( int argc, char* argv[], unsigned int* nx, unsigned int* ny, unsigned int* iter, char** arq_saida );

void escreve_solucao_gnuplot( char* arq_saida, double tempo_total_GaussSiedel, unsigned int num_iter, Real_t* residuo_iter, Sist_Lin* sist, Real_t* solucao );

void aloca_sist( Sist_Lin** sist, unsigned int nx, unsigned int ny );
void aloca_e_inicializa_solucao(  Real_t **u, Sist_Lin *sist  );
void libera_sist( Sist_Lin** sist );
void libera_vetor(  Real_t** vetor  );
Real_t calcula_norma_residuo( Sist_Lin *sist, Real_t *u );

double timestamp(void);



#endif