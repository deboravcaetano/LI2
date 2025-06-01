/**
* @file auxiliares.c
* @brief Este ficheiro contém o módulo 'auxiliares' do Projeto Final de LI2 (24/25).
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
 * @brief Aplica um conjunto de regras lógicas sobre o tabuleiro.
 * 
 * @param e Ponteiro para a estrutura de estado do jogo.
 * 
 * @return true se pelo menos uma modificação foi feita no tabuleiro.
 * @return false se nenhuma alteração foi realizada.
 * 
 * @details Esta função implementa três regras fundamentais de inferência:
 *          1. `riscarLetrasIguaisBrancas()`:
 *             - Se uma letra está em branco (maiúscula), todas outras instâncias
 *               da mesma letra na mesma linha/coluna devem ser riscadas.
 * 
 *          2. `pintarVizinhasDeRiscadas()`:
 *             - Toda a célula riscada (#) deve ter suas 4 vizinhas ortogonais
 *               convertidas para branco (maiúsculas).
 * 
 *          3. `pintarCasasQueIsolamLetras()`:
 *             - Se riscar uma célula criaria áreas isoladas de letras,
 *               converte essa célula para branco em vez de riscar.
 * 
 * @see riscarLetrasIguaisBrancas()
 * @see pintarVizinhasDeRiscadas()
 * @see pintarCasasQueIsolamLetras()
 */

bool aplicaRegras(ESTADO *e) {
    // regista mudanças do tabuleiro
    bool mudou = false;

    // pinta de branco todas as casas vizinhas de uma casa riscada
    if (pintarVizinhasDeRiscadas(e))   mudou = true;

    // pinta de branco uma casa quando seria impossível que esta fosse riscada por isolar letras 
    if (pintarCasasQueIsolamLetras(e)) mudou = true;

    // risca todas as letras iguais a uma letra branca na mesma linha e/ou coluna
    if (riscarLetrasIguaisBrancas(e))  mudou = true;
 
    return mudou;
}

/**
 * @brief Marca o inicio de comando para desfazer comandos recursivos numa iteração
 * 
 * @param e Ponteiro para a estrutura de estado do jogo
 * 
 * @see JOGADA_SALVA
 * @see desfazer
 * @see autocompletar
 * @see resolver
 */

void marcaInicioComando(ESTADO *e) {
    // criar elemento da stack/histórico
    JOGADA_SALVA *marca = malloc(sizeof(JOGADA_SALVA));

    // erro: falha na alocação, sair da função 
    if (!marca) return; 

    // inicialização das variáveis da stack
    marca -> x = -1; 
    marca -> y = -1;
    marca -> c = '*';
    marca -> inicioComando = true; 

    // adicionar elemento com a marcado à stack/histórico
    marca -> ant = e -> historico;
    e -> historico = marca;
}

/**
 * @brief Implementa a lógica de resolver o puzzle.
 * 
 * @param e         Ponteiro para o estado atual do jogo (modificado durante a resolução).
 * @param original  Ponteiro para cópia do estado original (usado para restauração).
 * 
 * @return true se o puzzle foi resolvido com sucesso.
 * @return false se o puzzle for insolúvel a partir do estado atual.
 * 
 * @details Esta função implementa um algoritmo :
 *          1. 
 *             - Aplica repetidamente regras 
 *             - Verifica validade após cada alteração
 *             - Aborta se violar regras básicas
 * 
 *          2. 
 *             - Seleciona a primeira célula minúscula 
 *             - Testa hipótese 1: converter para branco (maiúscula)
 *             - Se falhar, testa hipótese 2: riscar ('#')
 *             - Recursão 
 * 
 *          3. Mecanismo de retrocesso:
 *             - Restaura estado original após cada ramo mal-sucedido
 *             - Interrompe busca quando encontra primeira solução
 * 
 * @see aplicaRegras()
 * @see verificaRegras()
 * @see tabuleiroResolvido()
 * @see restaurarEstado()
 */

