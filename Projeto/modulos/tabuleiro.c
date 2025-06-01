/**
* @file tabuleiro.c
* @brief Este ficheiro contém o módulo 'tabuleiro' do Projeto Final de LI2 (24/25).
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
 * @brief Função que imprime o tabuleiro do puzzle
 */

bool imprimirTabuleiro(ESTADO *e) {
    // verifica se existe um tabuleiro (válido)
    if (e -> tabuleiro == NULL || e -> linhas <= 0 || e -> colunas <= 0) return false;

    // imprime o cabeçalho (letras das colunas) 
    printf("\n    ");
    for (int j = 0; j < e -> colunas; j++) printf(NEGRITO AZUL " %c" RESET, 'a' + j);
    printf("\n");

    // imprime a linha superior (moldura do tabuleiro)
    printf("   -");
    for (int j = 0; j < e -> colunas; j++) printf("--");
    printf("--\n");

    // imprime o conteúdo do tabuleiro
    for (int i = 0; i < e -> linhas; i++) {
        // imprime os números e a linha da esquerda (moldura do tabuleiro)
        if ((i + 1) < 10) printf(NEGRITO AZUL " %d " RESET "|", i + 1);  // números de 1 dígito
        else printf(NEGRITO AZUL "%d " RESET "|", i + 1);   // números de 2 dígitos
        
        // imprime as casas do tabuleiro
        for (int j = 0; j < e -> colunas; j++) printf(" %c", e -> tabuleiro[i][j]);
        
        // imprime linha da direita (moldura do tabuleiro)
        printf(" |\n");
    }

    // imprime a linha inferior (moldura do tabuleiro)
    printf("   -");
    for (int j = 0; j < e -> colunas; j++) printf("--");
    printf("--\n");

    return true;
}

/**
 * @brief Verifica se o tabuleiro está completamente resolvido.
 * 
 * @param e Ponteiro para a estrutura de estado do jogo.
 * @return true se o tabuleiro está resolvido e válido.
 * @return false caso contrário.
 * 
 * @details  Condições para ganhar:
 *          - Todas as células devem estar como branco (maiúsculas) ou riscadas (#)
 *          - O tabuleiro deve cumprir todas as regras
 *  
 * @see verificaRegras()
 * @see ESTADO
 */

bool tabuleiroResolvido(ESTADO *e) {
    // verifica se existem minúsculas
    for (int i = 0; i < e -> linhas; i++) {
        for (int j = 0; j < e -> colunas; j++) {
            if (isLower(e -> tabuleiro[i][j])) return false;
        }
    }
    
    // verifica se o tabuleiro é válido
    verificaRegras(e,0);
    
    return e -> tab_valid;
}

/**
 * @brief Função que lê o conteúdo de um tabuleiro de um dado ficheiro.
 * 
 * @param e Ponteiro para a estrutura de estado do jogo onde vai salvar o conteúdo do tabuleiro.
 * @param file Ficheiro de onde vai ler o conteúdo.
 * 
 */

bool lerConteudoTabuleiro(FILE *file, ESTADO *e) {
    // inicializar a string 'linha'
    char linha[100];
    // lê a primeira linha (dimensões do tabuleiro)
    fgets(linha, sizeof(linha), file);

    // itera sobre as linhas do tabuleiro
    for (int i = 0; i < e -> linhas; i++) {
        // lê uma linha do ficheiro
        if (fgets(linha, sizeof(linha), file) != NULL) {
            // associa cada caracter da linha lida, a uma casa do tabuleiro
            for (int j = 0; j < e -> colunas && j < strLen(linha) && linha[j] != '\n'; j++) {
                e -> tabuleiro[i][j] = linha[j];
            }
        }
        // erro: falha ao ler o tabuleiro
        else {
            fprintf(stderr, " Erro ao ler o conteúdo do tabuleiro do arquivo!\n");
            return false;
        }
    }

    return true;
}

/**
 * @brief Lê todas as informação do tabuleiro de um arquivo.
 * 
 * @param e Ponteiro para a estrutura de estado do jogo onde vai salvar o conteúdo do tabuleiro.
 * @param file Ficheiro de onde vai ler o conteúdo.
 * 
 * @see alocarMemoria()
 * @see leconteudoTabuleiro()
 * 
 */

bool lerTabuleiroDoArquivo(FILE *file, ESTADO *e) {
    // ler as dimensões do tabuleiro
    if (fscanf(file, "%d %d", &e -> linhas, &e -> colunas) != 2) {
        fprintf(stderr, NEGRITO AZUL " Erro:"RESET " Formato inválido no arquivo!\n");
        return false;
    }

    // alocar memória para o tabuleiro
    if (!alocarMemoria(e)) {
        fprintf(stderr, NEGRITO AZUL " Erro:"RESET" Falha ao alocar memória para o tabuleiro!\n");
        return false;
    }

    // ler o tabuleiro
    if (!lerConteudoTabuleiro(file, e)) {
        libertarMemoria(e);
        return false;
    }

    return true;
}

/**
 * @brief Carrega o histórico de jogadas a partir de um ficheiro.
 * 
 * @param file Ponteiro para o ficheiro aberto em modo leitura.
 * @param e    Ponteiro para a estrutura de estado do jogo.
 * 
 * @return Ponteiro para a lista ligada de JOGADA_SALVA carregada.
 *         NULL em caso de erro ou histórico vazio.
 * 
 * @details Esta função:
 *          1. Lê sequencialmente o ficheiro com formato:
 *             "x y c inicioCmd" por linha
 *          2. Valida coordenadas contra as dimensões do tabuleiro
 *          3. Aceita marcadores especiais (x=-1, y=-1) para início de comando
 *          4. Aloca memória para cada jogada e constrói a lista em ordem cronológica
 * 
 * 
 * @see JOGADA_SALVA
 * 
 */

