#include <stdio.h>

int trailingZ(unsigned int n) {
    int cont = 0;
    while (n > 0 && (n % 2 == 0)) {  // Continua enquanto o último bit é 0
        cont++;
        n = n / 2; 
    }
    return cont; 
}

/*
Nota: o 0 ou 1 que sai da primeira divisão por 2 é o do bit menos signficativo
Ex: n = 12 que em binário é 1100, trailingZ vai verificar se n em binário tem 0s da direita para a esquerda
*/


int main() {
    unsigned int numero;

    printf("Digite um número inteiro positivo: ");
    scanf("%u", &numero);

    printf("O número de bits 0 no final é: %d\n", trailingZ(numero)); 
    return 0;
}