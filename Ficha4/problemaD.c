// PROBLEMA D LI2 24/25

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int l, c, r;
} TriploMov; // Representa um movimento que a peça pode fazer

typedef struct {
    int linha, coluna;
} Posicao; // Representa uma posição específica no tabuleiro

typedef struct {
    int dl, dc; // Dimensões tabuleiro
    int lI, cI; // Coordenadas iniciais
    TriploMov *triplosM; // Ponteiro para um array de TriploMov
    int t; // Tamanho do array triplosM (total de movimentos possíveis)
    Posicao *coordenadas; // Ponteiro para um array que armazena todas as posições que pode alcançar
    int nC; // Tamanho do array coordenadas
} InfoPeca; // Todas as informações sobre uma peça

typedef InfoPeca *INFO;

// Lê os dados de um caso de teste do input 
int lerCasos(INFO infoP) {
    // Lê o número de movimentos
    if (scanf("%d", &infoP->t) != 1) return 1;

    // Aloca memória para o array de movimentos
    infoP->triplosM = malloc(infoP->t * sizeof(TriploMov));
    if (infoP->triplosM == NULL) return 1;

    // Lê o deslocamento na linha, na coluna e a recorrência
    for (int i = 0; i < infoP->t; i++) {
        if (scanf("%d %d %d", &infoP->triplosM[i].l, &infoP->triplosM[i].c, &infoP->triplosM[i].r) != 3) {
            free(infoP->triplosM);
            return 1;
        }
    }

    // Lê as dimensões do tabuleiro e as coordenadas iniciais
    if (scanf("%d %d %d %d", &infoP->dl, &infoP->dc, &infoP->lI, &infoP->cI) != 4) {
        free(infoP->triplosM);
        return 1;
    }

    // Aloca memória para o array de coordenadas
    infoP->coordenadas = malloc(infoP->dl * infoP->dc * sizeof(Posicao));
    if (infoP->coordenadas == NULL) {
        free(infoP->triplosM);
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

// Adiciona a posição no array
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
void movNaoRecorrentes(INFO infoP,int lI, int cI, int direcoes[8][2]) {
    for (int j = 0; j < 8; j++) {
        int novaL = lI + direcoes[j][0];
        int novaC = cI + direcoes[j][1];

        if (podeMover(novaL, novaC, infoP->dl, infoP->dc)) {
            adicionaPosicao(infoP, novaL, novaC);
        }
    }
}


// Calcula e adiciona no array coordenadas de movimentos recorrentes
void movRecorrentes(INFO infoP, int lI, int cI, int direcoes[8][2]) {
    for (int j = 0; j < 8; j++) {
        // Controla a distância que a peça se move em cada iteração
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
        int dl = infoP->triplosM[i].l;
        int dc = infoP->triplosM[i].c;
        int r = infoP->triplosM[i].r;

        // Todas as direções possíveis
        int direcoes[8][2] = {
            {dl, dc}, {dl, -dc}, {-dl, dc}, {-dl, -dc},
            {dc, dl}, {dc, -dl}, {-dc, dl}, {-dc, -dl}
        };

        if (r == 0) { // Movimento não recorrente
            movNaoRecorrentes(infoP, infoP->lI, infoP->cI, direcoes);
        } else { // Movimento recorrente
            movRecorrentes(infoP, infoP->lI, infoP->cI, direcoes);
        }
    }
}

// Liberta memória
void libertarMemoria(INFO infoP) {
    free(infoP->triplosM);
    free(infoP->coordenadas);
}

// Função para imprimir os resultados de um caso
void imprimirResultados(int caso, INFO infoP) {
    printf("Caso %d\n", caso);
    for (int i = 0; i < infoP->nC; i++) {
        printf("%d %d\n", infoP->coordenadas[i].linha, infoP->coordenadas[i].coluna);
    }
}

int main() {
    int nCasos;
    // Lê o número de casos
    if (scanf("%d", &nCasos) != 1) return 1;

    // Array de ponteiros para structs InfoPeca para guardar os resultados de cada caso 
    InfoPeca *resultados[nCasos];

    for (int c = 1; c <= nCasos; c++) {
        // Aloca memória para uma struct
        InfoPeca *info = malloc(sizeof(InfoPeca));
        // Verifica se a alocação de memória foi bem sucedida
        if (info == NULL) return 1;

        // Lê os dados de cada caso e preenche a struct
        if (lerCasos(info)) {
            free(info);
            return 1;
        }

        // Após a struct ser preenchida calcula as coordenadas possíveis
        coordenadasP(info);

        ordenaCoord(info);

        // Guarda os resultados no array
        resultados[c - 1] = info;
    }

    // Imprime os resultados de todos os casos
    for (int c = 1; c <= nCasos; c++) {
        imprimirResultados(c, resultados[c - 1]);
        libertarMemoria(resultados[c - 1]);
        free(resultados[c - 1]);
    }

    return 0;
}
