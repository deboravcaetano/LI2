/**
* @file estado.c
* @brief Este ficheiro contém o módulo 'estado' do Projeto Final de LI2 (24/25).
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
 * @brief Cria uma cópia do estado atual para depois conseguir voltar em caso de erro.
 */

ESTADO* criaCopiaEstado(ESTADO *original) {
    if (!original) return NULL;

    ESTADO *copia = malloc(sizeof(ESTADO));
    if (!copia) return NULL;

    copia -> looping = original -> looping;
    copia -> linhas = original -> linhas;
    copia -> colunas = original -> colunas;
    copia -> tab_valid = original -> tab_valid;

    copia -> tabuleiro = NULL;
    copia -> historico = NULL;
    copia -> caminhoFicheiro = NULL;
  
    copia -> tabuleiro = malloc(copia -> linhas * sizeof(char *));
    if (!copia -> tabuleiro) {
        free(copia);
        return NULL;
    }

    for (int i = 0; i < copia -> linhas; i++) {
        copia -> tabuleiro[i] = malloc(copia -> colunas * sizeof(char));
        if (!copia -> tabuleiro[i]) {
            for (int j = 0; j < i; j++) free(copia -> tabuleiro[j]);
            free(copia -> tabuleiro);
            free(copia);
            return NULL;
        }
        memcpy(copia -> tabuleiro[i], original -> tabuleiro[i], copia -> colunas);
    }

    if (original -> caminhoFicheiro) {
        copia -> caminhoFicheiro = strdup(original -> caminhoFicheiro);
        if (!copia -> caminhoFicheiro) {
            for (int i = 0; i < copia -> linhas; i++) free(copia -> tabuleiro[i]);
            free(copia -> tabuleiro);
            free(copia);
            return NULL;
        }
    }

    return copia;
}

/**
 * @brief Restaura o estado do jogo em caso de falha em resolver o puzzle na função resolverPuzzle
 * 
 * @param destino Estado onde estamos a trabalhar
 * @param origem Copia do estado antes da tentativa de resolver
 */

void restaurarEstado(ESTADO *destino, ESTADO *origem) {
    for (int i = 0; i < destino->linhas; i++) {
        memcpy(destino -> tabuleiro[i], origem -> tabuleiro[i], destino -> colunas * sizeof(char));
    }

    destino -> tab_valid = origem -> tab_valid;
    destino -> looping = origem -> looping;

    if (destino -> caminhoFicheiro) {
        free(destino -> caminhoFicheiro);
        destino -> caminhoFicheiro = NULL;
    }
    if (origem -> caminhoFicheiro) {
        destino -> caminhoFicheiro = strdup(origem -> caminhoFicheiro);
        if (!destino -> caminhoFicheiro) {
            fprintf(stderr, NEGRITO AZUL "Erro:"RESET " Falha a copiar o caminho.\n");
        }
    }
}

/**
 * @brief Função que guarda uma jogada no histórico.
 * 
 * @param estadoAtual  Ponteiro para a estrutura de estado do jogo.
 * @param x Coordenada x onde ocorreu alteração.
 * @param y Coordenada y onde ocorreu alteração.
 * @param caracterAnterior Estado da célula antes de ser alterada.
 */

// Guardar o estado do puzzle a cada alteração
bool guardarEstado(ESTADO *estadoAtual, int x, int y, char caracterAnterior) {
    // se o estado atual ou o tabuleiro não forem válidos, não guarda o estado
    if (!estadoAtual || x < 0 || x >= estadoAtual -> linhas || y < 0 || y >= estadoAtual -> colunas) {
        return false;
    }

    // alocação de memória para registo da última jogada no histórico
    JOGADA_SALVA *novaJogada = malloc(sizeof(JOGADA_SALVA));
    // erro: alocação de memória falhou
    if (!novaJogada) return false;

    // inicialização do registo
    novaJogada -> x = x;
    novaJogada -> y = y;
    novaJogada -> c = caracterAnterior; 
    novaJogada -> inicioComando = false;
    novaJogada -> ant = estadoAtual -> historico; 

    // adicionar o registo ao histórico
    estadoAtual -> historico = novaJogada;      

    return true;
}