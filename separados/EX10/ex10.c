#include <stdio.h>

char *strstr(char s1[], char s2[]) {
    int i, j;
    
    // Se s2 for uma string vazia, retorna s1
    if (s2[0] == '\0') {
        return NULL;
    }
    
    for (i = 0; s1[i] != '\0'; i++) {
        // Verifica se os caracteres de s2 correspondem aos de s1 a partir de i
        for (j = 0; s2[j] != '\0' && s1[i + j] == s2[j]; j++);
        
        // Se chegamos ao final de s2, encontramos a substring
        if (s2[j] == '\0') {
            return &s1[i];
        }
    }
    
    // Se não encontrar a substring, retorna NULL
    return NULL;
}

int main() {
    char s1[] = "Hello, world!";
    char s2[] = "world";
    char *result;

    result = strstr(s1, s2);

    if (result != NULL) {
        printf("Substring encontrada: %s\n", result);
    } else {
        printf("Substring não encontrada.\n");
    }

    return 0;
}