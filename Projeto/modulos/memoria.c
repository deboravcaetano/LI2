/**
* @file memoria.c
* @brief Este ficheiro contém o módulo 'memoria' do Projeto Final de LI2 (24/25).
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
 * @brief Liberta a memória alocada dinamicamente para o tabuleiro do jogo.
 * 
 * @param e Ponteiro para a estrutura de estado do jogo.
 * 
 * @details Esta função:
 *          1. Verifica se o tabuleiro existe (não é NULL)
 *          2. Liberta individualmente cada linha da matriz
 *          3. Liberta a estrutura
 *          4. Define o ponteiro do tabuleiro como NULL para prevenir uso pós-liberação
 * 
 * 
 * @see ESTADO
 */

void libertarMemoria(ESTADO *e) {
    if (e -> tabuleiro != NULL) {
        for (int i = 0; i < e -> linhas; i++) free(e -> tabuleiro[i]);
        free(e -> tabuleiro);
        e -> tabuleiro = NULL;
    }

    if (e -> caminhoFicheiro != NULL) {
        free(e -> caminhoFicheiro);
        e -> caminhoFicheiro = NULL;
    }
}

/**
 * @brief Aloca memória dinamicamente para o tabuleiro do jogo.
 * 
 * @param e Ponteiro para a estrutura de estado do jogo.
 * @return true se a alocação foi bem-sucedida.
 * @return false se ocorrer falha na alocação de memória.
 * 
 * @details Esta função:
 *          1. Aloca memória para um array de ponteiros (linhas).
 *          2. Para cada linha, aloca memória para um array de caracteres (colunas).
 *          3. Em caso de falha em qualquer etapa:
 *             - Liberta toda a memória alocada parcialmente.
 *             - Mantém o estado consistente (tabuleiro = NULL)
 *             - Retorna imediatamente com false
 * 
 * @see libertarMemoria()
 */

bool alocarMemoria(ESTADO *e) {
    // aloca n linhas para o tabuleiro
    e -> tabuleiro = malloc(e -> linhas * sizeof(char *));
    // erro: falha na alocação
    if (e -> tabuleiro == NULL) return false;

    // iteração sobre as linhas do tabuleiro
    for (int i = 0; i < e -> linhas; i++) {
        // aloca n colunas para cada linha
        e -> tabuleiro[i] = malloc(e -> colunas * sizeof(char));
        // erro: falha na alocação (liberta a  memória alocada parcialmente)
        if (e -> tabuleiro[i] == NULL) {
            // para saber quantas linhas já estavam alocadas
            e -> linhas = i;
            // liberta a memória do tabuleiro
            libertarMemoria(e);
            return false;
        }
    }
    
    return true;
}

/**
 * @brief Liberta toda a memória alocada para o histórico de jogadas.
 * 
 * @param e Ponteiro para a estrutura de estado do jogo.
 * 
 * @details Esta função:
 *          1. Percorre a lista ligada de JOGADA_SALVA do topo até o fim
 *          2. Liberta cada nó individualmente
 *          3. Atualiza o ponteiro do histórico para NULL
 * 
 * 
 * @see JOGADA_SALVA
 * @see libertarMemoria()
 */

void libertarHistorico(ESTADO *e) {
    // regista o topo da stack 'historico'
    JOGADA_SALVA *atual = e -> historico;

    // enquanto houver elementos no histórico
    while (atual != NULL) {
        // regista o elemento anterior da stack
        JOGADA_SALVA *proximo = atual -> ant; 
        // liberta a memória do topo da stack
        free(atual);                       
        // define o topo da stack como o elemento anterior
        atual = proximo;
    }

    // define o ponteiro do histórico como NULL
    e -> historico = NULL;  
}

/**
 * @brief Liberta memória de cópia de estado.
 */

void libertarCopiaEstado(ESTADO *copia) {
    for (int i = 0; i < copia -> linhas; i++) free(copia -> tabuleiro[i]);
    free(copia -> tabuleiro);
    copia -> tabuleiro = NULL;

    if (copia -> caminhoFicheiro != NULL) {
        free(copia -> caminhoFicheiro);
        copia -> caminhoFicheiro = NULL;
    }
}

/**
 * @brief Liberta memória da queue
 */

void freeQueue(Queue q) {
    // verifica se a queue está vazia
    if (!q) return;

    // liberta a memória de todos os valores da queue
    free(q -> valores);
    // liberta a memória da queue (estrutura)
    free(q);
}