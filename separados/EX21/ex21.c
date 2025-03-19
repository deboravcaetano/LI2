#include <stdio.h>

int contaVogais(char s[]) {
    char vogais[] = "aeiouAEIOU";
    int i, j;
    int numvogais = 0;
    
    for (i = 0; s[i] != '\0'; i++) {
        for (j = 0; vogais[j] != '\0'; j++) {
            if (s[i] == vogais[j]) {
                numvogais++;
            }
        }
    }
    return numvogais;
}

int main() {
    char frase[] = "Esta frase tem vogais";  
    printf("Número de vogais: %d\n", contaVogais(frase));
    return 0;
}