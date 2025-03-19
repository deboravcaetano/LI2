#include <stdio.h>

int qDigFor(unsigned int n) {
    int count;
    if (n == 0) {
        return 1;  // Caso especial: o número 0 tem 1 dígito
    }
    for (count = 0; n > 0; count++) {
        n /= 10;
    }
    return count;
}

/*
Porquê a divisão por 10?

A divisão por 10  remove o último dígito do número.
Exemplo:
12345 / 10 = 1234 (O dígito 5 é removido)

1234 / 10 = 123 (O dígito 4 é removido)

123 / 10 = 12 (O dígito 3 é removido)

12 / 10 = 1 (O dígito 2 é removido)

1 / 10 = 0 (O dígito 1 é removido, e o número torna-se 0)


*/

int main() {
    unsigned int num = 440;
    printf("O número de dígitos em %u é: %d\n", num, qDigFor(num));
    return 0;
}
