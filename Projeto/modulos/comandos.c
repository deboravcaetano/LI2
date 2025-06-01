/**
* @file comandos.c
* @brief Este ficheiro contém o módulo 'comandos' do Projeto Final de LI2 (24/25).
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
 * @brief Grava o estado atual do jogo em um arquivo.
 * 
 * Esta função é responsável por salvar o estado completo do jogo, i.e, tabuleiro e histórico,
 * em um arquivo especificado.
 * 
 * @param cmd   Caractere que representa o comando (deve ser 'g' para gravar).
 * @param arg   Nome do arquivo onde será gravado.
 * @param e     Ponteiro para a estrutura de estado do jogo.
 * 
 * @return Retorna um código de erro:
 *         - E_OK se a gravação foi bem-sucedida.
 *         - E_CMD_INV se o comando for inválido, i.e, diferente de 'g'.
 *         - E_ARG se o argumento (nome do arquivo) estiver vazio.
 *         - E_FICHEIRO se não foi possível criar/abrir o arquivo.
 * 
 * @see ESTADO
 * @see gravarTabuleiro()
 * @see gravarHistorico()
 * @see argumentoVazio()
 */

int gravar(char cmd, char *arg, ESTADO *e) {
    // erro: comando inválido (o comando invocado, não chama esta função)
    if (cmd != 'g') return E_CMD_INV;

    // erro: o comando precisa de um argumento
    if (!argumentoVazio(cmd, arg)) return E_ARG;
    
    // abrir um ficheiro com permissão de escrita
    FILE *file = fopen(arg, "w");

    // erro: impossível abrir ficheiro
    if (file == NULL) {
        fprintf(stderr, NEGRITO AZUL "\n Erro:"RESET" Não foi possível abrir o ficheiro %s para gravar o estado completo.\n", arg);
        return E_FICHEIRO;
    }

    // escrever dimensões do tabuleiro no ficheiro
    fprintf(file, "%d %d\n", e -> linhas, e -> colunas);
    // escrever tabuleiro no ficheiro
    gravarTabuleiro(file, e);
    // escrever histórico no ficheiro
    gravarHistorico(file, e);

    // mensagem de sucesso
    printf("\n Jogo salvo com sucesso em %s!\n", arg);

    // fechar o ficheiro
    fclose(file);

    return E_OK;
}

/**
 * @brief Termina o jogo, ou seja, sai do programa.
 * 
 * @param cmd   Caractere que representa o comando (deve ser 's' para sair).
 * @param arg   Vazio.
 * @param e     Ponteiro para a estrutura de estado do jogo.
 * 
 * @return Retorna um código de erro:
 *         - E_OK se a saída foi bem-sucedida.
 *         - E_CMD_INV se o comando for inválido, i.e, diferente de 's'.
 *         - E_ARG se o argumento  não estiver vazio.
 * 
 * @see ESTADO
 * @see argumentoNaoVazio()
 * @see confirmarSaida()
 * @see libertarMemoria()
 * @see libertarHistorico()
 */

int sair(char cmd, char *arg, ESTADO *e) {
    // erro: comando inválido (o comando invocado, não chama esta função)
    if (cmd != 's') return E_CMD_INV;

    // erro: o comando não precisa de argumentos
    if (!argumentoNaoVazio(cmd, arg)) return E_ARG;

    // verificar se o utilizador deseja gravar o jogo
    confirmarSaida(e);

    // libertar a memória alocada para o tabuleiro
    libertarMemoria(e);
    // libertar a memória alocada para o histórico
    libertarHistorico(e);

    // sinalizar o término do jogo
    e -> looping = false;
    
    return E_OK;
}

/**
 * @brief Carrega um estado de jogo a partir de um ficheiro.
 * 
 * @param cmd   Caractere que representa o comando (deve ser 'l' para carregar).
 * @param arg   Caminho do ficheiro a ser carregado.
 * @param e     Ponteiro para a estrutura de estado do jogo.
 * 
 * @return Retorna um código de erro:
 *         - E_OK se o carregamento foi bem-sucedido.
 *         - E_CMD_INV se o comando for inválido (diferente de 'l')
 *         - E_ARG se o argumento estiver vazio
 *         - E_FICHEIRO se ocorrer erro ao abrir/ler o ficheiro
 * 
 * @details Esta função:
 *          1. Valida o comando e argumentos
 *          2. Grava o jogo atual se existir
 *          3. Liberta memória do estado atual
 *          4. Abre o ficheiro especificado
 *          5. Carrega tabuleiro e histórico
 *          6. Atualiza o caminho do ficheiro no estado
 *          7. Imprime o novo tabuleiro
 * 
 * @see ESTADO
 * @see JOGADA_SALVA
 * @see argumentoVazio()
 * @see gravar()
 * @see libertarMemoria()
 * @see libertarHistorico()
 * @see lerTabuleiroDoArquivo()
 * @see lerHistoricoJogadas()
 * @see imprimirTabuleiro()
 */

