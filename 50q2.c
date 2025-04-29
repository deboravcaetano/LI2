// PARTE 2 : 50 QUESTÕES


#include <stdlib.h>

typedef struct lligada {
        int valor;
        struct lligada *prox;
    } *LInt;

// 1

int length (LInt l){
    int comprimento = 0;
    while( l != NULL){
        comprimento ++;
        l = l-> prox;
    }

    return comprimento;
}

// 2

void freeL(LInt l){
    LInt temp;
    while(l != NULL){
        temp = l;      // Guarda o nó atual
        l = l->prox;   // Avança
        free(l);       // Liberta o nó atual
    } 
}

// 3

void imprimeL(LInt l) {
    while(l != NULL) {
        printf("%d\n",l->valor);
        l = l->prox;
    }
}

// 4

LInt reverse(LInt l){
    LInt t = NULL;
    LInt temp;
    
    while(l != NULL){
        temp = l->prox;
        l->prox = t;
        t = l;
        l = temp;

    }
    return t;
}

// 5

void insertOrd(LInt *l, int e) {
    LInt p = *l;
    LInt ant = NULL;

    LInt celula = malloc(sizeof(struct lligada));
    celula->valor = e;
    celula->prox = NULL;


    // Encontra a posição correta para inserir
    while (p != NULL && p->valor < e) {
        ant = p;
        p = p->prox;
    }


    if (ant == NULL) {
        // --- Caso 1: Inserir no início 
        celula->prox = *l;
        *l = celula;  
    } else {
        // --- Caso 2: Inserir no meio ou no final
        celula->prox = p;
        ant->prox = celula;
    }
}

// 6

int removeOneOrd(LInt *l, int e) {
    LInt p = *l;
    LInt ant = NULL;

    // Percorre a lista até encontrar o elemento ou chegar ao fim
    while (p != NULL && p->valor < e) {
        ant = p;
        p = p->prox;
    }

    // Se o elemento não existir ou a lista estiver vazia
    if (p == NULL || p->valor != e) {
        return 1; // Elemento não encontrado
    }

    // Remove o elemento
    if (ant == NULL) {
        *l = p->prox; // Remove o primeiro nó
    } else {
        ant->prox = p->prox; // Remove um nó do meio ou final
    }

    free(p); // Liberta a memória do nó a remover
    return 0; 
}

// 7

void merge(LInt *r, LInt a, LInt b) {
    while (a != NULL && b != NULL) {
        if (a->valor < b->valor) {
            *r = a;
            a = a->prox;
            r = &((*r)->prox);
        } else {
            *r = b;
            b = b->prox;
            r = &((*r)->prox);
        }
    }
    if (a == NULL) *r = b;
    if (b == NULL) *r = a;
}

// 8

void splitQS(LInt l, int x, LInt *mx, LInt *Mx) {

    while (l != NULL) {
        if (l->valor < x) {
            *mx = l; // Adiciona o nó à lista dos menores
            mx = &((*mx)->prox);
            l = l->prox;
        } else {
            *Mx = l; // Adiciona o nó à lista dos maiores
            Mx = &((*Mx)->prox);
            l = l->prox;
        }
    }

    // Termina as duas listas
    *mx = NULL;
    *Mx = NULL;
}

// 9

LInt parteAmeio (LInt *l){
    int meio = length(*l)/2;
    int p = 0;

    if(meio == 0) return NULL;

    LInt y = (*l);   // // Aponta para o início da primeira metade
    LInt ant = NULL; // Guarda o último nó da primeira metade

    while(p < meio) {
        ant = (*l); // ant aponta para o nó atual
        (*l) = (*l)->prox;
        p++;
    }
    ant->prox = NULL; // Corta a ligação entre as duas metades
    return y;
}

// 10

int removeAll(LInt *l, int e) {
    int celulasRem = 0;
    LInt p = *l;
    LInt ant = NULL;
    LInt temp;

    while (p != NULL) {
        if (p->valor == e) {
            temp = p; 
            if (ant == NULL) { // Remover o primeiro nó
                *l = p->prox;
                p = *l;
            } else {           // Remover nó do meio/fim
                ant->prox = p->prox;
                p = p->prox;
            }
            free(temp);
            celulasRem++;
        } else {
            ant = p;
            p = p->prox;
        }
    }
    return celulasRem;
}

// 11










