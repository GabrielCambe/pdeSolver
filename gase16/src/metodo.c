#include "utils.h"
#include <math.h>

void gaussSiedel( Sist_Lin **sistema, Real_t **u ){
    Real_t Uij;

    for(unsigned int i = 1; i < (((*sistema)->nx)+1); ++i){
        for(unsigned int j = 1; j < (((*sistema)->ny)+1); ++j){
            Uij = (*sistema)->b[ index(i,j,(((*sistema)->ny)+2)) ];
            Uij -= (((*sistema)->dInfy) * (*u)[ index(i,j-1,((*sistema)->ny)+2) ]);
            Uij -= (((*sistema)->dInfx) * (*u)[ index(i-1,j,((*sistema)->ny)+2) ]);
            Uij -= (((*sistema)->dSupy) * (*u)[ index(i,j+1,((*sistema)->ny)+2) ]);
            Uij -= (((*sistema)->dSupx) * (*u)[ index(i+1,j,((*sistema)->ny)+2) ]);
            Uij /= ((*sistema)->dPrin);

            // // Calcula diferença entre aproximações ||  x(k) – x(k – 1) ||
            // (*err_aprox_atual) = fabs( (Uij - ((*u)[ index(i,j,((*sistema)->ny)+2) ])) );
            // (*maior_err_aprox) = ((*err_aprox_atual) > (*maior_err_aprox)) ? ((*err_aprox_atual)) : ((*maior_err_aprox));

            (*u)[ index(i,j,((*sistema)->ny)+2) ] = Uij;
        }
    }

    return;
}


Real_t calcula_norma_residuo(Sist_Lin *sist, Real_t *u){

    Real_t *residuo = myMalloc(((sist->nx)+2)*((sist->ny)+2), Real_t);
    testMalloc(residuo);

    Real_t norma = 0;

    // calcula norma resíduo
    for(unsigned int i = 1; i < ((sist->nx)+1); ++i){
        for(unsigned int j = 1; j < ((sist->ny)+1); ++j){
            residuo[ index(i,j,(sist->ny)+2) ] = (sist->b[ index(i,j,(sist->ny)+2) ]);
            residuo[ index(i,j,(sist->ny)+2) ] -= ((sist->dInfy) * u[ index(i,j-1,(sist->ny)+2) ]);  
            residuo[ index(i,j,(sist->ny)+2) ] -= ((sist->dInfx) * u[ index(i-1,j,(sist->ny)+2) ]);
            residuo[ index(i,j,(sist->ny)+2) ] -= ((sist->dSupy) * u[ index(i,j+1,(sist->ny)+2) ]);
            residuo[ index(i,j,(sist->ny)+2) ] -= ((sist->dSupx) * u[ index(i+1,j,(sist->ny)+2) ]);
            residuo[ index(i,j,(sist->ny)+2) ] -= (sist->dPrin) * u[ index(i,j,(sist->ny)+2) ];
            // calcula norma do vetor de residuo
            norma += residuo[ index(i,j,(sist->ny)+2) ] * residuo[ index(i,j,(sist->ny)+2) ];
        }
    }

    norma = sqrt(norma);

    libera_vetor(&residuo);

    return norma;
}

// Retorna tempo em milisegundos
double timestamp(void){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}