int ler(char cmd, char *arg, ESTADO *e) {
    // erro: comando inválido (o comando invocado, não chama esta função)
    if (cmd != 'l') return E_CMD_INV;

    // erro: o comando precisa de argumentos
    if (!argumentoVazio(cmd, arg)) return E_ARG;

    // salvar o estado de um tabuleiro antes de abrir um novo
    if (e -> tabuleiro != NULL) gravar('g', e -> caminhoFicheiro, e);

    // libertar a memória alocada para o tabuleiro
    libertarMemoria(e);
    // libertar a memória alocada para o histórico
    libertarHistorico(e);

    // abrir um ficheiro com permissão de leitura
    FILE *file = fopen(arg, "r");

    // erro: impossível abrir ficheiro
    if (file == NULL) {
        fprintf(stderr, NEGRITO AZUL "\n Erro:" RESET" Não foi possível abrir o ficheiro %s\n", arg);
        return E_FICHEIRO;
    }

    // erro: estrutura de ficheiro inválida
    if (!lerTabuleiroDoArquivo(file, e)) {
        fclose(file);
        return E_FICHEIRO;
    }

    // ler histórico do ficheiro
    JOGADA_SALVA *historico = lerHistoricoJogadas(file, e);

    // se o histórico estiver vazio, e tiver atingido o fim do ficheiro
    if (!historico && !feof(file)) { // feof() retorna EOF (End Of File) em caso de erro
        // mensagem de erro
        fprintf(stderr, "\n Erro ao ler histórico de jogadas!\n");
        
        // libertar a memória alocada para o histórico
        libertarHistorico(e);

        // fechar o ficheiro
        fclose(file);

        // erro: estrutura de ficheiro inválida
        return E_FICHEIRO;
    }

    // em caso de sucesso, o histórico do jogo é o histórico carregado
    e -> historico = historico;

    // copiar o caminho do ficheiro (argumento) para o estado
    e -> caminhoFicheiro = malloc(20);  
    if (!e -> caminhoFicheiro) {
        return E_MALLOC;
    }

    strncpy(e -> caminhoFicheiro, arg, 20 - 1);
    e -> caminhoFicheiro[20 - 1] = '\0';

    // fechar o ficheiro
    fclose(file);

    // imprimir tabuleiro carregado
    imprimirTabuleiro(e);

    return E_OK;
} 

/**
 * @brief Risca (#) uma célula específica no tabuleiro do jogo.
 * 
 * @param cmd   Caractere que representa o comando (deve ser 'r' para riscar)
 * @param arg   String com as coordenadas da célula a riscar 
 * @param e     Ponteiro para a estrutura de estado do jogo
 * 
 * @return Retorna um código de erro:
 *         - E_OK: Operação bem-sucedida
 *         - E_CMD_INV: Comando inválido (diferente de 'r')
 *         - E_ARG: Argumento ausente
 *         - E_TAB_NC: Tabuleiro não carregado
 *         - E_COORD: Coordenadas inválidas ou fora dos limites ou mal-formatadas.
 *         - E_MALLOC: Falha na alocação de memória
 * 
 * @details Esta função:
 *          1. Valida o comando e argumentos
 *          2. Verifica se o tabuleiro está carregado
 *          3. Converte e valida as coordenadas
 *          4. Registra a operação no histórico para undo
 *          5. Atualiza o tabuleiro marcando a célula como '#'
 *          6. Verifica se o tabuleiro foi resolvido
 * 
 * @see ESTADO
 * @see argumentoVazio()
 * @see tabuleiroCarregado()
 * @see verificaArgumento()
 * @see coordenadasValidas()
 * @see marcaInicioComando()
 * @see guardarEstado()
 * @see imprimirTabuleiro()
 * @see tabuleiroResolvido()
 */

