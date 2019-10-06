#include "utils.h"

// Trata a entrada passada pela linha de comando
/* PRECISA MUDAR A CHECAGEM DA VALIDADE DOS ARGUMENTOS -nx, -ny e -i PARA C CERTIFICAR QUE NÃO EXISTEM LETRAS NO NUMERO PASSADO */
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