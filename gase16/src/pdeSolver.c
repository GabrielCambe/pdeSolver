#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>


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


// Libera a estrutura de dados usada para armazenar o sistema linear resultante da eq diferencial
// void libera_sist(Sist_Lin** sist){
/*!
    \brief Essa função libera o vetor b do um sistema linear passado como argumento e depois libera o proprio sistema. 

    \param sist Ponteiro para o sistema que representa a equação diferencial.
*/
// void libera_sist( Sist_Lin** sist );
void libera_sist(Sist_Lin* sist){
    // desalocar o vetor b
    if( sist->b != NULL ){
      free(sist->b);
      sist->b = NULL;
    }
  
    return;
}

// Libera o vetor usado como solucao no metodo
/*!
    \brief Essa função chama a função free() para o vetor passado como argumento. 
    
    \param vetor Um vetor de tipo Real_t qualquer.
*/
void libera_vetor( Real_t** vetor ){
    // desaloca o vetor
    if( (*vetor) != NULL ){
      free((*vetor));
      (*vetor) = NULL;
    }

    return;
}


/*!
    \brief Essa função escreve em um arquivo os valores calculados pelo método de Gauss-Siedel.
    Os valores de u(x,y) são postos junto ao seus respectivos x e y num arquivo do tipo grid do gnuplot.
    Também são escritos os comentários acerca da execução do programa, o resíduo por iteração e o tempo médio de execução do método.
    
    \param arq_saida String que contém o nome do arquivo de saída.
    \param tempo_total_GaussSiedel Um double com o valor do somatório dos tempos de cada iteração executada do método.
    \param num_iter Um inteiro com o número de iterações feitas na execução de gaussSiedel.
    \param residuo_iter Um vetor de tipo Real_t, os resíduos para cada iteração.
    \param sist Ponteiro para o sistema que representa a equação diferencial.
    \param solucao Vetor solução do sistema.
*/
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


      // Para o gnuplot
      
      /*!
        \file arqdat
        \brief Esse arquivo é usado para passar o número de pontos como argumentos para o gnuplot.
      */
      FILE *arquivo_de_dados = fopen( "arqdat", "w+" );
      if( arquivo_de_dados != NULL ){
        fprintf(arquivo_de_dados, "%s", arq_saida);
        fclose(arquivo_de_dados);
      }

      /*!
        \file nx
        \brief Esse arquivo é usado para passar o número de pontos como argumentos para o gnuplot.
      */
      FILE *num_pontos_x = fopen( "nx", "w+" );
      if( num_pontos_x != NULL ){
        fprintf(num_pontos_x, "%u", (sist->nx)+2);
        fclose(num_pontos_x);
      }
    
      /*!
        \file ny
        \brief Esse arquivo é usado para passar o número de pontos como argumentos para o gnuplot.
      */
      FILE *num_pontos_y = fopen( "ny", "w+" );
      if( num_pontos_x != NULL ){
        fprintf(num_pontos_y, "%u", (sist->ny)+2);
        fclose(num_pontos_y);
      }

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

    return;
}

// Trata a entrada passada pela linha de comando
/* PRECISA MUDAR A CHECAGEM DA VALIDADE DOS ARGUMENTOS -nx, -ny e -i PARA C CERTIFICAR QUE NÃO EXISTEM LETRAS NO NUMERO PASSADO */
/*!
    \brief Essa função valida os comandos passados como argumento. 

    \param argc Um inteiro que representa o número de argumentos passados do Shell para o programa.
    \param argv Um vetor de strings, os argumentos da função main.
    \param nx Um inteiro sem sinal, o número de passos na dimensão x da região onde resolveremos a equação diferencial.
    \param ny Um inteiro sem sinal, o número de passos na dimensão y da região onde resolveremos a equação diferencial.
    \param iter Ponteiro para um tipo unsigned inte que armazenará o número máximo de iterações a se fazer na resolução do problema.
    \param arq_saida Ponteiro para uma string que contém o nome do arquivo de saída ou NULO.
*/
int le_comandos(int argc, char* argv[], unsigned int* nx, unsigned int* ny, unsigned int* iter, char** arq_saida){
    // testar validade das entradas
    if(argc >= 7){ // existem os 3 parametros obrigatorios?
        // "nx", "ny" e "i" existem e são inteiros positivos?
        if( !(strcmp(argv[1], "-nx")) ){
            if( atoi(argv[2]) > 0 ){
	            (*nx) = (unsigned int)atoi(argv[2]);
            }
            else{
	            // abortar e imprimir erro
                perror("Erro: Argumento Inválido! O valor correto para -nx é um número inteiro maior que zero!\n");
                return (-1);
            }
        }
        else{
            // abortar e imprimir erro
            perror("Erro: Argumento Inválido! A primeira opção deveria ser -nx.\n");
            return (-1);
        }

        if( !(strcmp(argv[3], "-ny")) ){
            if( atoi(argv[4]) > 0 ){
	            (*ny) = (unsigned int)atoi(argv[4]);
            }
            else{
	            // abortar e imprimir erro
                perror("Erro: Argumento Inválido! O valor correto para -ny é um número inteiro maior que zero!\n");
	            return (-1);
            }
        }
        else{
            // abortar e imprimir erro
            perror("Erro: Argumento Inválido! A segunda opção deveria ser -ny.\n");
            return (-1);
        }

      
        if( !(strcmp(argv[5], "-i")) ){
            if( atoi(argv[6]) > 0 ){
	            (*iter) = (unsigned int)atoi(argv[6]);
            }
            else{
	            // abortar e imprimir erro
                perror("Erro: Argumento Inválido! O valor correto para -i é um número inteiro maior que zero!");
	            return (-1);
            }
        }
        else{
            perror("Erro: Argumento Inválido! A terceira opção deveria ser -i.\n");
            return (-1);
        }
      
      
        if( argc >= 8 ){ // existe um 4o parametro?
            if( argc == 9 && !(strcmp(argv[7], "-o")) ){ // ele é -o?
	            (*arq_saida) = argv[8];
            }
            else{
	            // abortar e imprimir erro
                perror("Erro: Chamada Inválida!\nA chamada deve ser da forma 'pdeSolver -nx <Nx> -ny <Ny> -i <maxIter> -o arquivo_saida'.\n");
	            return (-1);
            }
        }
        else{
            // caso -o não exista, o valor NULL indicará que o programa deve usar a saída padrão
            (*arq_saida) = NULL;
        }
      
    }
    else{
        // abortar e imprimir erro
        perror("Erro: Número errado de argumentos! A chamada deve ser da forma 'pdeSolver -nx <Nx> -ny <Ny> -i <maxIter> -o arquivo_saida'.\n");
        return (-1);
    }

    return 0;
}