int riscar(char cmd, char *arg, ESTADO *e) { 
    // erro: comando inválido (o comando invocado, não chama esta função)
    if (cmd != 'r') return E_CMD_INV;

    // erro: o comando precisa de argumentos
    if (!argumentoVazio(cmd, arg)) return E_ARG;

    // valida a existência de um tabuleiro
    if (!tabuleiroCarregado(e)) return E_TAB_NC;

    // valida o formato do argumento/coordenadas
    if (!verificaArgumento(arg)) return E_COORD;

    // converte as coordenadas em valores numéricos
    int coluna = toInt_Alpha(arg[0]); 
    int linha = stringParaInteiro(arg + 1) - 1;

    // valida a coerência do argumento/coordenadas
    if (!coordenadasValidas(e, coluna, linha)) return E_COORD;
    
    // regista no histórico o início do comando
    marcaInicioComando(e);

    // guarda o valor atual da coordenada
    char valor_atual = e -> tabuleiro[linha][coluna];
    
    // regista no histórico a alteração
    if (!guardarEstado(e, linha, coluna, valor_atual)) {
        fprintf(stderr, "\n Erro: Falha ao guardar estado anterior!\n");
        return E_MALLOC;
    }

    // risca a coordenada
    e -> tabuleiro[linha][coluna] = '#';

    // imprime o tabuleiro (com a coordenada riscada)
    imprimirTabuleiro(e);

    // verifica se o tabuleiro está resolvido
    if (tabuleiroResolvido(e)) printf(ROSA NEGRITO "\n PARABÉNS! CONSEGUISTE RESOLVER O PUZZLE.\n" RESET);

    return E_OK;
}

/**
 * @brief Converte uma célula específica em letra maiúscula no tabuleiro.
 * 
 * @param cmd   Caractere que representa o comando (deve ser 'b' para branco).
 * @param arg   String com as coordenadas da célula.
 * @param e     Ponteiro para a estrutura de estado do jogo.
 * 
 * @return Retorna um código de erro:
 *         - E_OK: Operação bem-sucedida.
 *         - E_CMD_INV: Comando inválido (diferente de 'b').
 *         - E_ARG: Argumento ausente. 
 *         - E_TAB_NC: Tabuleiro não carregado.
 *         - E_COORD: Coordenadas inválidas ou fora dos limites ou mal formatadas.
 *         - E_MALLOC: Falha na alocação de memória.
 * 
 * @details Esta função:
 *          1. Valida o comando e argumentos.
 *          2. Verifica se o tabuleiro está carregado.
 *          3. Processa e valida as coordenadas.
 *          4. Registra o estado anterior no histórico.
 *          5. Converte a célula para maiúscula (branco).
 *          6. Verifica se o puzzle foi resolvido.
 * 
 * @see ESTADO
 * @see argumentoVazio()
 * @see tabuleiroCarregado()
 * @see verificaArgumento()
 * @see coordenadasValidas()
 * @see marcaInicioComando()
 * @see guardarEstado()
 * @see imprimirTabuleiro()
 * @see tabuleiroResolvido()
 */

int branco (char cmd, char *arg, ESTADO *e) { 
    // erro: comando inválido (o comando invocado, não chama esta função)
    if (cmd != 'b') return E_CMD_INV;

    // erro: o comando precisa de argumentos
    if (!argumentoVazio(cmd, arg)) return E_ARG;

    // valida a existência de um tabuleiro
    if (!tabuleiroCarregado(e)) return E_TAB_NC;
    
    // valida o formato do argumento/coordenadas
    if (!verificaArgumento(arg)) return E_COORD;

    // converte as coordenadas em valores numéricos
    int coluna = toInt_Alpha(arg[0]); 
    int linha = stringParaInteiro(arg + 1) - 1;

    // valida a corência do argumento/coordenadas
    if(!coordenadasValidas(e, coluna, linha)) return E_COORD;

    // regista no histórico o início do comando
    marcaInicioComando(e);

    // guarda o valor atual da coordenada
    char valor_atual = e -> tabuleiro[linha][coluna];

    // regista no histórico a alteração
    if (!guardarEstado(e, linha, coluna, valor_atual)) {
        fprintf(stderr, AZUL NEGRITO "\n Erro:" RESET" Falha ao guardar estado anterior!\n");
        return E_MALLOC;
    }
    
    // converte a coordenada em branca
    char letra = e -> tabuleiro[linha][coluna];
    e -> tabuleiro[linha][coluna] = toUpper(letra);
    
    // imprime o tabuleiro (com a coordenada branca)
    imprimirTabuleiro(e);

    // verifica se o tabuleiro está resolvido
    if (tabuleiroResolvido(e)) printf(ROSA NEGRITO "\n PARABÉNS! CONSEGUISTE RESOLVER O PUZZLE.\n" RESET);

    return E_OK;
}

