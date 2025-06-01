/**
* @file type.h
* @brief Este ficheiro contém o módulo 'type' do Projeto Final de LI2 (24/25).
* 
* Projeto Final - Puzzle Game
* Laboratórios de Informática II (UMinho 24/25) 
*
* (c) Débora Luana Pinto Caetano    <a112332@alunos.uminho.pt>, 2025-06-01
* (c) Marcos Bernardo da Silva Lobo <a110959@alunos.uminho.pt>, 2025-06-01
* (c) Maria Inês da Cruz Machado    <a111464@alunos.uminho.pt>, 2025-06-01
*/

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 1024


// Cores de texto
#define AZUL       "\033[34m"     
#define ROSA       "\033[38;2;255;105;180m"   
#define VERDE      "\033[32m"      
#define CIANO      "\033[36m"   
#define NEGRITO    "\033[1m"
#define ITALICO    "\033[3m"  
#define LARANJA    "\033[38;5;208m"
#define VERMELHO   "\033[38;5;196m" 
#define ROSA_CLARO "\033[38;5;218m"
#define RESET      "\033[0m"


/**
 * @struct jogadaSalva
 * @brief Estrutura que armazena uma jogada no jogo.
 */
typedef struct jogadaSalva {
    int x; /**< Coordenada x onde foram feitas as mudanças. */
    int y; /**< Coordenada y onde foram feitas as mudanças. */
    char c;   /**< Estado do caracter antes de ser alterado. */
    struct jogadaSalva *ant; /**< Ponteiro para a jogada anterior. */
    bool inicioComando; /**< Indica se é o início de um comando. */
} JOGADA_SALVA;


/**
 * @struct estado
 * @brief Estrutura que armazena o estado atual do jogo.
 */
typedef struct estado {
    bool looping;  /**< Indica se o jogo decorre ou não. */
    bool tab_valid; /**< Indica se o tabuleiro é válido. */
    int linhas; /**< Indica tamanho do tabuleiro. */
    int colunas; /**< Indica tamanho do tabuleiro. */
    char **tabuleiro; /**< Tabuleiro. */
    JOGADA_SALVA *historico;  /**< Ponteiro para o histórico de jogadas do tipo JOGADA_SALVA. */
    char *caminhoFicheiro; /**< Caminho do ficheiro do puzzle. */
} ESTADO;

/**
 * @brief Estrutura de coordenadas.
 */
typedef struct {
    int x; /**< Coordenada em x. */
    int y; /**< Coordenada em y. */
} Coordenada;


/**
 * @brief Estrutura de uma queue circular.
 */
typedef struct queue {
    int capacidade; /**< Cpacidade total do array valores. */ 
    int front;  /**< Indica o índice da frente. */
    int nElem; /**< Número de elementos no array de valores. */
    Coordenada *valores; /**< Array com as coordenadas. */
} *Queue;

  
typedef int (*COMANDO) (char cmd, char *arg, ESTADO *e);


// Funções em comandos.c 
int gravar (char cmd, char *arg, ESTADO *e);
int sair(char cmd, char *arg, ESTADO *e);
int ler(char cmd, char *arg, ESTADO *e);
int riscar(char cmd, char *arg, ESTADO *e);
int branco (char cmd, char *arg, ESTADO *e);
int desfazer(char cmd, char *arg, ESTADO *estadoAtual);
int verificar(char cmd, char *arg, ESTADO *e);
int help(char cmd, char *arg, ESTADO *e);
int autocompletar(char cmd, char *arg, ESTADO *e);
int autocompletarRec(char cmd, char *arg, ESTADO *e);
int regras(char cmd, char *arg, ESTADO *e);
int resolver(char cmd, char *arg, ESTADO *e);
int limpa (char cmd, char *arg, ESTADO *e);
int desfazerCoordenada (char cmd, char *arg, ESTADO *e);

// Funções em simples.c
int clear ();
int toInt_Alpha (char c);
int toInt_Number (char c);
int isDigit (char c);
int isLower (char c);
bool isUpper(char c);
char toLower(char c);
char toUpper(char c);
int strLen(char *string);
int mensagemInicial();

// Funções em memoria.c
void libertarMemoria(ESTADO *e);
bool alocarMemoria(ESTADO *e);
void libertarHistorico(ESTADO *e);
void libertarCopiaEstado(ESTADO *copia);
void freeQueue(Queue q);

// Funções em verifica.c
bool verificaRegras(ESTADO *e, bool imprime);
Queue criarQueue(int capacidade);
bool enqueue(Queue q, Coordenada coord);
int traduzIndice(int ind, int max);
Coordenada dequeue(Queue q);
bool isEmpty(Queue q);
bool verificarCaminho(ESTADO *e, int imprime);
bool verificarBrancasRepetidas(ESTADO *e, int imprime);
bool verificarVizinhasRiscadas(ESTADO *e, int imprime);

// Funções em regras.c
bool riscarLetrasIguaisBrancas(ESTADO *e);
bool pintarVizinhasDeRiscadas(ESTADO *e);
void marcaLetras(ESTADO *e, int i, int j, bool visitado[e->linhas][e->colunas]);
bool todasConectadas(ESTADO *e);
bool pintarCasasQueIsolamLetras(ESTADO *e);

// Funções em estado.c
ESTADO *criaCopiaEstado(ESTADO *atual);
void restaurarEstado(ESTADO *destino, ESTADO *origem);
bool guardarEstado(ESTADO *estadoAtual, int x, int y, char caracterAnterior);

// Funções em tabuleiro.c
bool imprimirTabuleiro(ESTADO *e);
bool tabuleiroResolvido(ESTADO *e);
bool lerConteudoTabuleiro(FILE *file, ESTADO *e);
bool lerTabuleiroDoArquivo(FILE *file, ESTADO *e);
JOGADA_SALVA* lerHistoricoJogadas(FILE *file, ESTADO *e);
bool tabuleiroCarregado(ESTADO *e);
void gravarTabuleiro(FILE *file, ESTADO *e);
void gravarHistorico(FILE *file, ESTADO *e);
bool verificarHistoricoVazio(ESTADO *estadoAtual);
void restaurarJogada(ESTADO *estadoAtual, JOGADA_SALVA *jogada);
JOGADA_SALVA* processarJogadas(ESTADO *estadoAtual, JOGADA_SALVA **anterior);
void removerMarcadorComando(ESTADO *estadoAtual, JOGADA_SALVA *atual, JOGADA_SALVA *anterior);

// Funções em auxiliares.c
bool aplicaRegras(ESTADO *e);
void marcaInicioComando(ESTADO *e);
bool resolverPuzzle (ESTADO *e, ESTADO *original);
int lerLinha(char *cmd, char *arg);
int executarComando(COMANDO comandos[], char cmd, char *arg, ESTADO *estado);
bool argumentoVazio(char cmd, char *arg);
bool argumentoNaoVazio(char cmd, char *arg);
bool verificaArgumento(char *arg);
bool coordenadasValidas(ESTADO *e, int x, int y);
void confirmarSaida(ESTADO *e);
char encontrarUltimaMinuscula(ESTADO *e, int linha, int coluna);
void inicializarVisitados(ESTADO *e, int visitado[e->linhas][e->colunas], int *totalCelulasNaoRiscadas);
Coordenada encontrarCelulaInicial(ESTADO *e);
int realizarBusca(ESTADO *e, Coordenada inicio, int visitado[e->linhas][e->colunas]);
bool verificarMudancas(ESTADO *atual, ESTADO *anterior);
int stringParaInteiro (char *str);