/*!
    \brief Implementação do método de Gauss-Siedel para a equação diferencial apresentada. 

    \param sistema Ponteiro para o sistema que representa a equação diferencial.
    \param u Vetor solução do sistema.
*/
// void gaussSiedel( Sist_Lin **sistema, Real_t **u );
void gaussSiedel( Sist_Lin *sistema, Real_t **u ){
    Real_t Uij;

    for(unsigned int i = 1; i < ((sistema->nx)+1); ++i){
        for(unsigned int j = 1; j < ((sistema->ny)+1); ++j){
            // Uij = sistema->b[ index(i,j,((sistema->ny)+2)) ] - ((sistema->dInfy) * (*u)[ index(i,j-1,(sistema->ny)+2) ]);
            // Uij -= ((sistema->dInfx) * (*u)[ index(i-1,j,(sistema->ny)+2) ]);
            // Uij -= ((sistema->dSupy) * (*u)[ index(i,j+1,(sistema->ny)+2) ]);
            // Uij -= ((sistema->dSupx) * (*u)[ index(i+1,j,(sistema->ny)+2) ]);
            // Uij /= (sistema->dPrin);
            // A ideia seria dividir a soma pela quantidade de registradores disponiveis para out of order execution

            // Uij = bij - dInfy*Uij-1 - dInfx*Ui-1j - dSupy*Uij+1 - dSupx*Ui+1j; 
            Uij = sistema->b[ index(i,j,((sistema->ny)+2)) ] - ((sistema->dInfy) * (*u)[ index(i,j-1,(sistema->ny)+2) ]) - ((sistema->dInfx) * (*u)[ index(i-1,j,(sistema->ny)+2) ]) - ((sistema->dSupy) * (*u)[ index(i,j+1,(sistema->ny)+2) ]) - ((sistema->dSupx) * (*u)[ index(i+1,j,(sistema->ny)+2) ]);
            Uij /= (sistema->dPrin);

            // // Calcula diferença entre aproximações ||  x(k) – x(k – 1) ||
            // (*err_aprox_atual) = fabs( (Uij - ((*u)[ index(i,j,(sistema->ny)+2) ])) );
            // (*maior_err_aprox) = ((*err_aprox_atual) > (*maior_err_aprox)) ? ((*err_aprox_atual)) : ((*maior_err_aprox));

            (*u)[ index(i,j,(sistema->ny)+2) ] = Uij;
        }
    }

    return;
}


/*!
    \brief Essa função calcula a norma L2 do resíduo de um sistema linear. 
 
    \param sist Ponteiro para o sistema que representa a equação diferencial.
    \param u Vetor solução do sistema.
*/
Real_t calcula_norma_residuo(Sist_Lin *sist, Real_t *u){

    Real_t *residuo = myMalloc(((sist->nx)+2)*((sist->ny)+2), Real_t);
    testMalloc(residuo);

    // Real_t residuo = 0; IMPLEMENTE A EXPRESSÃO PARA O RESIDUO!

    Real_t norma = 0;

    // calcula norma resíduo
    for(unsigned int i = 1; i < ((sist->nx)+1); ++i){
        for(unsigned int j = 1; j < ((sist->ny)+1); ++j){
            // residuo = b - Ax 
            residuo[ index(i,j,(sist->ny)+2) ] = (sist->b[ index(i,j,(sist->ny)+2) ]);
            residuo[ index(i,j,(sist->ny)+2) ] -= (((sist->dInfy) * u[ index(i,j-1,(sist->ny)+2) ]) + ((sist->dInfx) * u[ index(i-1,j,(sist->ny)+2) ]) + ((sist->dSupy) * u[ index(i,j+1,(sist->ny)+2) ]) + ((sist->dSupx) * u[ index(i+1,j,(sist->ny)+2) ]) + (sist->dPrin) * u[ index(i,j,(sist->ny)+2) ]);

            // calcula norma do vetor de residuo
            norma += residuo[ index(i,j,(sist->ny)+2) ] * residuo[ index(i,j,(sist->ny)+2) ];
        }
    }

    norma = sqrt(norma);

    libera_vetor(&residuo);

    return norma;
}