bool resolverPuzzle(ESTADO *e, ESTADO *original) {
    // aplica as regras do jogo até deixar de haver mudanças no tabuleiro
    while (aplicaRegras(e)) {
        // para cada altereção verifica se o tabuleiro resultante cumpre todas as regras do jogo
        verificaRegras(e, 0);

        // se o tabuleiro não for válido, restaura o tabuleiro e retorna 'false', ou seja, não é possível resolver
        if (!e -> tab_valid) {
            restaurarEstado(e, original);
            return false;
        }
    }

    // verifica se o tabuleiro está resolvido
    if (tabuleiroResolvido(e)) return true;

    // procura as minúsculas restantes no tabuleiro
    for (int i = 0; i < e -> linhas; i++) {
        for (int j = 0; j < e -> colunas; j++) {
            if (isLower(e -> tabuleiro[i][j])) {
                // tenta resolver, colocando a coordenada como branca
                e -> tabuleiro[i][j] = toUpper(e -> tabuleiro[i][j]); // coordenada branca
                if (resolverPuzzle(e, original)) return true; // se for resolúvel, retorna true
                restaurarEstado(e, original); // se não for resolúvel, volta ao estado original

                // tenta resolver, colocando a coordenada como riscada ('#')
                e -> tabuleiro[i][j] = '#'; // coordenada riscada
                if (resolverPuzzle(e, original)) return true; // se for resolúvel, retorna true
                restaurarEstado(e, original); // se não for resolúvel, volta ao estado original

                // tabuleiro irresolúvel
                return false;  
            }
        }
    }

    // tabuleiro resolvido
    return true;  
}

/**
 * @brief Processa entrada do utilizador.
 * 
 * @param cmd Ponteiro para armazenar o caractere do comando.
 * @param arg Ponteiro para armazenar a string do argumento.
 * 
 * @return Código de status:
 *         - E_OK: Entrada válida processada.
 *         - E_EOF: Fim de entrada
 *         - E_VAZIO: Linha vazia ou apenas espaços.
 *         - E_FORMATO_ARG: Formato de comando inválido.
 * 
 * 
 * Regras de formatação:
 *          - Comandos sem argumentos: "c"
 *          - Comandos com argumentos: "c argumento"
 *          - Não são permitidos:
 *             * ra1
 *             * r a1 2 
 */

int lerLinha(char *cmd, char *arg) {
    // inicializar o array 'linha'
    char linha[LINE_SIZE] = {0};

    // imprimir a marca de input
    printf("\n> ");

    // verificar o fim do input
    if (fgets(linha, LINE_SIZE, stdin) == NULL) return E_EOF;

    // substituir caracter '\n' (newline) por '\0' para marcar o fim da linha
    linha[strcspn(linha, "\n")] = '\0'; 

    // entrada = " "
    if (strspn(linha, " ") == strlen(linha)) return E_VAZIO;
    // entrada = "\0"
    if (strlen(linha) == 0) return E_VAZIO;

    // verifica a coerência do input (valida o espaçamento)
    if (strlen(linha) >= 2 && linha[1] != ' ') {
        fprintf(stderr, NEGRITO AZUL "\n Erro:"RESET" Comando mal formatado. Use '<comando>' ou '<comando> <argumento>'.\n");
        return E_FORMATO_ARG;
    }

    // guarda o primeiro argumento
    char arg1[LINE_SIZE] = {0};
    // guarda o segundo argumento durante a validação de excesso de argumentos
    char arg2[LINE_SIZE] = {0};

    int numArgs = sscanf(linha, " %c %s %s", cmd, arg1, arg2);

    // sem argumentos
    if (numArgs == 0) return E_VAZIO;
    // apenas o comando (um argumento)
    else if (numArgs == 1) {
        arg[0] = '\0'; 
        return E_OK;
    }
    // comando e argumento (dois argumentos)
    else if (numArgs == 2) {
        strcpy(arg, arg1); 
        return E_OK;
    }
    // erro: argumentos a mais    
    else {
        fprintf(stderr, NEGRITO AZUL "\nErro:" RESET " Muitos argumentos. Use '<comando>' ou '<comando> <argumento>'.\n");
        return E_FORMATO_ARG;
    }
}

/**
 * @brief Orquestra a execução de um comando usando uma lista comandos possíveis.
 * 
 * @param comandos Array de ponteiros para funções de comando.
 * @param cmd      Caractere do comando a ser executado.
 * @param arg      Argumento do comando (string).
 * @param estado   Ponteiro para a estrutura de estado do jogo.
 * 
 * 
 * @details Esta função:
 *          1. Itera sobre um array de comandos.
 *          2. Para cada:
 *             - Chama com argumento se fornecido.
 *             - Chama com NULL se argumento estiver vazio.
 * 
 * @see COMANDO
 */

int executarComando(COMANDO comandos[], char cmd, char *arg, ESTADO *estado) {
    // iteração sobre o array de comandos de jogo
    for (int i = 0; comandos[i] != NULL; i++) {
        int resultado;

        // se o array de argumentos não estiver vazio, executa com 'arg'
        if (arg[0] != '\0') resultado = comandos[i](cmd, arg, estado);
        // senão executa com NULL
        else resultado = comandos[i](cmd, NULL, estado);

        // se ocorrer algum erro, que não 'comando inválido', retorna o erro
        // NOTA: o erro 'comando inválido' apenas diz que o comando em execução não é compatível com o introduzido
        if (resultado != E_CMD_INV) return resultado; 
    }
    
    // se todos os comandos retornarem o erro 'comando inválido', o comando introduzido não existe
    return E_CMD_INV;
}

