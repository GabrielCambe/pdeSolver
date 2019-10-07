/*! 
    \mainpage Informações Gerais
    \section intro Introduction
  
    O programa apresentado nessa documentação resolve a seguinte equação diferencial:
    
    <img src="http://latex.codecogs.com/svg.latex?(\partial u/\partial x + \partial u/\partial y) - (\partial^2 u/\partial^2 x + \partial^2 u/\partial^2 y)=" border="0"/>
  
    <img src="http://latex.codecogs.com/svg.latex?4*\pi^2(sin(2*\pi*x)*sinh(\pi*y)+sin(2*\pi*(\pi-x))*sinh(\pi(\pi-y)))," border="0"/>

    com <img src="http://latex.codecogs.com/svg.latex?(x,y) \in [0,\pi] \times [0,\pi]" border="0"/>.

    Onde u, que é função de x e y, é a função que quero aproximar.

    Para resolver esse problema usei o método das diferenças finitas e o método de gauss-Siedel para a resolução do sistema linear resultante da transformação finita.

    \section discussion Resolução
  

    \subsection step1 Diferenças Finitas: Discretizar um problema contínuo.

    Como não se pode pensar em limites como usualmente se pensa dentro de uma máquina de precisão finita, é importante utilizar métodos finitos para a aproximação da derivada.

    Inicialmente passamos a observar cada ponto u(x,y) se imaginarmos eles igualmente espaçados em cada dimensão, podemos definir passos hx e hy de maneira que a aproximação por diferenças finitas centrais para a derivada primeira e segunda para x e para y são as seguintes:

    <img src="http://latex.codecogs.com/svg.latex?\partial u/\partial x = (u(x+hx,y)-u(x-hx,y))/(2*hx)" border="0"/>.
    
    <img src="http://latex.codecogs.com/svg.latex?\partial u/\partial y = (u(x,y+hy)-u(x,y-hy))/(2*hy)" border="0"/>.
    
    <img src="http://latex.codecogs.com/svg.latex?\partial^2 u/\partial^2 x = (u(x+hx,y)-2u(x,y)+ u(x-hx,y))/(hx^2)" border="0"/>.
    
    <img src="http://latex.codecogs.com/svg.latex?\partial^2 u/\partial^2 y = (u(x,y+hy)-2u(x,y)+u(x,y-hy))/(hy^2)" border="0"/>.

    Assim u(x,y) é equivalente a Uij onde um aumento de 1 unidade em i ou j representa um passo de hx ou hy.

    O sistema linear que surgiu foi o resultado de tratar os nx*ny pontos da equação diferencial como um vetor de valores reais (as valores de Uij).
    
    A matriz A resultante é uma matriz em faixas, com cinco diagonais. A principal, que multiplica o termo Uij e as outras são para os outros termos de U que aparecem na equação discretizada.

  
    \subsection step2 Gauss Siedel: Calcular a aproximação numérica.
    
    Para a iteração do Gaus-Siedel, isolo o elemento Uij e calculo o valor da expressão dele.

    A condição de parada é a norma do resíduo para a iteração atual, que é comparado com um valor de erro máximo MAXERR.    

    \section Uso
    A execução do algorítmo é como está expecificada na especificação.
    Para plotar o gráfico da função calculada basta usar "make plot3d" no diretorio raiz após ter executado o método pelo menos uma vez.

    \author Gabriel Sales GRR20165266
    \date 06-10-2019
    \copyright <a href="https://www.gnu.org/licenses/gpl-3.0-standalone.html">GPLv3</a>
    
 */
