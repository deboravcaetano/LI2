#include <stdio.h>

int tamanhoString(char s[]) {
    int i;
    for(i = 0; s[i] != '\0'; i++);
    return i;
}

void strrev(char s[]) {
    int i, j;
    char t;
    int tam = tamanhoString(s);
    for (i = 0, j = tam - 1; i < j; i++, j--) {
        t = s[i];
        s[i] = s[j];
        s[j] = t;
    }
}

int main() {
    char s[] = "world";
    printf("String original: %s\n", s);
    strrev(s);
    printf("A string invertida é: %s\n", s);

    return 0;
}



