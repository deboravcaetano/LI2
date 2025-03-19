#include <stdio.h>

char *strcat(char s1[], char s2[]) {
    int i, j;

    // Encontra o final da string s1
    for (i = 0; s1[i] != '\0'; i++); // Loop vazio: apenas avança 'i'

    // Copia s2 para o final de s1
    for (j = 0; s2[j] != '\0'; j++) {
        s1[i++] = s2[j];
    }


    s1[i] = '\0';

    return s1; // Retorna um ponteiro para a string concatenada (s1)
}

int main() {
    char str1[100] = "Hello, ";  // Garanta espaço suficiente em s1
    char str2[] = "world!";

    printf("Antes: str1 = '%s', str2 = '%s'\n", str1, str2);

    strcat(str1, str2);

    printf("Depois: str1 = '%s'\n", str1);

    return 0;
}
