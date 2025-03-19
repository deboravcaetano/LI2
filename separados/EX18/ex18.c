#include <stdio.h>

int tamanhoString(char s[]) {
    int i;
    for(i = 0; s[i] != '\0'; i++);
    return i;
}

int maiorSufixo (char s1 [], char s2 []){
    int i,j,cont;
    int tam1 = tamanhoString(s1);
    int tam2 = tamanhoString(s2);
    for(i=tam1-1,j=tam2-1; s1[i] ==  s2[j] && i != 0 && j != 0; i--,j--) cont++;
    return cont;

}

int main() {
    char texto1[] = "artista"; 
    char texto2[] ="transporta";
    int maiorS = maiorSufixo(texto1,texto2);
    printf("O comprimento do maior prefixo é: %d\n", maiorS);

    return 0;
}