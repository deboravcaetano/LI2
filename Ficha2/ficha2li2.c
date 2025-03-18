#include <stdio.h>


int tamanhoString (char *s){
    int i;
    for(i=0;s[i] != '\0';i++);
    return i;
}


long long maximoProduto (char *N, int S) {
    long long maxProduto = 0;
    int tam = tamanhoString(N);
    for (int i = 0; i <= tam - S; i++) {
        long long produto = 1;
        int bool = 1;
        for (int j = 0; j < S && bool; j++) {
            if ((N[i+j]-'0') != 0){
               produto *= (N[i + j] - '0');
            } else {
            bool = 0;
            }
            if (produto > maxProduto) {
                maxProduto = produto;
            }
        }
    }
    return maxProduto;
}


int main() {
    int C;
    // lê o número de casos de teste
    if (scanf("%d", &C) != 1) 
       return 1;

    // armazena num array com C elementos
    long long resultados[C];

    // processa cada caso de teste
    for (int i = 0; i < C; i++) {
        // lê o número N e o tamanho S
        char N[1001];
        int S;
        if (scanf("%s %d", N, &S) != 2)
           return 1 ;

        // armazena o resultado no array
        resultados[i] = maximoProduto(N, S);
    }

    // imprime todos os resultados
    for (int i = 0; i < C; i++) {
        printf("%lld\n", resultados[i]);
    }

    return 0;
}
