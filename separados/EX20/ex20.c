#include <stdio.h>



int contaPal(char s[]) {
    int i, cont = 0;
    int dentroDaPalavra = 0; // vai funcionar como flagg indicar se estamos dentro de uma palavra

    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] != ' ' && !dentroDaPalavra) { // verifica se não estamos dentro de uma palavra (dentroDaPalavra == 0).
// Isso significa que o caractere atual é o início de uma nova palavra.
            cont++; 
            dentroDaPalavra = 1; 
        }
        // se o caractere atual for um espaço, marca que não estamos mais dentro de uma palavra
        else if (s[i] == ' ') {
            dentroDaPalavra = 0;
        }
    }

    return cont; 
}

int main() {
    char texto[] = "a a bb a"; 
    int numeroP = contaPal(texto); 

    printf("O número de palavras é: %d\n", numeroP); 

    return 0;
}