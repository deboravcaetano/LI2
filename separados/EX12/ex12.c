#include <stdio.h>

int vogal(char c) {
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}

void strnoV(char s[]) {
    int i, j;
    for (i = 0, j = 0; s[i] != '\0'; i++) {
        if (!vogal(s[i])) {
            s[j] = s[i];
            j++;
        }
    }
    s[j] = '\0';
}

// ver explicação folha 50 questões

int main() {
    char s[] = "Hello World";
    printf("Original: %s\n", s);
    strnoV(s);
    printf("Sem vogais: %s\n", s);
    return 0;
}