/**
 * @brief Desfaz a última jogada realizada no tabuleiro.
 * 
 * @param cmd           Caractere que representa o comando (deve ser 'd' para desfazer).
 * @param arg           Vazio.
 * @param estadoAtual   Ponteiro para a estrutura de estado do jogo.
 * 
 * @return Retorna um código de erro:
 *         - E_OK: Operação bem-sucedida
 *         - E_CMD_INV: Comando inválido (diferente de 'd')
 *         - E_ARG: Argumento não vazio
 *         - E_TAB_NC: Tabuleiro não carregado
 *         - E_HIST_VAZIO: Histórico vazio (nada para desfazer)
 * 
 * @details Esta função:
 *          1. Valida o comando e ausência de argumentos.
 *          2. Verifica se o tabuleiro está carregado.
 *          3. Verifica se existem operações no histórico para desfazer.
 *          4. Processa as jogadas para localizar a marca de início de comando.
 *          5. Remove o marcador do último comando.
 *          6. Atualiza o estado do jogo.
 * 
 * @see ESTADO
 * @see JOGADA_SALVA
 * @see argumentoNaoVazio()
 * @see tabuleiroCarregado()
 * @see verificarHistoricoVazio()
 * @see processarJogadas()
 * @see removerMarcadorComando()
 * @see imprimirTabuleiro()
 */

int desfazer(char cmd, char *arg, ESTADO *estadoAtual) {
    // erro: comando inválido (o comando invocado, não chama esta função)
    if (cmd != 'd') return E_CMD_INV;

    // erro: o comando não precisa de argumentos
    if (!argumentoNaoVazio(cmd, arg)) return E_ARG;

    // valida a existência de um tabuleiro
    if (!tabuleiroCarregado(estadoAtual)) return E_TAB_NC;

    // verifica se o histórico é vazio
    if (verificarHistoricoVazio(estadoAtual)) return E_HIST_VAZIO;

    // inicializa ponteiros para nodos do estado
    JOGADA_SALVA *anterior = NULL;
    JOGADA_SALVA *atual = processarJogadas(estadoAtual, &anterior);
    
    // remover comandos marcados
    removerMarcadorComando(estadoAtual, atual, anterior);


    // imprime o tabuleiro
    imprimirTabuleiro(estadoAtual);

    return E_OK;
}

/**
 * @brief Executa a verificação das regras do tabuleiro atual.
 * 
 * @param cmd   Caractere que representa o comando (deve ser 'v' para verificar).
 * @param arg   Vazio.
 * @param e     Ponteiro para a estrutura de estado do jogo.
 * 
 * @return Retorna um código de erro:
 *         - E_OK: Verificação concluída.
 *         - E_CMD_INV: Comando inválido (diferente de 'v').
 *         - E_ARG: Argumento não vazio.
 *         - E_TAB_NC: Tabuleiro não carregado.
 * 
 * @details Esta função:
 *          1. Valida o comando e ausência de argumentos.
 *          2. Verifica se o tabuleiro está carregado.
 *          3. Aciona o sistema de verificação de regras completo.
 * 
 * @see ESTADO
 * @see argumentoNaoVazio()
 * @see tabuleiroCarregado()
 * @see verificaRegras()
 */

int verificar(char cmd, char *arg, ESTADO *e) {
    // erro: comando inválido (o comando invocado, não chama esta função)
    if (cmd != 'v') return E_CMD_INV;

    // erro: o comando não precisa de argumentos
    if (!argumentoNaoVazio(cmd, arg)) return E_ARG;

    // valida a existência de um tabuleiro
    if (!tabuleiroCarregado(e)) return E_TAB_NC;

    // verifica se o tabuleiro cumpre todas as regras
    verificaRegras(e, 1);

    return E_OK;
}

/**
 * @brief Autocompleta células óbvias no tabuleiro usando as regras do jogo.
 * 
 * @param cmd   Caractere que representa o comando (deve ser 'a')
 * @param arg   Vazio
 * @param e     Ponteiro para a estrutura de estado do jogo
 * 
 * @return Retorna um código de erro:
 *         - E_OK: Autocompletamento realizado com sucesso.
 *         - E_CMD_INV: Comando inválido (diferente de 'a').
 *         - E_ARG: Argumento não vazio.
 *         - E_TAB_NC: Tabuleiro não carregado
 *         - E_TAB_INV: Tabuleiro contém violações de regras.
 *         - E_TAB_NM: Nenhuma modificação foi realizada.
 * 
 * @details Esta função:
 *          1. Valida pré-condições (comando, argumentos, tabuleiro carregado)
 *          2. Verifica se está válido
 *          3. Bloqueia execução se tabuleiro não estiver válido.
 *          4. Registra início da operação no histórico
 *          5. Aplica lógica de autocompletar.
 *          6. verifica se foi resolvido.
 * 
 * @see ESTADO
 * @see argumentoNaoVazio()
 * @see tabuleiroCarregado()
 * @see verificaRegras()
 * @see marcaInicioComando()
 * @see aplicaRegras()
 * @see imprimirTabuleiro()
 * @see tabuleiroResolvido()
 */

