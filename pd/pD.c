#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int l, c, r;
} Movimento;

typedef struct {
    int linha, coluna;
} Posicao;

typedef struct {
    int dl, dc;
    int lI, cI;
    Movimento *movimentos;
    int t;
    Posicao *coordenadas;
    int nC;
} InfoPeca;

typedef InfoPeca *INFO;

typedef struct {
    int nC;
    Posicao *coordenadas;
} ResultadoCaso;


// Lê os dados de um caso de teste do input e preenche a estrutura InfoPeca com esses dados
int lerCasos(INFO infoP) {
    // Número de movimentos
    if (scanf("%d", &infoP->t) != 1) return 1;

    // Aloca memória para o array de movimentos
    infoP->movimentos = malloc(infoP->t * sizeof(Movimento));
    if (infoP->movimentos == NULL) return 1;

    // Lê o deslocamento na linha, na coluna e a recorrência
    for (int i = 0; i < infoP->t; i++) {
        if (scanf("%d %d %d", &infoP->movimentos[i].l, &infoP->movimentos[i].c, &infoP->movimentos[i].r) != 3) {
            free(infoP->movimentos);
            return 1;
        }
    }

    // Lê as dimensões do tabuleiro e as coordenadas iniciais
    if (scanf("%d %d %d %d", &infoP->dl, &infoP->dc, &infoP->lI, &infoP->cI) != 4) {
        free(infoP->movimentos);
        return 1;
    }

    // Aloca memória para o array de coordenadas
    infoP->coordenadas = malloc(infoP->dl * infoP->dc * sizeof(Posicao));
    if (infoP->coordenadas == NULL) {
        free(infoP->movimentos);
        return 1;
    }

    // Inicializa o contador de coordenadas a 0
    infoP->nC = 0;

    return 0;
}

// Verifica se a posição é válida tendo em conta as dimensões do tabuleiro
int podeMover(int l, int c, int dl, int dc) {
    return (l >= 1 && l <= dl && c >= 1 && c <= dc);
}

// Verifica se dadas coordenadas já existem no array
int existeC(INFO infoP, int l, int c) {
    for (int i = 0; i < infoP->nC; i++) {
        if (infoP->coordenadas[i].linha == l && infoP->coordenadas[i].coluna == c) {
            return 1;
        }
    }
    return 0;
}

// Adiciona as coordenadas no array
void adicionaPosicao(INFO infoP, int l, int c) {
    if (!existeC(infoP, l, c)) {
        infoP->coordenadas[infoP->nC].linha = l;
        infoP->coordenadas[infoP->nC].coluna = c;
        infoP->nC++;
    }
}

// Ordena as coordenadas pela primeira e depois pela segunda
void ordenaCoord(INFO infoP) {
    for (int i = 0; i < infoP->nC; i++) {
        for (int j = 0; j < infoP->nC - 1 - i; j++) {
            if (infoP->coordenadas[j].linha > infoP->coordenadas[j + 1].linha ||
                (infoP->coordenadas[j].linha == infoP->coordenadas[j + 1].linha && infoP->coordenadas[j].coluna > infoP->coordenadas[j + 1].coluna)) {
                Posicao tmp = infoP->coordenadas[j];
                infoP->coordenadas[j] = infoP->coordenadas[j + 1];
                infoP->coordenadas[j + 1] = tmp;
            }
        }
    }
}

// Calcula e adiciona ao array coordenadas de movimentos não recorrentes, que só podem ser feitos uma vez segundo uma direção
void movNaoRecorrentes(INFO infoP, int dl, int dc, int lI, int cI, int r, int direcoes[8][2]) {
    for (int j = 0; j < 8; j++) {
        int novaL = lI + direcoes[j][0];
        int novaC = cI + direcoes[j][1];

        if (podeMover(novaL, novaC, infoP->dl, infoP->dc)) {
            adicionaPosicao(infoP, novaL, novaC);
        }
    }
}

// Calcula e adiciona no array coordenadas de movimentos recorrentes
void movRecorrentes(INFO infoP, int dl, int dc, int lI, int cI, int r, int direcoes[8][2]) {
    for (int j = 0; j < 8; j++) {
        int m = 1;
        int continuar = 1;

        while (continuar) {
            int novaL = lI + m * direcoes[j][0];
            int novaC = cI + m * direcoes[j][1];

            if (podeMover(novaL, novaC, infoP->dl, infoP->dc)) {
                adicionaPosicao(infoP, novaL, novaC);
                m++;
            } else {
                continuar = 0;
            }
        }
    }
}

// Calcula todas as posições alcançáveis dados os movimentos possíveis
void coordenadasP(INFO infoP) {
    for (int i = 0; i < infoP->t; i++) {
        int dl = infoP->movimentos[i].l;
        int dc = infoP->movimentos[i].c;
        int r = infoP->movimentos[i].r;

        // Todas as direções possíveis
        int direcoes[8][2] = {
            {dl, dc}, {dl, -dc}, {-dl, dc}, {-dl, -dc},
            {dc, dl}, {dc, -dl}, {-dc, dl}, {-dc, -dl}
        };

        if (r == 0) { // Movimento não recorrente
            movNaoRecorrentes(infoP, dl, dc, infoP->lI, infoP->cI, r, direcoes);
        } else { // Movimento recorrente
            movRecorrentes(infoP, dl, dc, infoP->lI, infoP->cI, r, direcoes);
        }
    }
}

// Liberta memória
void libertarMemoria(INFO infoP) {
    free(infoP->movimentos);
    free(infoP->coordenadas);
}

// Função para imprimir os resultados de um caso 
void imprimirResultados(int caso, ResultadoCaso resultado) {
    printf("Caso %d\n", caso);
    for (int i = 0; i < resultado.nC; i++) {
        printf("%d %d\n", resultado.coordenadas[i].linha, resultado.coordenadas[i].coluna);
    }
}

int main() {
    int nCasos;
    if (scanf("%d", &nCasos) != 1) return 1;

    // Array para guardar os resultados de cada caso
    ResultadoCaso resultados[nCasos];

    for (int c = 1; c <= nCasos; c++) {
        InfoPeca *info = malloc(sizeof(InfoPeca));
        if (info == NULL) return 1;

        if (lerCasos(info)) {
            free(info);
            return 1;
        }

        coordenadasP(info);
        ordenaCoord(info);

        // Guarda os resultados no array
        resultados[c - 1].nC = info->nC;
        resultados[c - 1].coordenadas = malloc(info->nC * sizeof(Posicao));
        for (int i = 0; i < info->nC; i++) {
            resultados[c - 1].coordenadas[i] = info->coordenadas[i];
        }

        libertarMemoria(info);
        free(info);
    }

    // Imprime os resultados de todos os casos
    for (int c = 1; c <= nCasos; c++) {
        imprimirResultados(c, resultados[c - 1]);
        free(resultados[c - 1].coordenadas); // Liberta a memória alocada para os resultados
    }

    return 0;
}