JOGADA_SALVA* lerHistoricoJogadas(FILE *file, ESTADO *e) {
    // inicializar o histórico
    JOGADA_SALVA *historico = NULL;
    JOGADA_SALVA *ultimo = NULL;

    // inicializar variáveis do nodo do histórico
    int x, y, inicioCmd;
    char c;

    // ler as coordenadas, o caractere anterior e a marca de inicio do comando
    while (fscanf(file, "%d %d %c %d", &x, &y, &c, &inicioCmd) == 4) {
        // verifica se o histórico é válido
        if ((x >= 0 && x < e -> linhas && y >= 0 && y < e -> colunas) || (x == -1 && y == -1)) {
            // alocar memória para registar o nodo
            JOGADA_SALVA *novaJogada = malloc(sizeof(JOGADA_SALVA));
            // erro: falha a alocar memória
            if (!novaJogada) {
                // liberta o histórico
                while (historico) {
                    JOGADA_SALVA *tmp = historico;
                    historico = historico -> ant;
                    free(tmp);
                }
                return NULL;
            }

            // regista os parâmetros do nodo
            novaJogada -> x = x;
            novaJogada -> y = y;
            novaJogada -> c = c;
            novaJogada -> inicioComando = inicioCmd;
            novaJogada -> ant = NULL;

            // coloca os nodos no fim do histórico
            if (historico == NULL) historico = ultimo = novaJogada;
            else {
                ultimo -> ant = novaJogada;
                ultimo = novaJogada;
            }
        }

        else fprintf(stderr,NEGRITO LARANJA " Aviso:"RESET" Jogada inválida encontrada no ficheiro (%d %d %c).\n", x, y, c);
    }

    return historico;
}

/**
 * @brief Verifica se o tabuleiro está carregado
 */

bool tabuleiroCarregado(ESTADO *e) {
    if (e -> tabuleiro == NULL || e -> linhas <= 0 || e -> colunas <= 0) {
        fprintf(stderr, AZUL NEGRITO "\nErro:" RESET " Tabuleiro não carregado!\n");
        return false;
    }

    return true;
}

/**
 * @brief Função para gravar o conteúdo do tabuleiro
 */

void gravarTabuleiro(FILE *file, ESTADO *e) {
    for (int i = 0; i < e -> linhas; i++) {
        for (int j = 0; j < e -> colunas; j++) {
            fputc(e -> tabuleiro[i][j], file);
        }
        fputc('\n', file);
    }
}

/**
 * @brief Função para gravar o histórico de jogadas.
 */

void gravarHistorico(FILE *file, ESTADO *e) {
    JOGADA_SALVA *atual = e->historico;
    while (atual != NULL) {
        int inicioCmd;
        if (atual->inicioComando) {
            inicioCmd = 1;
        } else {
            inicioCmd = 0;
        }
        fprintf(file, "%d %d %c %d\n", atual->x, atual->y, atual->c, inicioCmd);
        atual = atual->ant;
    }
}

/**
 * @brief Função que verifica se o histórico é vazio.
 */

bool verificarHistoricoVazio(ESTADO *estadoAtual) {
    // se histórico == NULL, está vazio
    if (!estadoAtual -> historico) {
        printf(NEGRITO "\n Oops! Parece que já não tens mais jogadas para desfazer...\n" RESET);
        return true;
    }

    return false;
}

/**
 * @brief  Função para restaurar o estado do tabuleiro a partir de uma jogada
 */

void restaurarJogada(ESTADO *estadoAtual, JOGADA_SALVA *jogada) {
    // se as coordenadas forem válidas, alterar o caracter pelo do histórico
    if (jogada -> x != -1 && jogada -> y != -1) estadoAtual -> tabuleiro[jogada -> x][jogada -> y] = jogada -> c;
}

/**
 * @brief Função para processar as jogadas até encontrar o início do comando
 */

JOGADA_SALVA* processarJogadas(ESTADO *estadoAtual, JOGADA_SALVA **anterior) {
    JOGADA_SALVA *atual = estadoAtual->historico;
    *anterior = NULL;
    
    while (atual != NULL && !atual->inicioComando) {
        restaurarJogada(estadoAtual, atual);
        
        JOGADA_SALVA *temp = atual;
        atual = atual->ant;
        
        if (*anterior == NULL) {
            estadoAtual->historico = atual;
        } else {
            (*anterior)->ant = atual;
        }
        
        free(temp);
    }
    
    return atual;
}

/**
 * @brief Função para remover o marcador de início de comando
 */

void removerMarcadorComando(ESTADO *estadoAtual, JOGADA_SALVA *atual, JOGADA_SALVA *anterior) {
    if (atual != NULL && atual -> inicioComando) {
        // inicializar ponteiro temporário
        JOGADA_SALVA *temp = atual;

        // se houver apenas um nodo no histórico
        if (anterior == NULL) estadoAtual -> historico = atual -> ant;
        // se houver mais do que um nodo
        else anterior -> ant = atual -> ant;

        free(temp);
    }
}