int autocompletar(char cmd, char *arg, ESTADO *e) {
    // erro: comando inválido (o comando invocado, não chama esta função)
    if (cmd != 'a') return E_CMD_INV;

    // erro: o comando não precisa de argumentos
    if (!argumentoNaoVazio(cmd, arg)) return E_ARG;

    // valida a existência de um tabuleiro
    if (!tabuleiroCarregado(e)) return E_TAB_NC;
 
    // verifica se o tabuleiro cumpre todas as regras
    verificaRegras(e, 0);

    // valida o tabuleiro
    if (!e -> tab_valid) {
        printf(VERMELHO NEGRITO "\n Tabuleiro inválido!" RESET  " Pressione v para descobrir as violações!\n");
        return E_TAB_INV; 
    }

    // regista no histórico o início do comando
    marcaInicioComando(e);

    // aplica uma iteração da lógica de regras
    if (!(aplicaRegras(e))) {
        desfazer('d',NULL,e);
        printf(NEGRITO "\n Impossível aplicar regras no estado atual!\n" RESET);
        return E_TAB_NM;
    }
    // imprime o tabuleiro
    imprimirTabuleiro(e);

    // verifica se o tabuleiro está resolvido
    if (tabuleiroResolvido(e)) printf(ROSA NEGRITO "\n PARABÉNS! CONSEGUISTE RESOLVER O PUZZLE.\n" RESET);
    
    return E_OK;
} 

/**
 * @brief Autocompleta o tabuleiro repetidamente até que não ocorram mais alterações
 * 
 * @param cmd   Caractere que representa o comando (deve ser 'A')
 * @param arg   Vazio.
 * @param e     Ponteiro para a estrutura de estado do jogo
 * 
 * @return Retorna um código de erro:
 *         - E_OK: Autocompletamento completo realizado.
 *         - E_CMD_INV: Comando inválido (diferente de 'A').
 *         - E_ARG: Argumento não vazio.
 *         - E_TAB_NC: Tabuleiro não carregado.
 *         - E_TAB_INV: Tabuleiro contém violações de regras.
 *         - E_MALLOC: Falha na alocação de memória.
 * 
 * @details Esta função:
 *          1. Valida pré-condições (comando, argumentos, tabuleiro carregado)
 *          2. Verifica se o tabuleiro é válido.
 *          3. Bloqueia execução se tabuleiro não estiver válido.
 *          4. Registra início da operação no histórico.
 *          5. Aplica a lógica as regras em loop:
 *             - Cria cópia do estado antes de ser alterado.
 *             - Aplica regras.
 *             - Compara com estado anterior
 *             - Repete enquanto houver mudanças
 *          6. Verifica se foi resolvido.
 * 
 * @see ESTADO
 * @see argumentoNaoVazio()
 * @see tabuleiroCarregado()
 * @see verificaRegras()
 * @see marcaInicioComando()
 * @see aplicaRegras()
 * @see criaCopiaEstado()
 * @see verificarMudancas()
 * @see libertarCopiaEstado()
 * @see imprimirTabuleiro()
 * @see tabuleiroResolvido()
 */

int autocompletarRec(char cmd, char *arg, ESTADO *e) {
    // erro: comando inválido (o comando invocado, não chama esta função)
    if (cmd != 'A') return E_CMD_INV;

    // erro: o comando não precisa de argumentos
    if (!argumentoNaoVazio(cmd, arg)) return E_ARG;

    // valida a existência de um tabuleiro
    if (!tabuleiroCarregado(e)) return E_TAB_NC;

    // verifica se o tabuleiro cumpre todas as regras
    verificaRegras(e, 0);

    // valida o tabuleiro
    if (!e -> tab_valid) {
        printf(VERMELHO NEGRITO "\n Tabuleiro inválido!" RESET  " Pressione v para descobrir as violações!\n");
        return E_TAB_INV; 
    }

    // regista no histórico o início do comando
    marcaInicioComando(e);

    // regista a ocorrência de mudanças por iteração
    bool houveMudanca = 1;

    ESTADO *copia = criaCopiaEstado(e);
        if (!copia) return E_MALLOC;

    // enquanto ocorrerem mudanças
    while (houveMudanca) {
        // registar a inexistência de mudanças na iteração
        houveMudanca = 0;
        
        // salva o estado antes de modificar
        ESTADO *comp = criaCopiaEstado(e);
        if (!comp) return E_MALLOC;

        // aplica uma iteração da lógica de regras
        aplicaRegras(e);

        // compara os estados novo com o antigo para saber se houve mudanças
        houveMudanca = verificarMudancas(e, comp);
        
        // liberta a cópia do estado
        libertarCopiaEstado(comp);
        free(comp);
    }

    if(!verificarMudancas(e, copia)){
        desfazer('d',NULL,e);
        printf(NEGRITO "\n Impossível aplicar regras no estado atual!\n"RESET);
        libertarCopiaEstado(copia);
        free(copia);
        return E_TAB_NM;
    }

    libertarCopiaEstado(copia);
    free(copia);

    // imprime o tabuleiro
    imprimirTabuleiro(e);

    // verifica se o tabuleiro está resolvido
    if (tabuleiroResolvido(e)) printf(ROSA NEGRITO "\n PARABÉNS! CONSEGUISTE RESOLVER O PUZZLE.\n" RESET);

    return E_OK;
}

