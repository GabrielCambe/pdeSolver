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
#define MAXERR 6.0E-5
#define PI 3.14159265358979323846



/*!
    \typedef double Real_t
    \details Defino esse tipo para facilitar mudanças posteriores. Por exemplo, mudar a precisão de todas as funções para float.
*/
typedef double Real_t;


/*!
    \brief Essa estrutura anônima foi definida para o tipo Sist_Lin, que representa o sistema linear resultante da discretização da equação diferencial. 
    \details As variáveis dInfx, dInfy, dPrin, dSupx e dSupy representam os valores das diagonais não-nulas da matriz A do sistema. Os inteiros nx e ny são o número de pontos em cada dimensão, passados como argumento para o programa.
*/
typedef struct {
  Real_t dInfx, dInfy, dPrin, dSupx, dSupy;
  /*!
    \var Real_t* b
    Vetor de termos independentes do sistema. Contém os valores de f(x,y) = 4*PI²(sin(2*PI*x)*sinh(PI*y)+sin(2*PI*(PI-x))*sinh(PI*(PI-y)))
  */
  Real_t *b;
  unsigned int nx; /*!< Um inteiro, o número de pontos para a dimensão x. */ 
  unsigned int ny; /*!< Um inteiro, o número de pontos para a dimensão y. */ 
}Sist_Lin;



/*!
    \brief Essa função valida os comandos passados como argumento. 

    \param argc Um inteiro que representa o número de argumentos passados do Shell para o programa.
    \param argv Um vetor de strings, os argumentos da função main.
    \param nx Um inteiro sem sinal, o número de passos na dimensão x da região onde resolveremos a equação diferencial.
    \param ny Um inteiro sem sinal, o número de passos na dimensão y da região onde resolveremos a equação diferencial.
    \param iter Ponteiro para um tipo unsigned inte que armazenará o número máximo de iterações a se fazer na resolução do problema.
    \param arq_saida Ponteiro para uma string que contém o nome do arquivo de saída ou NULO.
*/
int le_comandos( int argc, char* argv[], unsigned int* nx, unsigned int* ny, unsigned int* iter, char** arq_saida );


/*!
    \brief Essa função escreve em um arquivo os valores calculados pelo método de Gauss-Siedel.
    Os valores de u(x,y) são postos junto ao seus respectivos x e y num arquivo do tipo grid do gnuplot.
    Também são escritos os comentários acerca da execução do programa, o resíduo por iteração e o tempo médio de execução do método.
    
    \param arq_saida String que contém o nome do arquivo de saída.
    \param tempo_total_GaussSiedel Um double com o valor do somatório dos tempos de cada iteração executada do método.
    \param num_iter Um inteiro com o número de iterações feitas na execução de gaussSiedel.
    \param residuo_iter Um vetor de tipo Real_t, os resíduos para cada iteração.
    \param sist Ponteiro para o sistema que representa a equação diferencial.
    \param solucao Vetor solução do sistema.*/
void escreve_solucao_gnuplot( char* arq_saida, double tempo_total_GaussSiedel, unsigned int num_iter, Real_t* residuo_iter, Sist_Lin* sist, Real_t* solucao );


/*!
    \brief Essa função aloca as estruturas de dados que compôem um sistema linear no programa. 
    Constituído basicamente dos membros da struct Sist_Lin, essa função executa um malloc para o vetor b.
    Ela também o preenche com os valores de f(x,y). 
    
    \param sist Ponteiro para o sistema que representa a equação diferencial.
    \param nx Um inteiro sem sinal, o número de passos na dimensão x da região onde resolveremos a equação diferencial.
    \param ny Um inteiro sem sinal, o número de passos na dimensão y da região onde resolveremos a equação diferencial.
*/
void aloca_sist( Sist_Lin** sist, unsigned int nx, unsigned int ny );


/*!
    \brief Essa função aloca o vetor solução u(x,y) e o inicializa.
    Esse vetor representa a região do plano para a qual se quer resolver a equação diferencial.
    O vetor é preenchido com zeros nos seus pontos internos e com as condições de contorno nas bordas da região.
    No caso da equação do trabalho a região e as cndições eram:
    O dominio D = [0, PI] x [0, PI], onde:
        u(x,0) = sin( 2*PI*(PI-x) ) * sinh( PI*PI )
        u(x,PI) = sin( 2*PI*x ) * sinh( PI*PI )
        u(0,y) = 0
        u(PI,y) = 0
    
    \param sist Ponteiro para o sistema que representa a equação diferencial.
    \param u Vetor solução do sistema.
*/
void aloca_e_inicializa_solucao(  Real_t **u, Sist_Lin *sist  );


/*!
    \brief Essa função libera o vetor b do um sistema linear passado como argumento e depois libera o proprio sistema. 

    \param sist Ponteiro para o sistema que representa a equação diferencial.
*/
void libera_sist( Sist_Lin** sist );


/*!
    \brief Essa função chama a função free() para o vetor passado como argumento. 
    
    \param vetor Um vetor de tipo Real_t qualquer.
*/

void libera_vetor(  Real_t** vetor  );

/*!
    \brief Implementação do método de Gauss-Siedel para a equação diferencial apresentada. 

    \param sistema Ponteiro para o sistema que representa a equação diferencial.
    \param u Vetor solução do sistema.
*/
void gaussSiedel( Sist_Lin **sistema, Real_t **u );


/*!
    \brief Essa função calcula a norma L2 do resíduo de um sistema linear. 
 
    \param sist Ponteiro para o sistema que representa a equação diferencial.
    \param u Vetor solução do sistema.
*/
Real_t calcula_norma_residuo( Sist_Lin *sist, Real_t *u );


/*!
    \brief Função disponibilizada pelos professores para a medição de tempo.
*/
double timestamp( void );


#endif