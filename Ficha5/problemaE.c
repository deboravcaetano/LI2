// PROBLEMA E LI2 24/25

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int l, c, r;
} TriploMov;

typedef struct {
    int dl, dc;           // Dimensões do tabuleiro
    int lI, cI;           // Coordenadas iniciais
    int lF, cF;           // Coordenadas finais
    TriploMov *triplosM;  // Array de movimentos possíveis
    int t;                // Número de triplos de movimento
    char **tabuleiro;     // Matriz do tabuleiro com letras
    char *trajeto;        // String do caminho a ser formado
    int **visitado;       // Matriz para marcar posições visitadas
} InfoPeca;

typedef InfoPeca *INFO;

typedef struct {
    InfoPeca dados;
    int resultado; // 1 ou 0 indicando se existe um caminho válido
} CasoTeste;


// Lê o tabuleiro com as letras
int lerTabuleiro(INFO infoP, int L, int C) {
    // Linhas
    infoP->dl = L;
    // Colunas
    infoP->dc = C;
    
    // Aloca memória para o tabuleiro
    infoP->tabuleiro = malloc(L * sizeof(char*));
    if (infoP->tabuleiro == NULL) return 1;
    
    // Para cada linha do tabuleiro
    for (int i = 0; i < L; i++) {
        infoP->tabuleiro[i] = malloc(C * sizeof(char));
        if (infoP->tabuleiro[i] == NULL) {
            for (int j = 0; j < i; j++) free(infoP->tabuleiro[j]);
            free(infoP->tabuleiro);
            return 1;
        }
        
        char linha[C + 1];
        // Lê cada linha do tabuleiro e armazena em linha
        if (scanf("%s", linha) != 1) {
            for (int j = 0; j <= i; j++) free(infoP->tabuleiro[j]);
            free(infoP->tabuleiro);
            return 1;
        }
        
        // Copia cada linha para o tabuleiro
        for (int j = 0; j < C; j++) {
            infoP->tabuleiro[i][j] = linha[j];
        }
    }
    
    return 0;
}

// Lê um caso de teste
int lerCaso(CasoTeste *caso) {
    INFO infoP = &caso->dados;

    // Lê o número de movimentos
    if (scanf("%d", &infoP->t) != 1) return 1;

    // Aloca memória para o array de movimentos
    infoP->triplosM = malloc(infoP->t * sizeof(TriploMov));
    if (infoP->triplosM == NULL) return 1;

    // Lê os triplos 
    for (int i = 0; i < infoP->t; i++) {
        if (scanf("%d %d %d", &infoP->triplosM[i].l, &infoP->triplosM[i].c, &infoP->triplosM[i].r) != 3) {
            free(infoP->triplosM);
            return 1;
        }
    }

    // Lê as coordenadas iniciais e finais
    if (scanf("%d %d %d %d", &infoP->lI, &infoP->cI, &infoP->lF, &infoP->cF) != 4) {
        free(infoP->triplosM);
        return 1;
    }
    
    // Por causa dos arrays começarem em 0, subtraimos 1 às coordenadas
    infoP->lI--; infoP->cI--; infoP->lF--; infoP->cF--;

    // Aloca memória para a string do trajeto
    infoP->trajeto = malloc(100 * sizeof(char));
    if (infoP->trajeto == NULL) {
        free(infoP->triplosM);
        return 1;
    }
    
    // Lê a string que corresponde ao trajeto 
    if (scanf("%s", infoP->trajeto) != 1) {
        free(infoP->triplosM);
        free(infoP->trajeto);
        return 1;
    }

    // Inicializa
    infoP->visitado = NULL; 

    return 0;
}

// Inicializa a matriz de visitados
int inicializarVisitado(INFO infoP) {
    // Liberta memória alocada anteriormente se houver
    if (infoP->visitado != NULL) {
        for (int i = 0; i < infoP->dl; i++) {
            free(infoP->visitado[i]);
        }
        free(infoP->visitado);
    }

    // Aloca memória para a matriz visitado
    infoP->visitado = malloc(infoP->dl * sizeof(int*));
    if (infoP->visitado == NULL) return 1;
    
    for (int i = 0; i < infoP->dl; i++) {
        infoP->visitado[i] = malloc(infoP->dc * sizeof(int));
        // Se falhar
        if (infoP->visitado[i] == NULL) {
            for (int j = 0; j < i; j++) free(infoP->visitado[j]);
            free(infoP->visitado);
            return 1;
        }
        // Inicializa com tudo a zeros que indica que a posição correspondente no tabuleiro ainda não foi visitada
        for (int j = 0; j < infoP->dc; j++) {
            infoP->visitado[i][j] = 0;
        }
    }
    return 0;
}

// Verifica se é uma posição válida tendo em conta os limites do tabuleiro 
int posicaoValida(INFO infoP, int l, int c) {
    return (l >= 0 && l < infoP->dl && c >= 0 && c < infoP->dc);
}

// Reconhece a função antes de ser chamada
int procuraCaminho(INFO infoP, int l, int c, char *caminho, int pathIdx);

int movimentoNaoRecorrente(INFO infoP, int l, int c, int dl, int dc, char *caminho, int pathIdx) {
    // Calcula nova posição com base na direção
    int novaL = l + dl;
    int novaC = c + dc;
    if (posicaoValida(infoP, novaL, novaC)) {
        // Chama recursão a partir da nova posição 
        if (procuraCaminho(infoP, novaL, novaC, caminho, pathIdx + 1)) {
            // Retorna 1 se o caminho der certo
            return 1;
        }
    }
    return 0;
}

