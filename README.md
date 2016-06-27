# First and Follow
Programa escrito na linguagem C++ para determinar os conjuntos **First And Follow** de uma determinada gramática, como mostra o exemplo a seguir: 

***OBS***: O símbolo ```*``` é usado como vazio e ```$``` como final.
O arquivo gramaticas.txt contém exemplos de gramáticas para serem usados como entrada.

### Entrada:
O arquivo de entrada deve seguir o modelo a seguir:
```
    G = (N,T,P,S)
    N = {S}
    T = {+,x,a} 
    P = { 
        S -> +SS | xSS | a
    }
```
- **G** = Definição da Gramática ( Conjunto das variáveis, Terminais, Derivações, Variável Inicial )
- **N** = Conjunto das Variáveis.
- **T** = Conjunto dos terminais
- **P** = Derivações

### Saída:
Serão informados conjunto das variáveis, conjunto dos simbolos, conjunto First e Follow.
```
CONJUNTO DAS VARIÁVEIS: S 
CONJUNTO DOS TERMINAIS: + a x 
First S: + a x 

Follow S: $ + a x 
```
### Execução

Para executar será necessário ter instalado o [GCC](https://gcc.gnu.org/). Para compilar o arquivo
``` 
    $ g++ -o FirstAndFollow FirstAndFollow.cpp
    $ ./FirstAndFollow < ```ArquivoComGramatica**```
```

    


