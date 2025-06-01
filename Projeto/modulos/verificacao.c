/**
* @file verificacao.c
* @brief Este ficheiro contém o módulo 'verificacao' do Projeto Final de LI2 (24/25).
* 
* Projeto Final - Puzzle Game
* Laboratórios de Informática II (UMinho 24/25) 
*
* (c) Débora Luana Pinto Caetano    <a112332@alunos.uminho.pt>, 2025-06-01
* (c) Marcos Bernardo da Silva Lobo <a110959@alunos.uminho.pt>, 2025-06-01
* (c) Maria Inês da Cruz Machado    <a111464@alunos.uminho.pt>, 2025-06-01
*/

#include "type.h"
#include "erros.h"

/**
 * @brief Função que verifica se um tabuleiro cumpre todas as regras do jogo.
 */

bool verificaRegras(ESTADO *e, bool imprime) {
    // validade do tabuleiro
    bool valido = true;

    if (!verificarCaminho(e,imprime)) valido = false;
    
    if (!verificarBrancasRepetidas(e,imprime)) valido = false;
    
    if (!verificarVizinhasRiscadas(e,imprime)) valido = false;

    // se o tabuleiro for válido
    if (valido) {
        if (imprime){
           printf(VERDE NEGRITO "\n Puzzle válido!"RESET " Nenhuma restrição violada.\n");
        }
        e -> tab_valid = true;
    } else {
        e -> tab_valid = false;
    }

    e->tab_valid = valido;

    return valido;
}

/**
 * @brief Função que inicializa os parâmetros da queue
 */

Queue criarQueue(int capacidade) {
    // se o tabuleiro estiver vazio
    if (capacidade <= 0) return NULL;
    
    // aloca memória para a queue
    Queue q = malloc(sizeof(struct queue));
    // erro: falha ao alocar memória
    if (!q) return NULL;
    
    // alocar memória para a lista de valores
    q -> valores = malloc(capacidade * sizeof(Coordenada));
    if (!q -> valores) {
        free(q);
        return NULL;
    }
    
    // inicializar a queue
    q -> capacidade = capacidade;
    q -> front = 0;
    q -> nElem = 0;
    return q;
}

/**
 * @brief Função que insere um elemento na queue
 */

bool enqueue(Queue q, Coordenada coord) {
    // se a queue estiver cheia ou for inválida, aborta
    if (!q || q -> nElem >= q -> capacidade) return false;

    // adiciona o valor à queue e aumenta o contador de elementos
    q -> valores[traduzIndice(q -> front + q -> nElem, q -> capacidade)] = coord;
    q -> nElem++;

    return true;
}

/**
 * @brief Função que calcula o índice
 * 
 * Garante com que a queue seja circular, ou seja, que ind volte para 0 quando encontra o final do array.
 */

int traduzIndice(int ind, int max) {
    if (ind < max) return ind;
    else return ind - max;
}

/**
 * @brief Função que remove o topo da queue.
 */

Coordenada dequeue(Queue q) {
    // coordenada inválida caso a queue esteja vazia
    Coordenada coord = {-1, -1};

    // se a queue estiver vazia ou for inválida, aborta
    if (!q || q -> nElem <= 0) return coord;
    
    // guarda o topo da queue
    coord = q -> valores[q -> front];

    // atualiza o topo e o contador de elementos
    q -> front = traduzIndice(q -> front + 1, q -> capacidade);
    q -> nElem--;

    return coord;
}
 
/**
 * @brief Função que verifica se a queue está vazia.
 */

bool isEmpty(Queue q) {
    return !q || q -> nElem == 0;
}

/**
 * @brief Função que verifica se há um caminho ortogonal entre todas as casas.
 * 
 * @see encontrarCelulaInicial()
 * @see inicializarVisitados()
 * @see realizarBusca()
 */