/**
 * @brief Resolve o tabuleiro.
 * 
 * @param cmd   Caractere que representa o comando (deve ser 'R').
 * @param arg   Vazio.
 * @param e     Ponteiro para a estrutura de estado do jogo.
 * 
 * @return Retorna um código de erro:
 *         - E_OK: Operação concluída.
 *         - E_CMD_INV: Comando inválido.
 *         - E_ARG: Argumento não vazio.
 *         - E_TAB_NC: Tabuleiro não carregado.
 *         - E_TAB_INV: Tabuleiro inicial contém violações.
 *         - E_MALLOC: Falha na alocação de memória.
 * 
 * @details Esta função:
 *          1. Valida pré-condições (comando, argumentos, tabuleiro carregado)
 *          2. Verifica se é válido.
 *          3. Cria cópia para futura restauração.
 *          4. Registra início da operação no histórico
 *          5. Chama o a função que aplica toda a lógica.
 *          6. Gerencia resultados (sucesso/fracasso)
 *          7. Restaura estado original se não encontrar solução
 * 
 * @see ESTADO
 * @see argumentoNaoVazio()
 * @see tabuleiroCarregado()
 * @see verificaRegras()
 * @see marcaInicioComando()
 * @see resolverPuzzle()
 * @see criaCopiaEstado()
 * @see restaurarEstado()
 * @see libertarMemoria()
 */

int resolver(char cmd, char *arg, ESTADO *e) {
    // erro: comando inválido (o comando invocado, não chama esta função)
    if (cmd != 'R') return E_CMD_INV;

    // erro: o comando não precisa de argumentos
    if (!argumentoNaoVazio(cmd, arg)) return E_ARG;

    // valida a existência de um tabuleiro
    if (!tabuleiroCarregado(e)) return E_TAB_NC;

    // verifica se o tabuleiro cumpre todas as regras
    verificaRegras(e, 0);  

    // valida o tabuleiro
    if (!e -> tab_valid) {
        printf(VERMELHO NEGRITO "\n Tabuleiro inválido!" RESET  " Pressione v para descobrir as violações!\n");
        return E_TAB_INV;  
    }

    // copia o estado para modificar
    ESTADO *original = criaCopiaEstado(e);
    if (original == NULL) {
        fprintf(stderr, " Erro: Falha ao criar cópia do tabuleiro\n");
        return E_MALLOC;
    }

    // regista no histórico o início do comando
    marcaInicioComando(e);

    // tenta resolver o puzzle e regista o resultado
    int resultado = resolverPuzzle(e, original);

    // se tiver sido resolvido
    if (resultado) {
        // imprime o tabuleiro
        imprimirTabuleiro(e);
        // mensagem de sucesso
        printf(NEGRITO AZUL "\n TABULEIRO RESOLVIDO COM SUCESSO.\n" RESET);
    }
    else {       
        // mensagem de insucesso
        printf(NEGRITO  VERMELHO "\n IMPOSSÍVEL RESOLVER TABULEIRO!\n" RESET);
        // restaurar o estado
        restaurarEstado(e, original);
        // desfaz a tentativa de resolver
        desfazer('d',NULL,e);
    }

    // libertar a memória da cópia
    libertarMemoria(original);
    free(original);

    return E_OK;
}

/**
 * @brief Limpa a tela do terminal.
 * 
 * @param cmd   Caractere que representa o comando (deve ser 'L').
 * @param arg   Vazio.
 * @param e     Ponteiro para a estrutura de estado do jogo.
 * 
 * @return Retorna um código de erro:
 *         - E_OK: Operação bem-sucedida.
 *         - E_CMD_INV: Comando inválido (diferente de 'L').
 *         - E_ARG: Argumento não vazio.
 * 
 * @see imprimirTabuleiro()
 * @see clear()
 */

