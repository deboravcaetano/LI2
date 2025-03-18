#include <stdio.h>

void contarMultiplos(char l, int I, int F, int K) {
    int cont = 0;
    for (int mult = K; mult <= F; mult += K) {
        if (mult >= I) {
            cont++;
        }
    }
    printf("%c %d\n", l, cont);
}

int main() {
    int N, I, F;
    if (scanf("%d", &N) != 1)
        return 1; 

    int Ks[26] = {0}; 

    for (int i = 0; i < N; i++) {
        char letra;
        int K;
        if (scanf(" %c %d", &letra, &K)!= 2)
           return 1; 
        Ks[letra - 'A'] = K;
    }

    if (scanf("%d %d", &I, &F) != 2)
       return 1;

    for (int i = 0; i < 26; i++) {
        if (Ks[i] != 0) { 
            contarMultiplos('A' + i, I, F, Ks[i]);
        }
    }

    return 0;
}
