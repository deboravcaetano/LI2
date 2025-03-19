#include <stdio.h>

char *strcpy(char *dest, char source[]) {
    int i;

    for (i = 0; source[i] != '\0'; i++) {
        dest[i] = source[i];
    }

    dest[i] = '\0'; 
    return dest; 
}

int main() {
    char str1[100]; 
    char str2[] = "Hello, world!";

    printf("Antes: str1 = '%s', str2 = '%s'\n", str1, str2);

    strcpy(str1, str2);

    printf("Depois: str1 = '%s'\n", str1);

    return 0;
}