int limpa (char cmd, char *arg, ESTADO *e) {
    (void) e; // para evitar warning Unused Parameter

    // erro: comando inválido (o comando invocado, não chama esta função)
    if (cmd != 'L') return E_CMD_INV;

    // erro: o comando não precisa de argumentos
    if(!argumentoNaoVazio(cmd, arg)) return E_ARG;   

    // executa o comando 'clear' no terminal
    clear(); 

    // imprime o tabuleiro
    imprimirTabuleiro(e);
    
    return E_OK;
}

/**
 * @brief Reverte uma célula específica para sua versão minúscula.
 * 
 * @param cmd   Caractere que representa o comando (deve ser 'm').
 * @param arg   String com as coordenadas da célula.
 * @param e     Ponteiro para a estrutura de estado do jogo,
 * 
 * @return Retorna um código de erro:
 *         - E_OK: Operação bem-sucedida.
 *         - E_CMD_INV: Comando inválido (diferente de 'm').
 *         - E_TAB_NC: Tabuleiro não carregado.
 *         - E_ARG: Argumento ausente ou mal formatado.
 *         - E_COORD: Coordenadas inválidas, fora dos limites, ou célula já minúscula.
 *         - E_MALLOC: Falha na alocação de memória.
 * 
 * @details Esta função:
 *          1. Valida o comando e argumentos.
 *          2. Verifica se o tabuleiro está carregado.
 *          3. Converte e valida as coordenadas.
 *          4. Verifica se a célula já está minúscula.
 *          5. Registra início do comando no histórico.
 *          6. Procura no histórico a última versão minúscula da célula.
 *          7. Guarda estado atual para permitir undo.
 *          8. Atualiza a célula para a versão minúscula encontrada.
 * 
 * @see ESTADO
 * @see argumentoVazio()
 * @see tabuleiroCarregado()
 * @see verificaArgumento()
 * @see coordenadasValidas()
 * @see marcaInicioComando()
 * @see encontrarUltimaMinuscula()
 * @see guardarEstado()
 * @see imprimirTabuleiro()
 */

int desfazerCoordenada(char cmd, char *arg, ESTADO *e) {
    // erro: comando inválido (o comando invocado, não chama esta função)
    if (cmd != 'm') return E_CMD_INV;

    // valida a existência de um tabuleiro
    if (!tabuleiroCarregado(e)) return E_TAB_NC;

    // erro: o comando precisa de argumentos
    if (!argumentoVazio(cmd, arg)) return E_ARG;

    // valida o formato do argumento/coordenadas
    if (!verificaArgumento(arg)) return E_COORD;

    // converte as coordenadas em valores numéricos
    int coluna = toInt_Alpha(arg[0]); 
    int linha = stringParaInteiro(arg + 1) - 1;

    // valida a coerência do argumento/coordenadas
    if (!coordenadasValidas(e, coluna, linha)) return E_COORD;

    // verifica se a célula é minúscula
    if (isLower(e -> tabuleiro[linha][coluna])) {
        printf(NEGRITO AZUL "\n Erro:" RESET " Célula já é minúscula!\n");
        return E_COORD;
    }

    // regista no histórico o início do comando
    marcaInicioComando(e);

    // procura no histórico a última minúscula nas coordenadas dadas
    char minuscula = encontrarUltimaMinuscula(e, linha, coluna);

    // guarda o estado atual antes de modificar
    if (!guardarEstado(e, linha, coluna, e -> tabuleiro[linha][coluna])) {
        fprintf(stderr, "\n Erro: Falha ao guardar estado anterior!\n");
        return E_MALLOC;
    }

    // atualiza o tabuleiro com a última minúscula nas coordenadas dadas 
    e -> tabuleiro[linha][coluna] = minuscula;
    
    // imprime o tabuleiro (com a coordenada reposta)
    imprimirTabuleiro(e);

    // mensagem de sucesso
    printf("\n Célula %c%d voltou à sua minúscula!\n", arg[0], linha + 1);

    return E_OK;
}

/**
 * @brief Exibe as regras do jogo e um dicionário de termos.
 * 
 * @param cmd   Caractere que representa o comando (deve ser 'w').
 * @param arg   Vazio.
 * @param e     Ponteiro para a estrutura de estado do jogo (não utilizado)
 * 
 * @return Retorna um código de erro:
 *         - E_OK: Exibição bem-sucedida.
 *         - E_CMD_INV: Comando inválido (diferente de 'w').
 *         - E_ARG: Argumento não vazio.
 */