// Retorna tempo em milisegundos
/*!
    \brief Função disponibilizada pelos professores para a medição de tempo.
*/
double timestamp(void){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}


// Aloca as estrutura de dados usada na resolução da eq diferencial
// void aloca_sist(Sist_Lin** sist, unsigned int nx, unsigned int ny){

/*!
    \brief Essa função aloca as estruturas de dados que compôem um sistema linear no programa. 
    Constituído basicamente dos membros da struct Sist_Lin, essa função executa um malloc para o vetor b.
    Ela também o preenche com os valores de f(x,y). 
    
    \param sist Ponteiro para o sistema que representa a equação diferencial.
    \param nx Um inteiro sem sinal, o número de passos na dimensão x da região onde resolveremos a equação diferencial.
    \param ny Um inteiro sem sinal, o número de passos na dimensão y da região onde resolveremos a equação diferencial.
*/
// void aloca_sist( Sist_Lin** sist, unsigned int nx, unsigned int ny );
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
      sist->b[ index(i,j,(sist->ny)+2) ] = (4*PI*PI)*( (sin(2*PI*x) * sinh(PI*y)) + (sin(2*PI*(PI-x)) * sinh(PI*(PI-y))));
    }
  }

  // criar os valores que representam a matriz A
  sist->dInfy = -((1/(2*hx)) + (1/(hx*hx))); 
  sist->dInfx = -((1/(2*hy)) + (1/(hy*hy)));
  sist->dPrin = ((4*PI*PI)+(2/(hx*hx))+(2/(hy*hy)));
  sist->dSupy = -((1/(2*hy)) - (1/(hy*hy)));
  sist->dSupx = -((1/(2*hx)) - (1/(hx*hx)));

  // antes eu tinha o sistema assim a convergencia se dava de maneira diferente
  // sist->dInfy = -((1/(hx*hx)) + (1/(2*hx))); 
  // sist->dInfx = -((1/(hy*hy)) + (1/(2*hy)));
  // sist->dPrin = ((4*PI*PI)+(2/(hx*hx))+(2/(hy*hy)));
  // sist->dSupy = -((1/(hy*hy)) - (1/(2*hy)));
  // sist->dSupx = -((1/(hx*hx)) - (1/(2*hx)));

  return;
}


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
            
            (*u)[ index(i,j,(sist->ny)+2) ] = 0;
            // (*u)[ index(i,j,(sist->ny)+2) ] = rand();
        }
    }

  return;
}


int main(int argc, char *argv[]){
  srand(time(NULL));

  unsigned int nx, ny, iter;
  char *arq_saida;

  // ler entradas da linha de comando
  if( le_comandos( argc, argv, &nx, &ny, &iter, &arq_saida ) != (-1)){

    // // alocar e inicializar sistema de equações 
    // Sist_Lin *sistema;

    // Aloco o sistema na stack e passo uma referencia para alocar as estruturas internas e inicializá-las
    Sist_Lin sistema;
    aloca_sist( &sistema, nx, ny );

    // alocar e inicializar o vetor solução nulo
    Real_t *u;
    aloca_e_inicializa_solucao( &u, &sistema );

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
      norma_residuo_por_iter[k-1] = calcula_norma_residuo(&sistema, u);

    } while( ((k <= iter) && (norma_residuo_por_iter[k-1] >= MAXERR)) );

    // // Checar se o método convergiu
    // if(norma_residuo_por_iter[k-1] >= MAXERR){
    //   perror("Atenção! Não houve convergência para o método! Tente aumentar o número de iterações.\n");
      
    //   // libere a memoria usada para as estruturas
    //   libera_sist( &sistema );
    //   libera_vetor( &u );
    //   free( norma_residuo_por_iter );
    //   norma_residuo_por_iter = NULL;
      
    //   return (-1);
    // }
    // mesmo que o metodo nao convirga os valores devem ser impressos
    // retornar um valor dferente quandpo o metodo o metodo nao convergir assim o script de animação pode tomar decisões
    // algo como while not converge continua a aumentar o numero de iterações, caso o argumento de número de iterações for vazio

    // escreve o arquivo de saída
    escreve_solucao_gnuplot( arq_saida, tempo_total_GaussSiedel, k, norma_residuo_por_iter, &sistema, u );
    
    // libere a memoria usada para as estruturas
    libera_sist( &sistema );
    libera_vetor( &u );
    libera_vetor( &norma_residuo_por_iter );
  
  }else{

    return (-1);
  }

  return 0;
}

