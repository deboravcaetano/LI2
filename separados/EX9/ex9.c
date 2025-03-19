#include <stdio.h>

int strcmp(const char s1[], const char s2[]) {
    int i;

    for (i = 0; s1[i] != '\0' && s2[i] != '\0'; i++) {
        if (s1[i] < s2[i]) {
            return -1; // s1 é menor que s2
        } else if (s1[i] > s2[i]) {
            return 1;  // s1 é maior que s2
        }
    }

    // Se chegamos aqui, uma das strings (ou ambas) terminou
    if (s1[i] == '\0' && s2[i] == '\0') {
        return 0;  // As strings são iguais
    } else if (s1[i] == '\0') {
        return -1; // s1 é mais curta, então é menor
    } else {
        return 1;  // s2 é mais curta, então s1 é maior
    }
}

int main() {
    char str1[] = "Hello";
    char str2[] = "World";

    int result = strcmp(str1, str2);

    if (result == 0) {
        printf("As strings são iguais.\n");
    } else if (result < 0) {
        printf("\"%s\" é menor que \"%s\".\n", str1, str2);
    } else {
        printf("\"%s\" é maior que \"%s\".\n", str1, str2);
    }

    return 0;
}