/**
 * @brief Valida a ausência de argumentos em comandos que precisam deles 
 */

bool argumentoVazio(char cmd, char *arg) {
    if (arg == NULL) {
        fprintf(stderr, AZUL NEGRITO "\n Erro:" RESET " O comando '%c' precisa de um argumento!\n", cmd);
        return false;
    }

    return true;
}

/**
 * @brief Valida a existência de argumentos em comandos que não precisam deles 
 */

bool argumentoNaoVazio(char cmd, char *arg) {
    if (arg != NULL) {
        printf(NEGRITO AZUL "\n ERRO:" RESET " Comando '%c' não precisa de um argumento!\n", cmd);
        return false;
    }

    return true;
}

/**
 * @brief Verifica se as coordenadas são dadas no formato correto
 * 
 * Exemplo: r a1
 * 
 * Não aceita:
 *      1. ra1
 *      2. r a1 2
 */

bool verificaArgumento(char *arg) {
    if (strlen(arg) < 2 || !isLower(arg[0])) {
        fprintf(stderr, NEGRITO AZUL "\n Erro:" RESET " Coordenadas inválidas.\n Use o formato " NEGRITO "<minúscula><número>" RESET " (ex: a1, b3)!\n");
        return false;
    }

    for (int i = 1; arg[i] != '\0'; i++) {
        if (!isDigit(arg[i])) {
            fprintf(stderr, NEGRITO AZUL "\n Erro:" RESET " Coordenadas inválidas. Use o formato " NEGRITO "<minúscula><número>" RESET " (ex: a1, b3)!\n");
            return false;
        }
    }

    return true;
}

/**
 * @brief Verifica se as coordendas estão dentro dos limites do tabuleiro.
 */

bool coordenadasValidas(ESTADO *e, int coluna, int linha) {
    // uma coordenada válida cumpre o seguinte: 0 <= coluna < e -> colunas, 0 <= linha < e -> linhas

    // se algum dos requisitos não se cumprir, retorna falso e imprime mensagem de erro
    if (linha < 0 || linha >= e -> linhas || coluna < 0 || coluna >= e -> colunas) {
        fprintf(stderr, AZUL NEGRITO "\n Erro:" RESET" Coordenadas fora dos limites do tabuleiro: (%dx%d)!\n", e -> linhas, e -> colunas);
        return false;
    }

    return true;
}

/**
 * @brief Verifica se o utilizador deseja gravar o estado atual do jogo antes de sair.
 * 
 * @see confirmarSaida()
 */


void confirmarSaida(ESTADO *e) {
    char resposta[5]; 
    
    printf(NEGRITO "\n GRAVAR ANTES DE SAIR [S][N]? " RESET);
    
    // faz a leitura e verifica se foi bem sucedida
    if (fgets(resposta, sizeof(resposta), stdin) != NULL) {
        // encontra o índice da primeira ocorrência de '\n' e coloca um '\0' (marca o fim da resposta)
        resposta[strcspn(resposta, "\n")] = '\0';  

        // verifica se a resposta é uma variação de 'Sim'
        if (strcmp(resposta, "s") == 0 || strcmp(resposta, "sim") == 0 || strcmp(resposta, "S") == 0 || strcmp(resposta, "Sim") == 0) {
            if (e -> caminhoFicheiro[0] != '\0') gravar('g', e -> caminhoFicheiro, e); 
            else printf(NEGRITO LARANJA "\n Aviso:" RESET " Nenhum ficheiro carregado para gravar!\n");
            return;
        }

        // verifica se a resposta é uma variação de 'Não'
        else if (strcmp(resposta, "n") == 0 || strcmp(resposta, "nao") == 0 || strcmp(resposta, "não") == 0 ||
                 strcmp(resposta, "N") == 0 || strcmp(resposta, "Nao") == 0 || strcmp(resposta, "Não") == 0) {
            return;
        }

        // a resposta é inválida
        else {
            printf(VERMELHO NEGRITO "\n Erro:" RESET " Resposta inválida! Use [S] ou [N].\n");
            // chamada recursiva até a resposta ser válida
            confirmarSaida(e);  
        } 
    }

    // erro: falha na leitura da resposta
    else printf("\n Erro ao ler a resposta. A sair sem gravar.\n");
}