bool verificarCaminho(ESTADO *e, int imprime) {
    // o tabuleiro não é válido
    if (e -> linhas == 0 || e -> colunas == 0) return true; 
    
    // inicializar array de visitados e número de células não riscadas
    int visitado[e->linhas][e->colunas];
    int totalCelulasNaoRiscadas;
    inicializarVisitados(e, visitado, &totalCelulasNaoRiscadas);
    
    // guardar coordenada inicial
    Coordenada inicio = encontrarCelulaInicial(e);

    // tabuleiro só com células riscadas
    if (inicio.x == -1) return false;  
    
    // número de células não riscadas que é possível alcançar a partir da célula inicial
    int celulasAlcancadas = realizarBusca(e, inicio, visitado);
    
    // se o número de células alcançadas == número de células não riscadas, há caminho
    if (celulasAlcancadas == totalCelulasNaoRiscadas) return true;
    else {
        // se necessário imprime uma mensagem (argumentos da função)
        if (imprime) printf(NEGRITO VERMELHO"\n Violação:" RESET" Não existe um caminho ortogonal entre todas as casas.\n");
        return false;
    }
}

/**
 * @brief Função que verifica se há letras maiúsculas repetidas na mesma linha ou coluna.
 * 
 * @see isUpper()
 */

bool verificarBrancasRepetidas(ESTADO *e, int imprime) {
    // validade do tabuleiro
    bool valido = true;

    // iteração sobre as casas do tabuleiro
    for (int i = 0; i < e -> linhas; i++) {
        for (int j = 0; j < e -> colunas; j++) {

            // célula atual
            char celula = e -> tabuleiro[i][j];
            
            if (isUpper(celula)) {
                // iteração sobre a linha
                for (int k = j + 1; k < e -> colunas; k++) {
                    if (e -> tabuleiro[i][k] == celula) {
                        // imprime (argumentos da função)
                        if (imprime) printf(NEGRITO VERMELHO "\n Violação:" RESET " Letra '%c' repetida em %c%d.\n", celula, 'a' + k, i + 1);
                        // há brancas repetidas na linha
                        valido = false;
                    }
                }

                // iteração sobre a coluna
                for (int k = i + 1; k < e -> linhas; k++) {
                    if (e -> tabuleiro[k][j] == celula) {
                        // imprime (argumentos da função)
                        if (imprime) printf(NEGRITO VERMELHO"\n Violação:" RESET" Letra '%c' repetida em %c%d.\n", celula, 'a' + j, k + 1);
                        // há brancas repetidas na coluna
                        valido = false;
                    }
                }
            }

        }
    }
    
    return valido;
}

/**
 * @brief Função que verifica se existem células riscadas juntas.
 */

bool verificarVizinhasRiscadas(ESTADO *e, int imprime) {
    // validade do tabuleiro
    bool valido = true;

    // iteração sobre as casas do tabuleiro
    for (int i = 0; i < e -> linhas; i++) {
        for (int j = 0; j < e -> colunas; j++) {
            
            // célula atual  
            char celula = e->tabuleiro[i][j];
            
            // iteração sobre a linha (verifica apenas a célula da direita)
            if (celula == '#') {
                if (j + 1 < e -> colunas && e -> tabuleiro[i][j+1] == celula) {
                    // imprime (argumentos da função)
                    if (imprime) printf(NEGRITO VERMELHO "\n Violação:" RESET " Células riscadas juntas em %c%d e %c%d.\n", 'a' + j, i + 1 , 'a' + j + 1, i + 1);
                    // há riscadas repetidas na linha
                    valido = false;
                }
            }

            // iteração sobre a linha (verifica apenas a célula de baixo)
            if (celula == '#') {
                if (i + 1 < e -> linhas && e -> tabuleiro[i+1][j] == celula) {
                    // imprime (argumentos da função)
                    if (imprime) printf(NEGRITO VERMELHO "\n Violação:" RESET" Células riscadas juntas em %c%d e %c%d.\n", 'a' + j, i + 1, 'a' + j, i + 2);
                    // há riscadas repetidas na linha
                    valido = false;
                }
            }
        }
    }
 
    return valido;
}