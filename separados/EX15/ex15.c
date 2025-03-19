#include <stdio.h>


int iguaisConsecutivos (char s[]) {
    int i, consec = 1, max = 0;
    for(i = 0; s[i]; i++) {
        if(s[i] == s[i + 1]) consec++;
        else {
            if(consec > max) max = consec;
            consec = 1;
        }
    }
    return max;
}

int main() {
    char texto[]="aaabcc";
    int resultado = iguaisConsecutivos(texto);
    printf("O comprimento da maior sequência de caracteres iguais é: %d\n", resultado);

    return 0;
}