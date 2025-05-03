// PROBLEMA E LI2 24/25

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int l, c, r;
} TriploMov;

typedef struct {
    int dl, dc;          
    int lI, cI;           
    int lF, cF;           
    TriploMov *triplosM;  
    int t;                
    char **tabuleiro;    
    char *trajeto;        
    int **visitado;       
} InfoPeca;

typedef InfoPeca *INFO;

typedef struct {
    InfoPeca dados;
    int resultado; // 1 ou 0 indicando se existe um caminho válido
} CasoTeste;



int lerTabuleiro(INFO infoP, int L, int C) {
    infoP->dl = L;
    infoP->dc = C;
    
    infoP->tabuleiro = malloc(L * sizeof(char*));
    if (infoP->tabuleiro == NULL) return 1;
    
    for (int i = 0; i < L; i++) {
        infoP->tabuleiro[i] = malloc(C * sizeof(char));
        if (infoP->tabuleiro[i] == NULL) {
            for (int j = 0; j < i; j++) free(infoP->tabuleiro[j]);
            free(infoP->tabuleiro);
            return 1;
        }
        
        char linha[C + 1];
        if (scanf("%s", linha) != 1) {
            for (int j = 0; j <= i; j++) free(infoP->tabuleiro[j]);
            free(infoP->tabuleiro);
            return 1;
        }
        
        for (int j = 0; j < C; j++) {
            infoP->tabuleiro[i][j] = linha[j];
        }
    }
    
    return 0;
}


int lerCaso(CasoTeste *caso) {
    INFO infoP = &caso->dados;

    if (scanf("%d", &infoP->t) != 1) return 1;

    infoP->triplosM = malloc(infoP->t * sizeof(TriploMov));
    if (infoP->triplosM == NULL) return 1;

    for (int i = 0; i < infoP->t; i++) {
        if (scanf("%d %d %d", &infoP->triplosM[i].l, &infoP->triplosM[i].c, &infoP->triplosM[i].r) != 3) {
            free(infoP->triplosM);
            return 1;
        }
    }

    if (scanf("%d %d %d %d", &infoP->lI, &infoP->cI, &infoP->lF, &infoP->cF) != 4) {
        free(infoP->triplosM);
        return 1;
    }
    

    infoP->lI--; infoP->cI--; infoP->lF--; infoP->cF--;


    infoP->trajeto = malloc(40 * 40 + 1 * sizeof(char));
    if (infoP->trajeto == NULL) {
        free(infoP->triplosM);
        return 1;
    }
    

    if (scanf("%s", infoP->trajeto) != 1) {
        free(infoP->triplosM);
        free(infoP->trajeto);
        return 1;
    }

    // Inicializa
    infoP->visitado = NULL; 

    return 0;
}


int inicializarVisitado(INFO infoP) {
    // Liberta memória alocada anteriormente se houver
    if (infoP->visitado != NULL) {
        for (int i = 0; i < infoP->dl; i++) {
            free(infoP->visitado[i]);
        }
        free(infoP->visitado);
    }

    
    infoP->visitado = malloc(infoP->dl * sizeof(int*));
    if (infoP->visitado == NULL) return 1;
    
    for (int i = 0; i < infoP->dl; i++) {
        infoP->visitado[i] = malloc(infoP->dc * sizeof(int));
        if (infoP->visitado[i] == NULL) {
            for (int j = 0; j < i; j++) free(infoP->visitado[j]);
            free(infoP->visitado);
            return 1;
        }

        for (int j = 0; j < infoP->dc; j++) {
            infoP->visitado[i][j] = 0;
        }
    }
    return 0;
}


int posicaoValida(INFO infoP, int l, int c) {
    return (l >= 0 && l < infoP->dl && c >= 0 && c < infoP->dc);
}


int procuraCaminho(INFO infoP, int l, int c, char *caminho, int pathIdx);

int movimentoNaoRecorrente(INFO infoP, int l, int c, int dl, int dc, char *caminho, int pathIdx) {
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

    if (!posicaoValida(infoP, l, c) || infoP->visitado[l][c]) return 0; 

    if (infoP->tabuleiro[l][c] != infoP->trajeto[pathIdx]) return 0;

    caminho[pathIdx] = infoP->tabuleiro[l][c];
    caminho[pathIdx + 1] = '\0';

    infoP->visitado[l][c] = 1;

    
    if (l == infoP->lF && c == infoP->cF && strcmp(caminho, infoP->trajeto) == 0) { 
        return 1;
    }
   
    for (int i = 0; i < infoP->t; i++) {
        int dl = infoP->triplosM[i].l, dc = infoP->triplosM[i].c, r = infoP->triplosM[i].r;

        int direcoes[8][2] = {
            {dl, dc}, {dl, -dc}, {-dl, dc}, {-dl, -dc},
            {dc, dl}, {dc, -dl}, {-dc, dl}, {-dc, -dl}
        };

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

    infoP->visitado[l][c] = 0;
    return 0;
}


int existeCaminho(CasoTeste *caso) {
    INFO infoP = &caso->dados;
    
    if (inicializarVisitado(infoP)) {
        return 0;
    }
    
    char caminho[strlen(infoP->trajeto) + 1];
    caminho[0] = '\0';
    
    return procuraCaminho(infoP, infoP->lI, infoP->cI, caminho, 0);
}


void libertarMemoriaCaso(CasoTeste *caso) {
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

void libertarMemoriaTabuleiro(INFO infoP) {
    for (int i = 0; i < infoP->dl; i++) {
        free(infoP->tabuleiro[i]);
    }
    free(infoP->tabuleiro);
}


void processarTodosCasos(CasoTeste *casos, int nCasos) {
    for (int i = 0; i < nCasos; i++) {
        casos[i].resultado = existeCaminho(&casos[i]);
        printf("Caso %d\n", i + 1);

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
    
    if (scanf("%d %d", &L, &C) != 2) return 1;
    
    InfoPeca tabuleiroInfo;

    if (lerTabuleiro(&tabuleiroInfo, L, C)) return 1;
    
    if (scanf("%d", &nCasos) != 1) {
        libertarMemoriaTabuleiro(&tabuleiroInfo);
        return 1;
    }
    
    CasoTeste casos[nCasos];

    for (int i = 0; i < nCasos; i++) {
        if (lerCaso(&casos[i])) {
            for (int j = 0; j < i; j++) {
                libertarMemoriaCaso(&casos[j]);
            }
            libertarMemoriaTabuleiro(&tabuleiroInfo);
            return 1;
        }

        casos[i].dados.dl = tabuleiroInfo.dl;
        casos[i].dados.dc = tabuleiroInfo.dc;
        casos[i].dados.tabuleiro = tabuleiroInfo.tabuleiro;
        casos[i].dados.visitado = NULL;
    }
    
    processarTodosCasos(casos, nCasos);
    
    libertarMemoriaTabuleiro(&tabuleiroInfo);
    
    return 0;
}



