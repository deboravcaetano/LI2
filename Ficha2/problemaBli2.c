#include <stdio.h>
#include <string.h>

// verifica se o número armazenado na produtoGAtual é maior que a do maiorProduto (strings com o mesmo tamanho).
int compProduto (char produtoGAtual[], char maiorProduto[]) {
    int i, tam = strlen(produtoGAtual);

    for (i = 0; i < tam; i++) {
        if (produtoGAtual[i] > maiorProduto[i]) return 1;
        else if (produtoGAtual[i] < maiorProduto[i]) return 0;
    }
    return 0;
}

// remove os zeros (caracteres) no inúcios de um array.
void removeZerosIniciais (char numero[]) {
    int i, tam;
    tam = strlen (numero);
    
    char aux[tam + 1];
    strcpy (aux, numero);

    for (i = 0; i < tam && numero [i] == '0'; i++);

    strcpy (numero, aux + i);
}

// calcula o produto de digitos numa string
void prodGrupo (char grupo[], char produto[]) {
    int i, j, resultado, carry, tam;
    char temp;

    tam = strlen (grupo);

    produto[0] = grupo[0];
    for (i = 1; i < tam; i++) produto[i] = '0';
    produto[i] = '\0';

    for (i = 1; grupo[i] != '\0'; i++) {
        for (j = carry = 0; j <= i; j++) {
            resultado = (grupo[i] - '0') * (produto[j] - '0') + carry;
            carry = resultado / 10;
            produto[j] = resultado % 10 + '0';
        }
    }

    tam = strlen (produto);
    // Inverte o resultado
    for (i = 0, j = tam - 1; i < j; i++, j--) {
        temp = produto[i];
        produto[i] = produto[j];
        produto[j] = temp;
    }
}

// produz o maior produto de S dígitos consecutivos 
void maxProdCons (char r[], int S) {
    int i, j;
    
    char grupoAtual[S + 1];
    char produtoGAtual[S + 1];
    char maiorProduto[S + 1];

    for (int i = 0; i < S; i++) {
        maiorProduto[i] = '0';
    }
    maiorProduto[S] = '\0';

    for (i = 0; r[i + S - 1] != '\0'; i++) { 
        for (j = 0; j < S; j++) grupoAtual[j] = r[i + j];
        grupoAtual[j] = '\0';

        prodGrupo (grupoAtual, produtoGAtual);

        if (compProduto (produtoGAtual, maiorProduto)) strcpy (maiorProduto, produtoGAtual);
    }

    strcpy (r, maiorProduto);

    removeZerosIniciais (r);
}

int main () {    
    int C;

    if (scanf("%d", &C) != 1) return 1;

    char resultados[C][1000];

    // processa cada caso de teste
    for (int i = 0; i < C; i++) {
        int S;
        if (scanf("%s %d", resultados[i], &S) != 2) return 1;
        maxProdCons (resultados[i], S);
    }

    for (int i = 0; i < C; i++) printf("%s\n", resultados[i]);
    
    return 0;
}