#include "utils.h"

// Aloca as estrutura de dados usada na resolução da eq diferencial
// void aloca_sist(Sist_Lin** sist, unsigned int nx, unsigned int ny){
void aloca_sist(Sist_Lin* sist, unsigned int nx, unsigned int ny){

  // sist = myMalloc( 1, Sist_Lin );
  // testMalloc( sist );
  // Nao preciso alocar o sistema na heap!

  // alocar vetor b
  sist->b = myMalloc( (nx+2)*(ny+2), Real_t ); // aloco na heap pois eu nao sei o tamanho do sistema em rempo de compilaçao
  testMalloc( sist->b );

  sist->nx = nx;
  sist->ny = ny;
  
  // calcular o valor dos passos.
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
      sist->b[ index(i,j,(sist->ny)+2) ] = 4*PI*PI*( (sin(2*PI*x) * sinh(PI*y)) + (sin(2*PI*(PI-x)) * sinh(PI*(PI-y))));
    }
  }

  // criar os valores que representam a matriz A
  sist->dInfy = -((1/(hx*hx)) + (1/(2*hx))); 
  sist->dInfx = -((1/(hy*hy)) + (1/(2*hy)));
  sist->dPrin = ((4*PI*PI)+(2/(hx*hx))+(2/(hy*hy)));
  sist->dSupy = -((1/(hy*hy)) - (1/(2*hy)));
  sist->dSupx = -((1/(hx*hx)) - (1/(2*hx)));

  return;
}

// 
void aloca_e_inicializa_solucao( Real_t **u, Sist_Lin *sist ){
    // inicializa a solução com 0 nos pontos interiores
    // e com as condições de contorno
    (*u) = myMalloc(((sist->nx)+2)*((sist->ny)+2), Real_t);
    testMalloc( (*u) );
    
    const Real_t hx = PI/((sist->nx)+1);
    const Real_t hy = PI/((sist->ny)+1);
    Real_t x = 0;
    Real_t y = 0;
    // acessar e inicializa o vetor solução
    for(unsigned int i = 0; i < ((sist->nx)+2); ++i, x += hx){
        y = 0;
        for(unsigned int j = 0; j < ((sist->ny)+2); ++j, y += hy){
            // if(i == 0 || i == ((sist->nx)+1)){ // u(0,y) = 0; u(PI,y) = 0
	          //   (*u)[ index(i,j,(sist->ny)+2) ] = 0 ;
	            
            // }else if(j == 0){ // u(x,0) = sin( 2*PI*(PI-x) ) * sinh( PI*PI )
	          //   (*u)[ index(i,j,(sist->ny)+2) ] = sin( 2*PI*(PI-x) ) * sinh( PI*PI );
	            
            // }else if(j == ((sist->ny)+1)){ // u(x,PI) = sin( 2*PI*x ) * sinh( PI*PI )
	          //   (*u)[ index(i,j,(sist->ny)+2) ] = sin( 2*PI*x ) * sinh( PI*PI );
	            
            // }else{
            //     (*u)[ index(i,j,(sist->ny)+2) ] = 0;
            //     // (*u)[ index(i,j,(sist->ny)+2) ] = rand();
            // }
            // posso evitar os diversos branchs

            if(i == 0 || i == ((sist->nx)+1)){ // u(0,y) = 0; u(PI,y) = 0
	            (*u)[ index(i,j,(sist->ny)+2) ] = 0 ;
              continue;  
            }
            
            if(j == 0){ // u(x,0) = sin( 2*PI*(PI-x) ) * sinh( PI*PI )
	            (*u)[ index(i,j,(sist->ny)+2) ] = sin( 2*PI*(PI-x) ) * sinh( PI*PI );
              continue;
            }
            
            if(j == ((sist->ny)+1)){ // u(x,PI) = sin( 2*PI*x ) * sinh( PI*PI )
	            (*u)[ index(i,j,(sist->ny)+2) ] = sin( 2*PI*x ) * sinh( PI*PI );
              continue;
            }
            
            // (*u)[ index(i,j,(sist->ny)+2) ] = 0;
            (*u)[ index(i,j,(sist->ny)+2) ] = rand();
        }
    }

  return;
}

// Libera a estrutura de dados usada para armazenar o sistema linear resultante da eq diferencial
// void libera_sist(Sist_Lin** sist){
void libera_sist(Sist_Lin* sist){
    // desalocar o vetor b
    free(sist->b);
    sist->b = NULL;
    
    // // desalocar o sistema 
    // free((*sist));
    // (*sist) = NULL;

    return;
}

// Libera o vetor usado como solucao no metodo
void libera_vetor( Real_t** vetor ){
    // desaloca o vetor
    free((*vetor));
    return;
}
