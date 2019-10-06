#include "utils.h"

// alocar as estruturas necessárias pra resolver o problema:
Sist_Lin* aloca_sist(unsigned int nx, unsigned int ny){

  Sist_Lin* sist = myMalloc( 1, Sist_Lin );
  testMalloc( sist );

  // alocar vetor b
  sist->b = myMalloc( (nx+2)*(ny+2), Real_t );
  testMalloc( sist->b );
  
  // calcular o valor dos passos
  const Real_t hx = PI/(nx+1); // se voce quiser n pontos num intervalo [a,b]
  const Real_t hy = PI/(ny+1); // o tamanho do passo é (b-a)/(n+1) 

  // b depende dos valores de x e y para cada ponto
  // que são gerados começando em 0 e somando passos hx e hy
  Real_t x = 0;
  Real_t y = 0;
  for(unsigned int i = 0; i < (nx+2); ++i, x += hx){
    y = 0;
    for(unsigned int j = 0; j < (ny+2); ++j, y += hy){
      // preencher o vetor b com os valores de f(x,y)
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

Real_t* aloca_e_inicializa_solucao( Sist_Lin * sist ){
  // inicializa a solução com 0s nos pontos interiores
  // e com as condições de contorno)
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