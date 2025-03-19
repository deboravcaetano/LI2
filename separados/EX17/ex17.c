#include <stdio.h>

int maiorPrefixo (char s1 [], char s2 []) {
    int i;
    for(i = 0; s1[i] == s2[i] && s1[i]; i++);
    return i;
}

int main() {
    char texto1[] = "transporte"; 
    char texto2[] ="transforma";
    int maiorP = maiorPrefixo(texto1,texto2);
    printf("O comprimento do maior prefixo é: %d\n", maiorP);

    return 0;
}