int regras(char cmd, char *arg, ESTADO *e) {
    // erro: comando inválido (o comando invocado, não chama esta função)
    if(cmd != 'w') return E_CMD_INV;

    // erro: o comando não precisa de argumentos
    if(!argumentoNaoVazio(cmd, arg)) return E_ARG;

    (void) e; // para evitar warning Unused Parameter
    
    printf("\n");
    printf(" ======================================================================================================================================\n");
    printf(" | --------------------------" NEGRITO " Regras " RESET "----------------------------- || -------------------------" NEGRITO " Dicionário "RESET "---------------------------|\n");
    printf(" |                                                                 ||                                                                 |\n");
    printf(" | 1) Em cada linha/coluna só pode existir "ITALICO "uma única" RESET " réplica de    ||  "ROSA_CLARO"Branco" RESET " - maiúscula                                             |\n");
    printf(" | cada símbolo que é pintada a branco.                            ||                                                                 |\n");
    printf(" |                                                                 ||  "ROSA_CLARO"Riscada" RESET " - #                                                    |\n");
    printf(" | 2) "ITALICO "Todas" RESET " as outras réplicas desse símbolo têm que ser riscadas. ||                                                                 |\n");
    printf(" |                                                                 ||  "ROSA_CLARO"Caminho ortogonal" RESET " - sequência de movimentos horizontais ou     |\n");
    printf(" | 3) "ITALICO "Todas" RESET " as casas vizinhas ortogonais de uma riscada têm que    ||  verticais.                                                     |\n");
    printf(" | estar pintadas a branco.                                        ||                                                                 |\n");
    printf(" |                                                                 || ---------------------------" NEGRITO " Ganhar "RESET "---------------------------- |\n");
    printf(" | 4) É " ITALICO"necessário"RESET" existir um caminho ortogonal entre todas as     ||                                                                 |\n");
    printf(" | casas brancas do tabuleiro.                                     ||  Ganhas quando já não existirem letras minúsculas e o puzzle    |\n");
    printf(" |                                                                 ||  for válido.                                                    |\n");
    printf(" |                                                                 ||                                                                 |\n");
    printf(" ======================================================================================================================================\n");

    return E_OK;
}

/**
 * @brief Exibe os comandos possíveis
 * 
 * @param cmd   Caractere que representa o comando (deve ser 'h').
 * @param arg   Vazio.
 * @param e     Ponteiro para a estrutura de estado do jogo (não utilizado)
 * 
 * @return Retorna um código de erro:
 *         - E_OK: Exibição bem-sucedida.
 *         - E_CMD_INV: Comando inválido (diferente de 'h').
 *         - E_ARG: Argumento não vazio.
 */

int help(char cmd, char *arg, ESTADO *e) {
    (void) e; // para evitar warning Unused Parameter

    // erro: comando inválido (o comando invocado, não chama esta função)
    if (cmd != 'h') return E_CMD_INV;

    // erro: o comando não precisa de argumentos
    if(!argumentoNaoVazio(cmd, arg)) return E_ARG;

    printf(NEGRITO ROSA_CLARO "\n l nivel/{1-6}" RESET ": Seleciona o nível para jogar.\n");
    printf(NEGRITO ROSA_CLARO " g {caminho}/ficheiro" RESET ": Salva o jogo.\n");
    printf( NEGRITO ROSA_CLARO "\n b coordenadas" RESET ": Pinta a letra.\n");
    printf(NEGRITO  ROSA_CLARO " r coordenadas" RESET ": Risca a letra.");
    printf(NEGRITO ROSA_CLARO "\n m coordenadas" RESET ": Volta à minúscula.\n");
    printf( NEGRITO ROSA_CLARO  "   coordenadas" RESET ": <letra minúscula><número> (letra: coluna, número: linha).\n");
    printf(NEGRITO ROSA_CLARO "\n d" RESET ": Desfaz a última jogada.\n");
    printf(NEGRITO ROSA_CLARO " v" RESET ": Verifica as restrições.\n");
    printf(NEGRITO ROSA_CLARO " s" RESET ": Termina o jogo.\n");
    printf( NEGRITO ROSA_CLARO " w" RESET ": Regras do jogo.\n");
    printf(NEGRITO ROSA_CLARO " a" RESET ": Aplica as regras em uma iteração.\n");
    printf(NEGRITO ROSA_CLARO " A" RESET ": Aplica as regras enquanto for possível alterar o puzzle.\n");
    printf( NEGRITO ROSA_CLARO " R" RESET ": Resolve o puzzle.\n");
    printf( NEGRITO ROSA_CLARO " L" RESET ": Limpa a tela.\n");
    
    return E_OK;
}