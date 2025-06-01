/**
* @file erros.h
* @brief Este ficheiro contém o header 'erros' do Projeto Final de LI2 (24/25).
* 
* Códigos de erro para melhorar a separação do tratamento de erros.
* 
* Projeto Final - Puzzle Game
* Laboratórios de Informática II (UMinho 24/25) 
*
* (c) Débora Luana Pinto Caetano    <a112332@alunos.uminho.pt>, 2025-06-01
* (c) Marcos Bernardo da Silva Lobo <a110959@alunos.uminho.pt>, 2025-06-01
* (c) Maria Inês da Cruz Machado    <a111464@alunos.uminho.pt>, 2025-06-01
*/

/**
 * @def E_OK
 * @brief Operação concluída com sucesso, tudo funcionou conforme esperado
 * 
 * Valor: 0
 * 
 */
#define E_OK 0

/**
 * @def E_CMD_INV
 * @brief Comando inválido ou desconhecido
 * 
 * Valor: 1
 * 
 * @details Ocorre quando:
 *          - O comando digitado não existe
 *          - Função incorreta chamada para processar o comando
 */
#define E_CMD_INV 1

/**
 * @def E_HIST_VAZIO
 * @brief Histórico de jogadas vazio
 * 
 * Valor: 2
 * 
 * @details Tentativa de desfazer sem jogadas no histórico
 */
#define E_HIST_VAZIO 2

/**
 * @def E_TAB_INV
 * @brief Tabuleiro inválido
 * 
 * Valor: 3
 * 
 * @details O tabuleiro atual viola uma ou mais regras do jogo:
 *          - Símbolos letras brancas duplicadas em linha/coluna
 *          - Células riscadas isoladas
 *          - Conectividade quebrada
 */
#define E_TAB_INV 3

/**
 * @def E_COORD
 * @brief Problema com coordenadas
 * 
 * Valor: 4
 * 
 */
#define E_COORD 4

/**
 * @def E_FICHEIRO
 * @brief Erro relacionado a operações com arquivos
 * 
 * Valor: 5
 * 
 * @details Inclui:
 *          - Falha ao abrir/criar arquivo
 *          - Formato de arquivo inválido
 *          - Falha de leitura/escrita
 */
#define E_FICHEIRO 5

/**
 * @def E_ARG
 * @brief Problema com argumentos de comando
 * 
 * Valor: 6
 * 
 * @details Ocorre quando:
 *          - Argumento ausente quando obrigatório
 *          - Argumento fornecido quando não permitido
 *          - Tipo de argumento incorreto
 */
#define E_ARG 6
/**
 * @def E_TAB_NM
 * @brief Nenhuma modificação no tabuleiro
 * 
 * Valor: 7
 * 
 * @details Operação não alterou o estado do tabuleiro
 */
#define E_TAB_NM 7

/**
 * @def E_MALLOC
 * @brief Falha na alocação de memória
 * 
 * Valor: 8
 * 
 * @details Memória insuficiente para operação
 *          ou erro no sistema de alocação
 */
#define E_MALLOC 8

/**
 * @def E_TAB_NC
 * @brief Tabuleiro não carregado
 * 
 * Valor: 9
 * 
 * @details Tentativa de operação que requer tabuleiro,
 *          mas nenhum tabuleiro está carregado
 */
#define E_TAB_NC 9


/**
 * @def E_FORMATO_ARG
 * @brief Formato de argumento inválido
 * 
 * Valor: 10
 * 
 * @details Sintaxe incorreta nos argumentos:
 *          - Múltiplos argumentos quando só um é esperado
 *          - Separadores incorretos
 */
#define E_FORMATO_ARG 10

/**
 * @def E_EOF
 * @brief Fim de entrada
 * 
 * Valor: 11
 * 
 * @details Leitura interrompida por fim
 *          (ex: Ctrl+D no terminal)
 */
#define E_EOF 11

/**
 * @def E_VAZIO
 * @brief Entrada vazia
 * 
 * Valor: 12
 * 
 * @details O utilizador submeteu linha vazia ou
 *          apenas espaços em branco
 */
#define E_VAZIO 12


