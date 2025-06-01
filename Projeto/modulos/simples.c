/**
* @file simples.c
* @brief Este ficheiro contém o módulo 'simples' do Projeto Final de LI2 (24/25).
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
 * @brief Executa o comando 'clear' no terminal
 */

int clear () {
    system("clear");
    return E_OK;
}

/**
 * @brief Converte uma letra minúscula num número inteiro
 */

int toInt_Alpha (char c) {
    return c - 'a';
}

/**
 * @brief Converte um caracter num número inteiro
 */

int toInt_Number (char c) {
    return c - '0';
}

/**
 * @brief Verifica se é um dígito
 */

int isDigit (char c) {
    return ('0' <= c && c <= '9');
}

/**
 * @brief Verifica se é minúscula
 */

int isLower (char c) {
    return ('a' <= c && c <= 'z');
}


/**
 * @brief Verifica se é maiúscula
 */

bool isUpper(char c) {
    return (c >= 'A' && c <= 'Z');
}

/**
 * @brief Converte uma letra maiúscula numa minúscula
 */

char toLower(char c) {
    // verifica se a letra é maiúscula antes da conversão
    if (isUpper(c)) return c + ('a' - 'A');
    return c;
}

/**
 * @brief Converte uma letra minúscula numa maiúscula
 */

char toUpper(char c) {
    // verifica se a letra é minúscula antes da conversão
    if (isLower(c)) return c - ('a' - 'A');
    return c;  
}

/**
 * @brief Calcula o comprimento de uma string
 */

int strLen(char *string) {
    int comprimento = 0;
    while (string[comprimento] != '\0') comprimento++;
    return comprimento;
}

// Mensagem inicial do jogo
int mensagemInicial(){
    printf("\n ____________________________________________________________________________________________________________________________________________________________________________________________\n\n\n");
    printf(ROSA"                                                   ██▓███    █    ██ ▒███████▒▒███████▒ ██▓    ▓█████      ▄████  ▄▄▄       ███▄ ▄███▓▓█████\n");
    printf("                                                   ▓██░  ██▒ ██  ▓██▒▒ ▒ ▒ ▄▀░▒ ▒ ▒ ▄▀░▓██▒    ▓█   ▀     ██▒ ▀█▒▒████▄    ▓██▒▀█▀ ██▒▓█   ▀\n");
    printf("                                                   ▓██░ ██▓▒▓██  ▒██░░ ▒ ▄▀▒░ ░ ▒ ▄▀▒░ ▒██░    ▒███      ▒██░▄▄▄░▒██  ▀█▄  ▓██    ▓██░▒███  \n");
    printf("                                                   ▒██▄█▓▒ ▒▓▓█  ░██░  ▄▀▒   ░  ▄▀▒   ░▒██░    ▒▓█  ▄    ░▓█  ██▓░██▄▄▄▄██ ▒██    ▒██ ▒▓█  ▄\n");
    printf("                                                   ▒██▒ ░  ░▒▒█████▓ ▒███████▒▒███████▒░██████▒░▒████▒   ░▒▓███▀▒ ▓█   ▓██▒▒██▒   ░██▒░▒████▒\n");
    printf("                                                   ▒▓▒░ ░  ░░▒▓▒ ▒ ▒ ░▒▒ ▓░▒░▒░▒▒ ▓░▒░▒░ ▒░▓  ░░░ ▒░ ░    ░▒   ▒  ▒▒   ▓▒█░░ ▒░   ░  ░░░ ▒░ ░\n");
    printf("                                                   ░▒ ░     ░░▒░ ░ ░ ░░▒ ▒ ░ ▒░░▒ ▒ ░ ▒░ ░ ▒  ░ ░ ░  ░     ░   ░   ▒   ▒▒ ░░  ░      ░ ░ ░  ░\n");
    printf("                                                   ░░        ░░░ ░ ░ ░ ░ ░ ░ ░░ ░ ░ ░ ░  ░ ░      ░      ░ ░   ░   ░   ▒   ░      ░      ░   \n");
    printf("                                                             ░       ░ ░      ░ ░        ░  ░   ░  ░         ░       ░  ░       ░      ░  ░\n" RESET);
    printf(NEGRITO"\n                                                                           PRESSIONA w PARA DESCOBRIRES COMO JOGAR"RESET);
    printf(NEGRITO"\n                                                                    PRESSIONA h PARA DESCOBRIRES OS COMANDOS QUE PODES USAR"RESET);
    printf("\n");
    printf("\n ____________________________________________________________________________________________________________________________________________________________________________________________\n\n");
    return E_OK;
}