int movimentoRecorrente(INFO infoP, int l, int c, int dl, int dc, char *caminho, int pathIdx) {
    // Controla a distância que a peça se move em cada iteração
    int m = 1;
    int podeContinuar = 1;
    while (podeContinuar) {
        int novaL = l + m * dl;
        int novaC = c + m * dc;
        if (!posicaoValida(infoP, novaL, novaC)) {
            podeContinuar = 0;
        } else {
            if (procuraCaminho(infoP, novaL, novaC, caminho, pathIdx + 1)) {
                return 1;
            }
            m++;
        }
    }
    return 0;
}

int procuraCaminho(INFO infoP, int l, int c, char *caminho, int pathIdx) {
    // Se é inválida ou já foi visitada é caminho inválido
    if (!posicaoValida(infoP, l, c) || infoP->visitado[l][c]) return 0; 

    // Verifica se a letra da célula corresponde à esperada no trajeto
    if (infoP->tabuleiro[l][c] != infoP->trajeto[pathIdx]) return 0;

    // Adiciona a letra ao caminho atual
    caminho[pathIdx] = infoP->tabuleiro[l][c];

    // Garante que a string seja sempre terminada
    caminho[pathIdx + 1] = '\0';

    // Marca a célula como visitada
    infoP->visitado[l][c] = 1;

    // Verifica se já chegou ao final e se a string formada até agora é igual à esperada
    if (l == infoP->lF && c == infoP->cF && strcmp(caminho, infoP->trajeto) == 0) { // strcmp retorna 0 se as duas são iguais
        return 1;
    }

    // Testa todos os movimentos possíveis    
    for (int i = 0; i < infoP->t; i++) {
        // Extrai os dados do movimento atual
        int dl = infoP->triplosM[i].l, dc = infoP->triplosM[i].c, r = infoP->triplosM[i].r;

        // 8 direções possíveis com base nesse movimento
        int direcoes[8][2] = {
            {dl, dc}, {dl, -dc}, {-dl, dc}, {-dl, -dc},
            {dc, dl}, {dc, -dl}, {-dc, dl}, {-dc, -dl}
        };

        // Tenta um movimento para cada direção
        for (int j = 0; j < 8; j++) {
            if (r == 0) {
                if (movimentoNaoRecorrente(infoP, l, c, direcoes[j][0], direcoes[j][1], caminho, pathIdx)) {
                    return 1;
                }
            } else {
                if (movimentoRecorrente(infoP, l, c, direcoes[j][0], direcoes[j][1], caminho, pathIdx)) {
                    return 1;
                }
            }
        }
    }

    // Se nada deu certo, desmarca a posição
    infoP->visitado[l][c] = 0;
    return 0;
}


// Verifica se existe um caminho válido
int existeCaminho(CasoTeste *caso) {
    // Cria um ponteiro infoP que aponta para a estrutura dados de um caso de teste
    INFO infoP = &caso->dados;
    
    if (inicializarVisitado(infoP)) {
        return 0;
    }
    
    char caminho[strlen(infoP->trajeto) + 1];
    caminho[0] = '\0';
    
    //O 0 indica o indice atual na string que vamos tentar construir
    return procuraCaminho(infoP, infoP->lI, infoP->cI, caminho, 0);
}

// Liberta a memória de um caso de teste
void libertarMemoriaCaso(CasoTeste *caso) {
    // Em vez de escrever caso->dados.campo escrevemos infoP->campo
    INFO infoP = &caso->dados;
    
    free(infoP->triplosM);
    free(infoP->trajeto);
    
    if (infoP->visitado != NULL) {
        for (int i = 0; i < infoP->dl; i++) {
            free(infoP->visitado[i]);
        }
        free(infoP->visitado);
    }
}

// Liberta a memória do tabuleiro
void libertarMemoriaTabuleiro(INFO infoP) {
    for (int i = 0; i < infoP->dl; i++) {
        free(infoP->tabuleiro[i]);
    }
    free(infoP->tabuleiro);
}

// Processa todos os casos de teste e imprime os resultados
void processarTodosCasos(CasoTeste *casos, int nCasos) {
    for (int i = 0; i < nCasos; i++) {
        casos[i].resultado = existeCaminho(&casos[i]);
        printf("Caso %d\n", i + 1);
        // Se é válido
        if (casos[i].resultado) {
            printf("SIM\n");
        } else {
            printf("NAO\n");
        }
        libertarMemoriaCaso(&casos[i]);
    }
}

int main() {
    int L, C, nCasos;
    
    // Lê as dimensões do tabuleiro
    if (scanf("%d %d", &L, &C) != 2) return 1;
    
    InfoPeca tabuleiroInfo;

    // Lê o tabuleiro em si
    if (lerTabuleiro(&tabuleiroInfo, L, C)) return 1;
    
    // Lê o número de casos
    if (scanf("%d", &nCasos) != 1) {
        libertarMemoriaTabuleiro(&tabuleiroInfo);
        return 1;
    }
    
    // Armazena todos os casos
    CasoTeste casos[nCasos];
    
    // Lê todos os casos
    for (int i = 0; i < nCasos; i++) {
        // Se algo correr mal
        if (lerCaso(&casos[i])) {
            for (int j = 0; j < i; j++) {
                libertarMemoriaCaso(&casos[j]);
            }
            libertarMemoriaTabuleiro(&tabuleiroInfo);
            return 1;
        }
        // Copia o tabuleiro para cada caso
        casos[i].dados.dl = tabuleiroInfo.dl;
        casos[i].dados.dc = tabuleiroInfo.dc;
        casos[i].dados.tabuleiro = tabuleiroInfo.tabuleiro;
        casos[i].dados.visitado = NULL;
    }
    
    // Processa e imprime os resultados
    processarTodosCasos(casos, nCasos);
    
    // Liberta a memória do tabuleiro
    libertarMemoriaTabuleiro(&tabuleiroInfo);
    
    return 0;
}