/**
 * @brief Encontra a minúscula associada a uma coordenada que esteja branca ou riscada.
 */

char encontrarUltimaMinuscula(ESTADO *e, int linha, int coluna) {
    JOGADA_SALVA *atual = e -> historico;
    char minuscula = '\0';
    int encontrou = 0;
    
    // itera enquanto houver histórico e não tiver encontrado a minúscula
    while (atual != NULL && !encontrou) {
        // as coordenadas no histórico são iguais às dos argumentos e a letra é minúscula
        if (atual -> x == linha && atual -> y == coluna && isLower(atual -> c)) {
            minuscula = atual -> c;
            encontrou = 1;
        }

        atual = atual->ant;
    }

    return minuscula;
}

/**
 * @brief Inicializa a matriz de células visitadas e conta o número de células não riscadas.
 */

void inicializarVisitados(ESTADO *e, int visitado[e -> linhas][e -> colunas], int *totalCelulasNaoRiscadas) {
    *totalCelulasNaoRiscadas = 0;
    for (int i = 0; i < e -> linhas; i++) {
        for (int j = 0; j < e -> colunas; j++) {
            visitado[i][j] = 0;
            if (e -> tabuleiro[i][j] != '#') (*totalCelulasNaoRiscadas)++;
        }
    }
}

/**
 * @brief Seleciona as coordenadas da primeira célula não riscada (procura de um caminho ortogonal)
 */

Coordenada encontrarCelulaInicial(ESTADO *e) {
    // inicialização com coordenada inválida (os índices começam em 0)
    Coordenada inicio = {-1, -1};

    // após encontrar uma coordenada que não esteja riscada ('#') os ciclos param de iterar
    for (int i = 0; i < e -> linhas && inicio.x == -1; i++) {
        for (int j = 0; j < e -> colunas && inicio.y == -1; j++) {
            if (e -> tabuleiro[i][j] != '#') {
                inicio.x = i;
                inicio.y = j;
            }
        }
    }

    return inicio;
}

/**
 * @brief Lógica do algoritmo de procura de um caminho ortogonal (Breadth-First Search Algorithm)
 * 
 * @see criarQueue()
 * @see dequeue()
 * @see enqueue()
 */

int realizarBusca(ESTADO *e, Coordenada inicio, int visitado[e -> linhas][e -> colunas]) {
    // inicializar a queue com as coordenadas 'inicio'
    Queue q = criarQueue(e -> linhas * e -> colunas);
    enqueue(q, inicio);
 
    // marcar a coordenada inicial como visitada
    visitado[inicio.x][inicio.y] = 1;
    int celulasAlcancadas = 1;
    
    // inicializar os arrays com as direções ortogonais
    // NOTA: os movimentos ortogonais variam num só eixo: (-1, 0) (1, 0) (0, -1) (0, 1)
    int dx[] = {-1, 1, 0, 0};  
    int dy[] = {0, 0, -1, 1};

    // enquanto a queue tiver elementos
    while (!isEmpty(q)) {
        // remover o topo da queue
        Coordenada atual = dequeue(q);
        
        for (int i = 0; i < 4; i++) {
            // variação das coordenadas, de acordo com os arrays das direções ortogonais
            int cx = atual.x + dx[i];
            int cy = atual.y + dy[i];
            
            if (cx >= 0 && cx < e -> linhas && cy >= 0 && cy < e -> colunas && !visitado[cx][cy] && e -> tabuleiro[cx][cy] != '#') {
                // marcar a coordenada como visitada
                visitado[cx][cy] = 1;
                celulasAlcancadas++;

                Coordenada vizinha = {cx, cy};
                //colocar na queue a vizinha ortogonal (variação da coordenada)
                enqueue(q, vizinha);
            }
        }
    }
    
    // libertar a queue vazia
    freeQueue(q);
    
    return celulasAlcancadas;
}

/**
 * @brief Verifica se existem mudanças entre dois estados do puzzle
 */

bool verificarMudancas(ESTADO *atual, ESTADO *anterior) {
    for (int i = 0; i < atual -> linhas; i++) {
        for (int j = 0; j < atual -> colunas; j++) {
            if (atual -> tabuleiro[i][j] != anterior -> tabuleiro[i][j]) return true;
        }
    }

    return false;
}

/**
 * @brief Converte uma string para inteiro.
 * 
 * Exemplo: "12" para 12.
 */

int stringParaInteiro (char *str) {
    int resultado = 0;
    int i = 0;

    while (isDigit(str[i])) {
        resultado = resultado * 10 + (str[i] - '0');
        i++;
    }

    return resultado;
}